/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + SMT32 工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
/*******************************************************************************
  includes
*******************************************************************************/
#include "board.h"
#include "rtthread.h"


/*******************************************************************************
  宏定义
*******************************************************************************/
#define __TEST_MEM_SIZE     100    /**< 内存大小（字节） */


/*******************************************************************************
  变量声明
*******************************************************************************/
/* 定义线程控制块句柄 */
static rt_thread_t __gp_alloc_thread = RT_NULL;
static rt_thread_t __gp_free_thread = RT_NULL;

/* 定义内存池控制块句柄 */
static rt_mp_t __gp_test_mp = RT_NULL;

/* 定义申请内存的指针 */
static rt_uint32_t *__gp_mem;


/*******************************************************************************
  函数声明
*******************************************************************************/
static void __alloc_thread_entry (void *parameter);
static void __free_thread_entry (void *parameter);


/*******************************************************************************
  main函数
*******************************************************************************/
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
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
  线程定义
*******************************************************************************/
static void __alloc_thread_entry (void *parameter)
{
    uint32_t addr;
    
    /* 申请动态内存 */
    __gp_mem = rt_malloc(__TEST_MEM_SIZE);
    if (__gp_mem == RT_NULL) {
        rt_kprintf("rt_malloc err\n");
    } else {
        rt_kprintf(
            "The dynamic memory is allocated successfully, address: 0x%x\n",
             __gp_mem);
    }
    
    /* 往申请到的内存写数据 */
    *__gp_mem = 0xDEADBEEF;
    
    /* 从内存中读数据 */
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
    /* 释放申请到的内存 */
    rt_free(__gp_mem);
    
    while (1) {
        LED2_TOGGLE;
        rt_thread_delay(500);
    }
}


/* end of file */
