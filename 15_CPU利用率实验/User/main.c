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
  includes
*******************************************************************************/
#include "board.h"
#include "rtthread.h"
#include "cpuusage.h"


/*******************************************************************************
  �궨��
*******************************************************************************/


/*******************************************************************************
  ��������
*******************************************************************************/
/* �����߳̿��ƿ��� */
static rt_thread_t __gp_led_thread = RT_NULL;
static rt_thread_t __gp_cpuusage_thread = RT_NULL;


/*******************************************************************************
  ��������
*******************************************************************************/
static void __led_thread_entry (void *parameter);
static void __get_cpuusage_thread_entry (void *parameter);


/*******************************************************************************
  main����
*******************************************************************************/
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
    __gp_led_thread = rt_thread_create("led",
                                        __led_thread_entry,
                                        RT_NULL,
                                        512,
                                        2,
                                        20);
    if (__gp_led_thread == RT_NULL) {
        rt_kprintf("Failed to create __gp_led_thread\n");
        return -1;
    }
    rt_thread_startup(__gp_led_thread);
    
    __gp_cpuusage_thread = rt_thread_create("cpuusage",
                                             __get_cpuusage_thread_entry,
                                             RT_NULL,
                                             512,
                                             3,
                                             20);
    if (__gp_cpuusage_thread == RT_NULL) {
        rt_kprintf("Failed to create __gp_cpuusage_thread\n");
        return -1;
    }
    rt_thread_startup(__gp_cpuusage_thread);
    
    return 0;
}


/*******************************************************************************
  �̶߳���
*******************************************************************************/
static void __led_thread_entry (void *parameter)
{
    uint32_t i;
    
    while (1) {
        
        for (i = 0; i < 1000; i++) 
            LED1_TOGGLE;
       
        rt_thread_delay(1);
    }
}

static void __get_cpuusage_thread_entry (void *parameter)
{
    rt_uint8_t major, minor;
    
    while (1) {
        
        /* ��ȡ CPU ʹ�������� */
        cpu_usage_get(&major, &minor);
        rt_kprintf("CPU usage: %d.%d%%\n", major, minor);
        rt_thread_delay(1000);  /* 1S ��ȡһ�� */
    }
}


/* end of file */
