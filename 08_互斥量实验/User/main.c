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
static rt_thread_t __gp_high_thread = RT_NULL;  /* ������ȼ��߳� */
static rt_thread_t __gp_mid_thread  = RT_NULL;  /* �е����ȼ��߳� */
static rt_thread_t __gp_low_thread  = RT_NULL;  /* ������ȼ��߳� */

/* ���廥�������ƿ� */
static rt_mutex_t __gp_test_mux = RT_NULL;

/* ���Ա�����������Դ��*/
static int __g_test_data = 1;


/*******************************************************************************
  ��������
*******************************************************************************/
static void __high_thread_entry (void *parameter);
static void __mid_thread_entry (void *parameter);
static void __low_thread_entry (void *parameter);

void __delay (uint32_t cnt);


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
    /*
     * ʵ��Ŀ�ģ������߳�high, mid, low�����ȼ��ֱ�Ϊ�ߡ��С��ͣ�low�̻߳�ȡ����
     * ����������mid�̣߳�mid�߳���һ�������ߵ���ѭ��������mid���ȼ���low�ߣ���
     * ���Ļ�CPU������ȥִ��mid�̣߳�������Զ����ص�low�̡߳��������ʱhigh�߳�
     * ȥ��ȡ����������ômid�����ȼ��ͻᱻ���ߵ���highһ��������mid�Ϳ���ȥ��ռ
     * low��CPU��Դ�������깫����Դ���ͷŻ�������Ȼ���ٻص���ʼ���ȼ���
     */
    
    /* ����һ�������� */
    __gp_test_mux = rt_mutex_create(
                           "test_mux",          /* ���������� */
                           RT_IPC_FLAG_FIFO);   /* ������ģʽ FIFO(0x00) */
    if (__gp_test_mux == RT_NULL) {
        rt_kprintf("rt_mutex_create err\n");
        return -1;
    }
    
    __gp_high_thread = rt_thread_create("high",
                                         __high_thread_entry,
                                         RT_NULL,
                                         512,
                                         2,
                                         20);
    if (__gp_high_thread == RT_NULL) {
        rt_kprintf("__gp_high_thread created failed\n");
        return -1;
    }
    rt_thread_startup(__gp_high_thread);
    
    /* ����mid�̣߳��������� */
    __gp_mid_thread = rt_thread_create("mid",
                                        __mid_thread_entry,
                                        RT_NULL,
                                        512,
                                        3,
                                        20);
    if (__gp_mid_thread == RT_NULL) {
        rt_kprintf("__gp_mid_thread created failed\n");
        return -1;
    }
    
    __gp_low_thread = rt_thread_create("low",
                                        __low_thread_entry,
                                        RT_NULL,
                                        512,
                                        4,
                                        20);
    if (__gp_low_thread == RT_NULL) {
        rt_kprintf("__gp_low_thread created failed\n");
        return -1;
    }
    rt_thread_startup(__gp_low_thread);
    
    return 0;
}

void __delay (uint32_t cnt)
{
    while (cnt--);
}


/*******************************************************************************
  �̶߳���
*******************************************************************************/
static void __high_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {   
        /* ��KEY1����ʱ��high�߳�ȥ��ȡ������ */
        if (Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON ) {
            /* ��ȡ������(һֱ�ȴ�) */
            ret = rt_mutex_take(__gp_test_mux, RT_WAITING_FOREVER);
            if (ret != RT_EOK) 
                rt_kprintf("rt_mutex_take err\n");
            else {
                rt_kprintf("high thread: I got the mutex, __g_test_data = %d\n", 
                            __g_test_data);
                /* �ͷŻ����� */
                ret = rt_mutex_release(__gp_test_mux);
                if (ret != RT_EOK)
                    rt_kprintf("rt_mutex_release err\n");
                else 
                    rt_kprintf("high thread: I released the mutex\n");
            }
        }
        rt_thread_delay(20);
    }
}

static void __mid_thread_entry (void *parameter)
{
    while (1) {
        /* ��˯�ߵ���ѭ�� */
        LED1_ON;
        __delay(0xFFFFF);
        LED1_OFF;
        __delay(0xFFFFF);
    }
}

static void __low_thread_entry (void *parameter)
{
    rt_err_t ret = RT_EOK;
    while (1) {
        /* ��ȡ������(һֱ�ȴ�) */
        ret = rt_mutex_take(__gp_test_mux, RT_WAITING_FOREVER);
        if (ret != RT_EOK) {
            rt_kprintf("rt_mutex_take err\n");
        } else {
            rt_kprintf("low thread: low thread take mutex ok\n");
            /* 
             * ����ȡ����������������mid�̣߳������ɹ���Ӧ�û������л���mid�̣߳�
             * ���뵽mid�̵߳���ѭ����
             */
            rt_thread_startup(__gp_mid_thread);
            
            /*
             * ����ִ�е����˵��high�߳���ȥ��ȡ������������low�߳����ȼ�����
             * �ߣ��Ӷ�����ռCPU��Դ������mid�̵߳���ѭ����
             */
            rt_kprintf("low thread: the mutex will be released next\n");
            ret = rt_mutex_release(__gp_test_mux);
            
            /*
             * ������Զ����ִ�е����low�߳�һ�ͷŻ������ͻ��л���high�̣߳�Ȼ
             * ���л���mid�̡߳�
             */
            rt_kprintf("low thread: can I excute here?\n");
        }      
        rt_thread_delay(20);
    }
}

/* end of file */
