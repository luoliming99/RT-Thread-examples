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
  ��������
*******************************************************************************/
/* �����߳̿��ƿ� */
static rt_thread_t __gp_send_thread = RT_NULL;
static rt_thread_t __gp_recv_thread = RT_NULL;
/* ������Ϣ���п��ƿ� */
static rt_mq_t __gp_test_mq = RT_NULL;

/* �������� */
static int __g_send_data = 0;

/* �������� */
static int __g_recv_data;

/*******************************************************************************
  ��������
*******************************************************************************/
static void send_thread_entry (void *parameter);
static void recv_thread_entry (void *parameter);


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
    rt_kprintf("���� K1 ������д����Ϣ���У����� K2 �����ݴ���Ϣ�����ж�����\n");
    
    /* ����һ����Ϣ���� */
    __gp_test_mq = rt_mq_create("test_mq",      /* ��Ϣ�������� */
                           40,                  /* ��Ϣ���д�С */
                           10,                  /* ��Ϣ����������� */
                           RT_IPC_FLAG_FIFO);   /* ����ģʽ FIFO(0x00) */
    if (__gp_test_mq == RT_NULL) {
        rt_kprintf("rt_mq_create err\n");
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
  �̶߳���
*******************************************************************************/
static void send_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {
        if (Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON ) {
            ret = rt_mq_send_wait(__gp_test_mq, 
                                  &__g_send_data, 
                                  sizeof(__g_send_data),
                                  RT_WAITING_FOREVER);
            if (ret != RT_EOK) {
                rt_kprintf("rt_mq_send err\n");
            }
            rt_kprintf("rt_mq_send data: %d\n", __g_send_data);
            __g_send_data ++;
        }
        rt_thread_delay(20);
    }
}

static void recv_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {
        if (Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON ) {
            ret = rt_mq_recv(__gp_test_mq, 
                             &__g_recv_data, 
                             sizeof(__g_recv_data), 
                             RT_WAITING_FOREVER);
            
            if (ret != RT_EOK) {
                rt_kprintf("rt_mq_recv err\n");
            }
            rt_kprintf("rt_mq_recv data: %d\n", __g_recv_data);
        }        
        rt_thread_delay(20);
    }
}



/* end of file */
