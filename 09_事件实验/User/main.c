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


/*******************************************************************************
  �궨��
*******************************************************************************/
#define KEY1_EVENT  (0x1 << 0)  /* �����¼�1���� */
#define KEY2_EVENT  (0x1 << 1)  /* �����¼�2���� */


/*******************************************************************************
  ��������
*******************************************************************************/
/* �����߳̿��ƿ��� */
static rt_thread_t __gp_send_thread = RT_NULL;  
static rt_thread_t __gp_recv_thread = RT_NULL; 

/* �����¼����ƿ��� */
static rt_event_t __gp_test_event = RT_NULL;


/*******************************************************************************
  ��������
*******************************************************************************/
static void __send_thread_entry (void *parameter);
static void __recv_thread_entry (void *parameter);


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
    /* ����һ���¼� */
    __gp_test_event = rt_event_create(
                           "test_event",        /* �¼����� */
                           RT_IPC_FLAG_FIFO);   /* �¼�ģʽ FIFO(0x00) */
    if (__gp_test_event == RT_NULL) {
        rt_kprintf("rt_event_create err\n");
        return -1;
    }
    
    __gp_send_thread = rt_thread_create("send",
                                         __send_thread_entry,
                                         RT_NULL,
                                         512,
                                         2,
                                         20);
    if (__gp_send_thread == RT_NULL) {
        rt_kprintf("__gp_send_thread created failed\n");
        return -1;
    }
    rt_thread_startup(__gp_send_thread);
    
    __gp_recv_thread = rt_thread_create("recv",
                                        __recv_thread_entry,
                                        RT_NULL,
                                        512,
                                        3,
                                        20);
    if (__gp_recv_thread == RT_NULL) {
        rt_kprintf("__gp_recv_thread created failed\n");
        return -1;
    }
    rt_thread_startup(__gp_recv_thread);
    
    return 0;
}


/*******************************************************************************
  �̶߳���
*******************************************************************************/
static void __send_thread_entry (void *parameter)
{
    rt_err_t    ret     = RT_EOK;
    rt_uint8_t  key_val = NO_PRES;
    
    while (1) {
        
        key_val = key_scan();
        
        /* ��KEY1����ʱ������һ���¼�1 */
        if (key_val == KEY1_PRES ) {
            rt_kprintf("KEY1 pressed\n");
            ret = rt_event_send(__gp_test_event, KEY1_EVENT);
            if (ret != RT_EOK) 
                rt_kprintf("event1 sending err\n");
        }
        
        /* ��KEY2����ʱ������һ���¼�2 */
        if (key_val == KEY2_PRES) {
            rt_kprintf("KEY2 pressed\n");
            ret = rt_event_send(__gp_test_event, KEY2_EVENT);
            if (ret != RT_EOK) 
                rt_kprintf("event2 sending err\n");
        }
        
        rt_thread_delay(20);
    }
}

static void __recv_thread_entry (void *parameter)
{
    rt_err_t    ret     = RT_EOK;
    rt_uint32_t recved;
    
    while (1) {
        
        /* ����KEY1��KEY2ͬʱ���µ��¼� */
        ret = rt_event_recv(__gp_test_event,
                            KEY1_EVENT | KEY2_EVENT,
                            RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                            RT_WAITING_FOREVER,
                            &recved);
        if (ret != RT_EOK) {
            rt_kprintf("rt_event_recv err\n");
        }
        
        rt_kprintf("recved = 0x%x\n", recved);
        LED1_TOGGLE;
        rt_thread_delay(20);
    }
}


/* end of file */
