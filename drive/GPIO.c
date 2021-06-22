/******************************************************************/
/* 函数名：                                                */
/* 功能：                                              */
/* 修改日期：                                                       */
/* 内容：                                                    */
/* 作者：QQ:363116119                                        */
/******************************************************************/

#include "gpio.h"
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void GPIO_Conf(void)
{
/**********************GPIO*******************************************************/
	GPIO_InitTypeDef GPIO_InitStructure;//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIO时钟
	
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//FILA
	GPIO_Init(GPIOA, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//LRV
	GPIO_Init(GPIOA, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_12;//V_SW  FAN
	GPIO_Init(GPIOB, &GPIO_InitStructure);//

	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;//
	GPIO_Init(GPIOC, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;//RST_LCD
	GPIO_Init(GPIOD, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//RELAY1
	GPIO_Init(GPIOE, &GPIO_InitStructure);//
	
//	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//DS18B20
//	GPIO_Init(GPIOB, &GPIO_InitStructure);//

	
	GPIO_SetBits(GPIOD,GPIO_Pin_13);//复位LCD
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);//复位LCD
	GPIO_SetBits(GPIOC,GPIO_Pin_6);//LCD复位完成
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//内阻测试为高档位
//	GPIO_ResetBits(GPIOB,GPIO_Pin_10);//内阻测试为高档位
}

void DS18B20_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIO时钟
	
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//DS18B20
	GPIO_Init(GPIOB, &GPIO_InitStructure);//
}

void DS18B20_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIO时钟
	
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//DS18B20
	GPIO_Init(GPIOB, &GPIO_InitStructure);//
}