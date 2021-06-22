#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
#define  CmdNumb         19
#define  CmdLen          9 

#define ChrStartR      0x3C
#define ChrStartS      0x3E

#define ChrEndR    	   0x0D
#define ChrEndS    	   0x0A

#define PARA_ERR       1
#define CMD_ERR        2
#define REM_ERR        3

#define MAXRxTxLen   128

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u16 USART_RX_CNT;
u16 SerialRemoteHandleL(u8 len,char* buf);

void USART_Configuration(void);
void USART2_Configuration(void);
void USART3_Configuration();
void USART3WriteDMA(u16 len);
void USART3HANDLE(void);
#endif