/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * 名称    :usart.h
 * 作者    :李振
 * 描述    :
 * 软件功能:
 * 硬件外设:
 * 修改日期:2015-2-5
*******************************************************************/
	
#ifndef _usart_h_
#define _usart_h_

void USART_Configuration(void);//串口初始化函数
void USART2_Configuration(void);
void UART1_Send(void);
static void USART1_NVIC_Config(void);
static void USART2_NVIC_Config(void);
#endif