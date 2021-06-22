/******************** (C) COPYRIGHT 2015 AVER ********************
 * �ļ���  ��DAC8531.C
 * ����    ������
 * ����    ���⺯����
 * ����    ������16BIT DAC
 * Ӳ�����ӣ�
 * �޸����ڣ�2015-8-18
*********************************************************************/
#include "my_register.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
/********************************************************************/
#define DAC_SYNC_LOW   GPIO_ResetBits(GPIOE, GPIO_Pin_5);
#define DAC_SYNC_HIGH  GPIO_SetBits(GPIOE, GPIO_Pin_5);   
      
#define DAC_SCK_LOW   GPIO_ResetBits(GPIOE, GPIO_Pin_4);
#define DAC_SCK_HIGH  GPIO_SetBits(GPIOE, GPIO_Pin_4); 

#define DAC_SDA_LOW   GPIO_ResetBits(GPIOE, GPIO_Pin_6);
#define DAC_SDA_HIGH  GPIO_SetBits(GPIOE, GPIO_Pin_6); 
/**********************************************************************************************************
*	�� �� ��: AD5541_GPIOCoing
*	����˵��: ���ų�ʼ��
*	��    �Σ�
*	�� �� ֵ: ��
**********************************************************************************************************/
void AD5541_GPIOCoing(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO�ṹ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//����GPIOFʱ��

	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//����˿�PE
	GPIO_Init(GPIOE, &GPIO_InitStructure);//���������뺯��
	
}
/**********************************************************************************************************
*	�� �� ��: AD5541_GPIOCoing
*	����˵��: ���ų�ʼ��
*	��    �Σ�
*	�� �� ֵ: ��
**********************************************************************************************************/
void AD5541_Send(vu16 date)
{
	vu8 cont;
	DAC_SYNC_LOW;//SYNC�õ�
	DAC_SCK_HIGH;//SCK�ø�
	NOP;
	NOP;
	NOP;
	NOP;
	DAC_SYNC_HIGH;//SYNC�ø�
	NOP;
	DAC_SCK_HIGH;//SCK�ø�
	NOP;
	NOP;
	DAC_SYNC_LOW;//SYNC�õ�
	for(cont = 0; cont < 16; cont++)
	{
		if(date & 0x8000)		// �ȴ���λ
		{
			DAC_SDA_HIGH;
		}
		else
		{
			DAC_SDA_LOW;
		}
		DAC_SCK_LOW;//SCK�õ�
		NOP;
		NOP;
		DAC_SCK_HIGH;//SCK�ø�
		NOP;
		NOP;
		date=date<<1;
	}
	DAC_SYNC_HIGH;//SYNC�ø�
	DAC_SCK_HIGH;//SCK�ø�
}
