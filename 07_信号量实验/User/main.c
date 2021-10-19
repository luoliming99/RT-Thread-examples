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
#define __SEM_VAL_INIT  1   /**< �ź�����ʼֵ */
/*******************************************************************************
  ��������
*******************************************************************************/
/* �����߳̿��ƿ� */
static rt_thread_t __gp_send_thread = RT_NULL;
static rt_thread_t __gp_recv_thread = RT_NULL;
/* �����ź������ƿ� */
static rt_sem_t __gp_test_sem = RT_NULL;

static int __g_sem_value = __SEM_VAL_INIT;


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
    /* ����һ���ź��� */
    __gp_test_sem = rt_sem_create(
                           "test_sem",          /* �ź������� */
                           1,                   /* �ź�����ʼֵ */
                           RT_IPC_FLAG_FIFO);   /* �ź���ģʽ FIFO(0x00) */
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
  �̶߳���
*******************************************************************************/
static void send_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {   
        /* ��ȡ�ź���(�ȴ�10 ticks) */
        ret = rt_sem_take(__gp_test_sem, 10);
        if (ret != RT_EOK) {
            rt_kprintf("rt_sem_take err\n");
        } else {
            printf("__g_sem_value: %d\n", __g_sem_value);
            /* �ͷ��ź��� */
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
            /* ��ȡ�ź���(һֱ�ȴ�) */
            ret = rt_sem_take(__gp_test_sem, RT_WAITING_FOREVER);
            if (ret != RT_EOK) 
                rt_kprintf("rt_sem_take err\n");
            else
                rt_kprintf("take semaphore %d\n", --__g_sem_value);
        }
        if (Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON ) {
            /* �ͷ��ź��� */
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
