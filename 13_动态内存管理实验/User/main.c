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
#define __TEST_MEM_SIZE     100    /**< �ڴ��С���ֽڣ� */


/*******************************************************************************
  ��������
*******************************************************************************/
/* �����߳̿��ƿ��� */
static rt_thread_t __gp_alloc_thread = RT_NULL;
static rt_thread_t __gp_free_thread = RT_NULL;

/* �����ڴ�ؿ��ƿ��� */
static rt_mp_t __gp_test_mp = RT_NULL;

/* ���������ڴ��ָ�� */
static rt_uint32_t *__gp_mem;


/*******************************************************************************
  ��������
*******************************************************************************/
static void __alloc_thread_entry (void *parameter);
static void __free_thread_entry (void *parameter);


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
    __gp_alloc_thread = rt_thread_create("alloc",
                                        __alloc_thread_entry,
                                        RT_NULL,
                                        512,
                                        2,
                                        20);
    if (__gp_alloc_thread == RT_NULL) {
        rt_kprintf("Failed to create __gp_alloc_thread\n");
        return -1;
    }
    rt_thread_startup(__gp_alloc_thread);
    
    __gp_free_thread = rt_thread_create("free",
                                        __free_thread_entry,
                                        RT_NULL,
                                        512,
                                        3,
                                        20);
    if (__gp_free_thread == RT_NULL) {
        rt_kprintf("Failed to create __gp_free_thread\n");
        return -1;
    }
    rt_thread_startup(__gp_free_thread);
    
    return 0;
}


/*******************************************************************************
  �̶߳���
*******************************************************************************/
static void __alloc_thread_entry (void *parameter)
{
    uint32_t addr;
    
    /* ���붯̬�ڴ� */
    __gp_mem = rt_malloc(__TEST_MEM_SIZE);
    if (__gp_mem == RT_NULL) {
        rt_kprintf("rt_malloc err\n");
    } else {
        rt_kprintf(
            "The dynamic memory is allocated successfully, address: 0x%x\n",
             __gp_mem);
    }
    
    /* �����뵽���ڴ�д���� */
    *__gp_mem = 0xDEADBEEF;
    
    /* ���ڴ��ж����� */
    rt_kprintf("The data is: 0x%x, the addr is: 0x%x\n",
               *__gp_mem, __gp_mem);
    
    rt_kprintf("__gp_test_mp addr: %x\n", __gp_test_mp);
    for (addr = (uint32_t)((uint8_t *)__gp_mem - 4); 
         addr < (uint32_t)((uint8_t *)__gp_mem + 12); 
         addr++) {
        rt_kprintf("%08x: %02x\n", 
                    addr, 
                   *((uint8_t *)addr));
    }
    while (1) {
        LED1_TOGGLE;
        rt_thread_delay(1000);
    }
}

static void __free_thread_entry (void *parameter)
{
    /* �ͷ����뵽���ڴ� */
    rt_free(__gp_mem);
    
    while (1) {
        LED2_TOGGLE;
        rt_thread_delay(500);
    }
}


/* end of file */
