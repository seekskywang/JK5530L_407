/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   adc1 Ӧ��bsp / DMA ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "dac.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
/**
  * @brief  ʹ��DAC��ʱ�ӣ���ʼ��GPIO
  * @param  ��
  * @retval ��
  */
void DAC_Config(void)
{
  DAC_InitTypeDef  DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��DACʱ�� */	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
  /* DAC��GPIO���ã�ģ������ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	


  /* ����DAC ͨ��1 */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;						//��ʹ���ⲿ����Դ
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//��ʹ�ò��η�����
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//��ʹ��DAC�������
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* ʹ��ͨ��1 ��PA4��� */
  DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}






























