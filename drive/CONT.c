/******************** (C) COPYRIGHT 2015 AVER ********************
 * 文件名  ：CONT.C
 * 作者    ：李振
 * 描述    ：库函数版
 * 内容    ：外设控制函数
 * 硬件连接：
 * 修改日期：2015-8-18
*********************************************************************/
#include "my_register.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

void MODE_CONT(void)
{
	/*********电流档位切换*************/
	if(LOAD_I_SW==1)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);//电流测量为高档位
	}
	else
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_11);//电流测量为低档位
	}
//	if(LOAD_I_SW==0)
//	{
//		if(Current>10000  && calflag == 0)//当测量电流大于低档最高电流时自动切换成高档位且锁定，需手动切换才能变成低档位
//		{
//			GPIO_ResetBits(GPIOC,GPIO_Pin_11);//电流测量为高档位
//			LOAD_I_SW=1;//自动切换为高档位
//		}
//	}
	/*************电压档位切换**************/
	if(LOAD_V_SW==0)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);//电压档位为低档 0-12V
	}
	else
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_1);//电压档位为高档 0-80V
	}
	if(LOAD_V_SW==0)
	{
		if(Para.CVoltage>12000 && calflag == 0)//当测量电压高于低档最大限制电压是档位自动跳转到高档
		{
			LOAD_V_SW=1;
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//电压档位为高档
		}
	}else if(LOAD_V_SW==1){
		if(Para.CVoltage<10000 && calflag == 0)
		{
			LOAD_V_SW=0;
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//电压档位为低档
		}
	}
	/***********内阻档位切换**************/
	if(RCONT_SW==0)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);//
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);//
	}
//	RCONT_SW=1;
	if(Rraly == 0)
	{
		if(RCONT_SW==0)
		{
			if(Para.CR_VLUE>210 && calflag == 0)//当测量电压高于低档最大限制电压是档位自动跳转到高档
			{
				RCONT_SW=1;
			}
		}else if(RCONT_SW==1){
			if(Para.CR_VLUE<190 && calflag == 0)
			{
				RCONT_SW=0;
			}
		}
	}else if(Rraly == 1){
		RCONT_SW=0;
	}else if(Rraly == 2){
		RCONT_SW=1;
	}
	/**********负载模式切换***************/
	if(LOAD_MODE==0 || LOAD_MODE==2/*CP模式*/)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_12);//CC模式
	}
	else if(LOAD_MODE==1)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_12);//CV模式
	}
}