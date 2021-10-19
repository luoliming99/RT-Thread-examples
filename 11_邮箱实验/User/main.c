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


/*******************************************************************************
  ��������
*******************************************************************************/
/* �����߳̿��ƿ��� */
static rt_thread_t __gp_send_thread = RT_NULL;
static rt_thread_t __gp_recv_thread = RT_NULL;

/* ����������ƿ��� */
static rt_mailbox_t __gp_test_mb = RT_NULL;

/* ���������ʼ����ֱ��� KEY1 �� KEY2 ���� */
static char __g_mail_1[] = {"Hello, I am mail 1"};
static char __g_mail_2[] = {"Hi, I am mail 2"};

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
    /* �������� */
    __gp_test_mb = rt_mb_create(
                        "test_mb",              /* �������� */
                        10,                     /* ����������С */ 
                        RT_IPC_FLAG_FIFO);      /* ����ģʽ FIFO(0x00) */
    if (__gp_test_mb == RT_NULL) {
        rt_kprintf("rt_mb_create err\n");
        return -1;
    }
   
    __gp_send_thread = rt_thread_create("send",
                                        __send_thread_entry,
                                        RT_NULL,
                                        512,
                                        2,
                                        20);
    if (__gp_send_thread == RT_NULL) {
        rt_kprintf("Failed to create __gp_send_thread\n");
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
        rt_kprintf("Failed to create __gp_recv_thread\n");
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
    rt_err_t ret     = RT_EOK;
    uint8_t  key_val = NO_PRES;
    
    while (1) {
        
        key_val = key_scan();
        
        if (key_val == KEY1_PRES) {
            rt_kprintf("KEY1 pressed\n");
            ret = rt_mb_send(__gp_test_mb, (rt_ubase_t)__g_mail_1);
            if (ret != RT_EOK) {
                rt_kprintf("Failed to send __g_mail_1\n");
            } else {
                rt_kprintf("__g_mail_1 sent sucessfully\n");
            }     
        }
        if (key_val == KEY2_PRES) {
            rt_kprintf("KEY2 pressed\n");
            ret = rt_mb_send(__gp_test_mb, (rt_ubase_t)__g_mail_2);
            if (ret != RT_EOK) {
                rt_kprintf("Failed to send __g_mail_2\n");
            } else {
                rt_kprintf("__g_mail_2 sent sucessfully\n");
            }     
        }
        
        rt_thread_delay(20);
    }
}

static void __recv_thread_entry (void *parameter)
{
    rt_err_t    ret        = RT_EOK;
    rt_ubase_t  mail;
    
    while (1) {
        ret = rt_mb_recv(__gp_test_mb,          /* ��������� */
                         &mail,                 /* ��Ž��յ����ʼ� */
                         RT_WAITING_FOREVER);   /* һֱ�ȴ� */
        if (ret != RT_EOK) {
            rt_kprintf("Failed to receive mail");
        } else {
            LED1_TOGGLE;
            rt_kprintf("Received mail: %s\n", (char *)mail);
        }  
        rt_thread_delay(1000);  /* 5S ����һ���ʼ� */
    }
}


/* end of file */
