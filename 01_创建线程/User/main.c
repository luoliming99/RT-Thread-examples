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
  头文件声明
*******************************************************************************/
#include "board.h"
#include "rtthread.h"

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_led1_thread_stack[512];    /* 线程栈 */
static struct rt_thread led1_thread;

static void led1_thread_entry (void *parameter)
{
    while (1) {
        LED1_ON;
        rt_thread_delay(500);
        
        LED1_OFF;
        rt_thread_delay(500);
    }
}


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	rt_thread_init(&led1_thread,
                   "led1",
                   led1_thread_entry,
                   RT_NULL,
                   &rt_led1_thread_stack[0],
                   sizeof(rt_led1_thread_stack),
                   3,
                   20);
    rt_thread_startup(&led1_thread);
}


/* end of file */
