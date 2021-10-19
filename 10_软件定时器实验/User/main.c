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
/* ���������ʱ�����ƿ��� */
static rt_timer_t __gp_soft_tim1 = RT_NULL;  
static rt_timer_t __gp_soft_tim2 = RT_NULL; 

static uint32_t __g_tim1_cnt = 0;
static uint32_t __g_tim2_cnt = 0;

/*******************************************************************************
  ��������
*******************************************************************************/
static void __soft_tim1_callback (void *parameter);
static void __soft_tim2_callback (void *parameter);


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
    /* ������ʱ�� 1 */
    __gp_soft_tim1 = rt_timer_create(
                       "soft_tim1",             /* �����ʱ������ */
                       __soft_tim1_callback,    /* �����ʱ����ʱ���� */
                       RT_NULL,                 /* �����ʱ����ʱ������ڲ��� */
                       5000,                    /* �����ʱ���ĳ�ʱʱ�� */
                                                /* ����ģʽ | �����ʱ�� */
                       RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);   
    if (__gp_soft_tim1 == RT_NULL) {
        rt_kprintf("software timer 1 creating err\n");
        return -1;
    }
    /* ������ʱ�� 1 */
    rt_timer_start(__gp_soft_tim1);
    
    /* ������ʱ�� 2 */
    __gp_soft_tim2 = rt_timer_create(
                       "soft_tim2",             /* �����ʱ������ */
                       __soft_tim2_callback,    /* �����ʱ����ʱ���� */
                       RT_NULL,                 /* �����ʱ����ʱ������ڲ��� */
                       1000,                    /* �����ʱ���ĳ�ʱʱ�� */
                                                /* ����ģʽ | �����ʱ�� */
                       RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);   
    if (__gp_soft_tim2 == RT_NULL) {
        rt_kprintf("software timer 2 creating err\n");
        return -1;
    }
    /* ������ʱ�� 2 */
    rt_timer_start(__gp_soft_tim2);
    
    return 0;
}


/*******************************************************************************
  �̶߳���
*******************************************************************************/
static void __soft_tim1_callback (void *parameter)
{
    uint32_t tick_cnt;
    
    __g_tim1_cnt ++;
    tick_cnt = rt_tick_get();
    
    rt_kprintf("tick_cnt = %5d, __g_tim1_cnt = %2d\n", 
                tick_cnt, __g_tim1_cnt);
}

static void __soft_tim2_callback (void *parameter)
{
    uint32_t tick_cnt;
    
    __g_tim2_cnt ++;
    tick_cnt = rt_tick_get();
    
    rt_kprintf("tick_cnt = %5d, __g_tim2_cnt = %2d\n", 
                tick_cnt, __g_tim2_cnt);
}


/* end of file */
