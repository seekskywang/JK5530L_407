/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * ����    :usart.h
 * ����    :����
 * ����    :
 * �������:
 * Ӳ������:
 * �޸�����:2015-2-5
*******************************************************************/
	
#ifndef _usart_h_
#define _usart_h_

void USART_Configuration(void);//���ڳ�ʼ������
void USART2_Configuration(void);
void UART1_Send(void);
static void USART1_NVIC_Config(void);
static void USART2_NVIC_Config(void);
#endif