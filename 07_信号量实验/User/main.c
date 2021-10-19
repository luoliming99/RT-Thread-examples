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
  宏定义
*******************************************************************************/
#define __SEM_VAL_INIT  1   /**< 信号量初始值 */
/*******************************************************************************
  变量声明
*******************************************************************************/
/* 定义线程控制块 */
static rt_thread_t __gp_send_thread = RT_NULL;
static rt_thread_t __gp_recv_thread = RT_NULL;
/* 定义信号量控制块 */
static rt_sem_t __gp_test_sem = RT_NULL;

static int __g_sem_value = __SEM_VAL_INIT;


/*******************************************************************************
  函数声明
*******************************************************************************/
static void send_thread_entry (void *parameter);
static void recv_thread_entry (void *parameter);


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
    /* 创建一个信号量 */
    __gp_test_sem = rt_sem_create(
                           "test_sem",          /* 信号量名字 */
                           1,                   /* 信号量初始值 */
                           RT_IPC_FLAG_FIFO);   /* 信号量模式 FIFO(0x00) */
    if (__gp_test_sem == RT_NULL) {
        rt_kprintf("rt_sem_create err\n");
        return -1;
    }
    
    __gp_send_thread = rt_thread_create("send",
                                        send_thread_entry,
                                        RT_NULL,
                                        512,
                                        3,
                                        20);
    if (__gp_send_thread != RT_NULL) {
        rt_thread_startup(__gp_send_thread);
    } else {
        rt_kprintf("send_thread created failed\n");
        return -1;
    }
    
    __gp_recv_thread = rt_thread_create("recv",
                                        recv_thread_entry,
                                        RT_NULL,
                                        512,
                                        2,
                                        20);
    if (__gp_recv_thread != RT_NULL) {
        rt_thread_startup(__gp_recv_thread);
    } else {
        rt_kprintf("recv_thread created failed\n");
        return -1;
    }
    
    return 0;
}


/*******************************************************************************
  线程定义
*******************************************************************************/
static void send_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {   
        /* 获取信号量(等待10 ticks) */
        ret = rt_sem_take(__gp_test_sem, 10);
        if (ret != RT_EOK) {
            rt_kprintf("rt_sem_take err\n");
        } else {
            printf("__g_sem_value: %d\n", __g_sem_value);
            /* 释放信号量 */
            ret = rt_sem_release(__gp_test_sem);
            if (ret != RT_EOK) {
                rt_kprintf("rt_sem_release err\n");
            }
        }
        rt_thread_delay(1000);
    }
}

static void recv_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {
        if (Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON ) {
            /* 获取信号量(一直等待) */
            ret = rt_sem_take(__gp_test_sem, RT_WAITING_FOREVER);
            if (ret != RT_EOK) 
                rt_kprintf("rt_sem_take err\n");
            else
                rt_kprintf("take semaphore %d\n", --__g_sem_value);
        }
        if (Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON ) {
            /* 释放信号量 */
            ret = rt_sem_release(__gp_test_sem);
            if (ret != RT_EOK)
                rt_kprintf("rt_sem_release err\n");
            else 
                rt_kprintf("release semaphore %d\n", ++__g_sem_value);
            
        }        
        rt_thread_delay(20);
    }
}



/* end of file */
