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


/*******************************************************************************
  变量声明
*******************************************************************************/
/* 定义软件定时器控制块句柄 */
static rt_timer_t __gp_soft_tim1 = RT_NULL;  
static rt_timer_t __gp_soft_tim2 = RT_NULL; 

static uint32_t __g_tim1_cnt = 0;
static uint32_t __g_tim2_cnt = 0;

/*******************************************************************************
  函数声明
*******************************************************************************/
static void __soft_tim1_callback (void *parameter);
static void __soft_tim2_callback (void *parameter);


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
    /* 创建定时器 1 */
    __gp_soft_tim1 = rt_timer_create(
                       "soft_tim1",             /* 软件定时器名字 */
                       __soft_tim1_callback,    /* 软件定时器超时函数 */
                       RT_NULL,                 /* 软件定时器超时函数入口参数 */
                       5000,                    /* 软件定时器的超时时间 */
                                                /* 单次模式 | 软件定时器 */
                       RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);   
    if (__gp_soft_tim1 == RT_NULL) {
        rt_kprintf("software timer 1 creating err\n");
        return -1;
    }
    /* 启动定时器 1 */
    rt_timer_start(__gp_soft_tim1);
    
    /* 创建定时器 2 */
    __gp_soft_tim2 = rt_timer_create(
                       "soft_tim2",             /* 软件定时器名字 */
                       __soft_tim2_callback,    /* 软件定时器超时函数 */
                       RT_NULL,                 /* 软件定时器超时函数入口参数 */
                       1000,                    /* 软件定时器的超时时间 */
                                                /* 周期模式 | 软件定时器 */
                       RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);   
    if (__gp_soft_tim2 == RT_NULL) {
        rt_kprintf("software timer 2 creating err\n");
        return -1;
    }
    /* 启动定时器 2 */
    rt_timer_start(__gp_soft_tim2);
    
    return 0;
}


/*******************************************************************************
  线程定义
*******************************************************************************/
static void __soft_tim1_callback (void *parameter)
{
    uint32_t tick_cnt;
    
    __g_tim1_cnt ++;
    tick_cnt = rt_tick_get();
    
    rt_kprintf("tick_cnt = %5d, __g_tim1_cnt = %2d\n", 
                tick_cnt, __g_tim1_cnt);
}

static void __soft_tim2_callback (void *parameter)
{
    uint32_t tick_cnt;
    
    __g_tim2_cnt ++;
    tick_cnt = rt_tick_get();
    
    rt_kprintf("tick_cnt = %5d, __g_tim2_cnt = %2d\n", 
                tick_cnt, __g_tim2_cnt);
}


/* end of file */
