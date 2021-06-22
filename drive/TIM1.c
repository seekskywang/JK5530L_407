/******************************************************************/
/* 名称TIM1 PWM                                             */
/* 效果：                                                        */
/* 内容：互补输出PWM                                              */
/* 作者：李振                                                    */
/* 联系方式：QQ:363116119                                        */
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
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*****************************************************************/
void TIM1_PWM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* TIM3 的配置 ---------------------------------------------------
   TIM3 输入时钟(TIM3CLK) 设置为 APB2 时钟 (PCLK2)    
    => TIM3CLK = PCLK2 = SystemCoreClock
   TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock 为48 MHz */
  /* TIM1 时钟使能 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM1_Config();//TIM1_GPIO初始化

  /* Time 定时基础设置*/
  TIM_TimeBaseStructure.TIM_Prescaler = 4;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time 定时设置为上升沿计算模式*/
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

  /* 频道1的PWM 模式设置 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0x5DC0;//使能频道1配置
	
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	PWM_FI=33000;
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_SetAutoreload(TIM1, PWM_FI);//设置PWM分辨率
	TIM_ARRPreloadConfig(TIM1, ENABLE);
  /* TIM1 计算器使能*/
  TIM_Cmd(TIM1, ENABLE);
	
  /* TIM1 主输出使能 */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);	
	TIM_SetCompare1(TIM1,PWM_FI/2);
}