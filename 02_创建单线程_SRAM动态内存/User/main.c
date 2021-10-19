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

/* �����߳̿��ƿ�ָ�� */
static rt_thread_t led1_thread = RT_NULL;

static void led1_thread_entry (void *parameter);

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
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
