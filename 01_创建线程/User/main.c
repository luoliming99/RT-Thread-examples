/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + SMT32 ����ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-MINI STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
/*******************************************************************************
  ͷ�ļ�����
*******************************************************************************/
#include "board.h"
#include "rtthread.h"

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_led1_thread_stack[512];    /* �߳�ջ */
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
  * @brief  ������
  * @param  ��  
  * @retval ��
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
