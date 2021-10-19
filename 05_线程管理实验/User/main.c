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
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t key_thread = RT_NULL;


/*******************************************************************************
  函数声明
*******************************************************************************/
static void led1_thread_entry (void *parameter);
static void key_thread_entry (void *parameter);


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
    
    key_thread = rt_thread_create("key",
                                   key_thread_entry,
                                   RT_NULL,
                                   512,
                                   2,
                                   20);
    if (key_thread != RT_NULL) {
        rt_thread_startup(key_thread);
    } else {
        return -1;
    }
    
    return 0;
}


/*******************************************************************************
  线程定义
*******************************************************************************/
static void led1_thread_entry (void *parameter)
{
    while (1) {
        LED1_ON;
//        rt_thread_delay(500);
        
        LED1_OFF;
//        rt_thread_delay(500);
    }
}

static void key_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {
        if (Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON ) {
            rt_kprintf("suspend led1_thread ... \r\n");
            ret = rt_thread_suspend(led1_thread);
            if (ret == RT_EOK) {
                rt_kprintf("led1_thread has been suspended sucessfully \r\n");
            } else {
                rt_kprintf("led1_thread suspended failed 0x%1x \r\n", ret);
            }
        }
        if (Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON ) {
            rt_kprintf("resume led1_thread ... \r\n");
            ret = rt_thread_resume(led1_thread);
            if (ret == RT_EOK) {
                rt_kprintf("led1_thread has been resumed sucessfully \r\n");
            } else {
                rt_kprintf("led1_thread resumed failed 0x%1x \r\n", ret);
            }
        }   
        rt_thread_delay(20);
    }
}


/* end of file */
