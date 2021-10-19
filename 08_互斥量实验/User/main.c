/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + SMT32 工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
/*******************************************************************************
  includes
*******************************************************************************/
#include "board.h"
#include "rtthread.h"


/*******************************************************************************
  变量声明
*******************************************************************************/
/* 定义线程控制块 */
static rt_thread_t __gp_high_thread = RT_NULL;  /* 最高优先级线程 */
static rt_thread_t __gp_mid_thread  = RT_NULL;  /* 中等优先级线程 */
static rt_thread_t __gp_low_thread  = RT_NULL;  /* 最低优先级线程 */

/* 定义互斥量控制块 */
static rt_mutex_t __gp_test_mux = RT_NULL;

/* 测试变量（公共资源）*/
static int __g_test_data = 1;


/*******************************************************************************
  函数声明
*******************************************************************************/
static void __high_thread_entry (void *parameter);
static void __mid_thread_entry (void *parameter);
static void __low_thread_entry (void *parameter);

void __delay (uint32_t cnt);


/*******************************************************************************
  main函数
*******************************************************************************/
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
    /*
     * 实验目的：三个线程high, mid, low的优先级分别为高、中、低，low线程获取到互
     * 斥量后启动mid线程，mid线程是一个不休眠的死循环，由于mid优先级比low高，这
     * 样的话CPU会立即去执行mid线程，并且永远不会回到low线程。但如果此时high线程
     * 去获取互斥量，那么mid的优先级就会被升高到和high一样，这样mid就可以去抢占
     * low的CPU资源，处理完公共资源，释放互斥量，然后再回到初始优先级。
     */
    
    /* 创建一个互斥量 */
    __gp_test_mux = rt_mutex_create(
                           "test_mux",          /* 互斥量名字 */
                           RT_IPC_FLAG_FIFO);   /* 互斥量模式 FIFO(0x00) */
    if (__gp_test_mux == RT_NULL) {
        rt_kprintf("rt_mutex_create err\n");
        return -1;
    }
    
    __gp_high_thread = rt_thread_create("high",
                                         __high_thread_entry,
                                         RT_NULL,
                                         512,
                                         2,
                                         20);
    if (__gp_high_thread == RT_NULL) {
        rt_kprintf("__gp_high_thread created failed\n");
        return -1;
    }
    rt_thread_startup(__gp_high_thread);
    
    /* 创建mid线程，但不启动 */
    __gp_mid_thread = rt_thread_create("mid",
                                        __mid_thread_entry,
                                        RT_NULL,
                                        512,
                                        3,
                                        20);
    if (__gp_mid_thread == RT_NULL) {
        rt_kprintf("__gp_mid_thread created failed\n");
        return -1;
    }
    
    __gp_low_thread = rt_thread_create("low",
                                        __low_thread_entry,
                                        RT_NULL,
                                        512,
                                        4,
                                        20);
    if (__gp_low_thread == RT_NULL) {
        rt_kprintf("__gp_low_thread created failed\n");
        return -1;
    }
    rt_thread_startup(__gp_low_thread);
    
    return 0;
}

void __delay (uint32_t cnt)
{
    while (cnt--);
}


/*******************************************************************************
  线程定义
*******************************************************************************/
static void __high_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {   
        /* 当KEY1按下时，high线程去获取互斥量 */
        if (Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON ) {
            /* 获取互斥量(一直等待) */
            ret = rt_mutex_take(__gp_test_mux, RT_WAITING_FOREVER);
            if (ret != RT_EOK) 
                rt_kprintf("rt_mutex_take err\n");
            else {
                rt_kprintf("high thread: I got the mutex, __g_test_data = %d\n", 
                            __g_test_data);
                /* 释放互斥量 */
                ret = rt_mutex_release(__gp_test_mux);
                if (ret != RT_EOK)
                    rt_kprintf("rt_mutex_release err\n");
                else 
                    rt_kprintf("high thread: I released the mutex\n");
            }
        }
        rt_thread_delay(20);
    }
}

static void __mid_thread_entry (void *parameter)
{
    while (1) {
        /* 无睡眠的死循环 */
        LED1_ON;
        __delay(0xFFFFF);
        LED1_OFF;
        __delay(0xFFFFF);
    }
}

static void __low_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {
        /* 获取互斥量(一直等待) */
        ret = rt_mutex_take(__gp_test_mux, RT_WAITING_FOREVER);
        if (ret != RT_EOK) {
            rt_kprintf("rt_mutex_take err\n");
        } else {
            rt_kprintf("low thread: low thread take mutex ok\n");
            /* 
             * 若获取到互斥量，就启动mid线程，启动成功后应该会立马切换到mid线程，
             * 陷入到mid线程的死循环。
             */
            rt_thread_startup(__gp_mid_thread);
            
            /*
             * 程序执行到这里，说明high线程有去获取互斥量，导致low线程优先级被升
             * 高，从而能抢占CPU资源，跳出mid线程的死循环。
             */
            rt_kprintf("low thread: the mutex will be released next\n");
            ret = rt_mutex_release(__gp_test_mux);
            
            /*
             * 程序永远不会执行到这里，low线程一释放互斥量就会切换到high线程，然
             * 后切换到mid线程。
             */
            rt_kprintf("low thread: can I excute here?\n");
        }      
        rt_thread_delay(20);
    }
}

/* end of file */
