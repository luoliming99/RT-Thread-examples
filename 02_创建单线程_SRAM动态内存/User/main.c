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

/* 定义线程控制块指针 */
static rt_thread_t led1_thread = RT_NULL;

static void led1_thread_entry (void *parameter);

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
    led1_thread = rt_thread_create("led1",
                                   led1_thread_entry,
                                   RT_NULL,
                                   512,
                                   3,
                                   20);
    if (led1_thread != RT_NULL) {
        rt_thread_startup(led1_thread);
    } else {
        return -1;
    }
    return 0;
}

static void led1_thread_entry (void *parameter)
{
    while (1) {
        LED1_ON;
        rt_thread_delay(500);
        
        LED1_OFF;
        rt_thread_delay(500);
    }
}


/* end of file */
