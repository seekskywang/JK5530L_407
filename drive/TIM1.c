/******************************************************************/
/* ���ƣHTIM1 PWM                                             */
/* Ч����                                                        */
/* ���ݣ��������PWM                                              */
/* ���ߣ�����                                                    */
/* ��ϵ��ʽ��QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
/*****************************************************************/
vu16 PWM_FI;
void TIM1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;;
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
  /* GPIO_INT*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*****************************************************************/
void TIM1_PWM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* TIM3 ������ ---------------------------------------------------
   TIM3 ����ʱ��(TIM3CLK) ����Ϊ APB2 ʱ�� (PCLK2)    
    => TIM3CLK = PCLK2 = SystemCoreClock
   TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock Ϊ48 MHz */
  /* TIM1 ʱ��ʹ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM1_Config();//TIM1_GPIO��ʼ��

  /* Time ��ʱ��������*/
  TIM_TimeBaseStructure.TIM_Prescaler = 4;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time ��ʱ����Ϊ�����ؼ���ģʽ*/
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

  /* Ƶ��1��PWM ģʽ���� */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0x5DC0;//ʹ��Ƶ��1����
	
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	PWM_FI=33000;
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_SetAutoreload(TIM1, PWM_FI);//����PWM�ֱ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);
  /* TIM1 ������ʹ��*/
  TIM_Cmd(TIM1, ENABLE);
	
  /* TIM1 �����ʹ�� */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);	
	TIM_SetCompare1(TIM1,PWM_FI/2);
}