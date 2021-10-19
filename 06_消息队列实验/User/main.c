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
  变量声明
*******************************************************************************/
/* 定义线程控制块 */
static rt_thread_t __gp_send_thread = RT_NULL;
static rt_thread_t __gp_recv_thread = RT_NULL;
/* 定义消息队列控制块 */
static rt_mq_t __gp_test_mq = RT_NULL;

/* 发送数据 */
static int __g_send_data = 0;

/* 接收数据 */
static int __g_recv_data;

/*******************************************************************************
  函数声明
*******************************************************************************/
static void send_thread_entry (void *parameter);
static void recv_thread_entry (void *parameter);


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
    rt_kprintf("按下 K1 将数据写入消息队列，按下 K2 将数据从消息队列中读出来\n");
    
    /* 创建一个消息队列 */
    __gp_test_mq = rt_mq_create("test_mq",      /* 消息队列名字 */
                           40,                  /* 消息队列大小 */
                           10,                  /* 消息队列最大容量 */
                           RT_IPC_FLAG_FIFO);   /* 队列模式 FIFO(0x00) */
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
  线程定义
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
