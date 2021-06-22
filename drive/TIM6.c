/******************************************************************/
/* 名称TIM3 PWM                                                */
/* 效果：                                                        */
/* 内容：产生一个200HZ 正占空比：60.9% 负占空比：30.9%的PWM      */
/* 作者：李振                                                    */
/* 联系方式：QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "tim6.h"
/*****************************************************************/
/*****************************************************************/
u32 Tick_10ms=0;
u32 OldTick;
extern u8 g_mods_timeout;

void MODS_Poll(void)
{
	uint16_t addr;
	static uint16_t crc1;
    static u32 testi;
	/* 超过3.5个字符时间后执行MODH_RxTimeOut()函数。全局变量 g_rtu_timeout = 1; 通知主程序开始解砿*/
//	if (g_mods_timeout == 0)	
//	{
//		return;								/* 没有超时，继续接收。不要清雿g_tModS.RxCount */
//	}

    testi=g_tModS.RxCount;
    testi=g_tModS.RxCount;
    testi=g_tModS.RxCount;
	if(testi>7)				/* ??????С?4???????? */
	{
		testi=testi;
	}
	testi=g_tModS.RxCount;
    if(testi==8)				/* ??????С?4???????? */
	{
		testi=testi+1;
	}
	//??????ˇ???
	if(OldTick!=Tick_10ms)
  	{  
	  OldTick=Tick_10ms;
	   if(g_mods_timeout>0)
      { 
	    g_mods_timeout--;
      }
	  if(g_mods_timeout==0 && g_tModS.RxCount>0)   //??????
      { 
		// goto err_ret;
	
      }
      else if(g_mods_timeout==0 && g_tModS.RxCount==0) //?????
         return;
      else //????ì???
         return;
	}
	else   //???10msì?????
		return;
	//g_mods_timeout = 0;	 					/* ??? */

	if (g_tModS.RxCount < 4)				/* ??????С?4???????? */
	{
		goto err_ret;
	}

	/* ??CRCУ?? */
// 	crc1 = CRC16(g_tModS.RxBuf, g_tModS.RxCount);
// 	if (crc1 != 0)
// 	{
// 		goto err_ret;
// 	}

// 	/* ??? (1??é */
// 	addr = g_tModS.RxBuf[0];				/* ?1?? ?? */
// 	if (addr != SADDR485)		 			/* ???????????ˇ??? */
// 	{
// 		goto err_ret;
// 	}

	/* 分析应用层协访*/
    if(g_tModS.RxBuf[2] == 0xA5)
    {
        UART_Action();
    }else{
//        usartocflag = 1;
        RecHandle();
    }
							
	
err_ret:
#if 0										/* 此部分为了串口打印结枿实际运用中可不要 */
	g_tPrint.Rxlen = g_tModS.RxCount;
	memcpy(g_tPrint.RxBuf, g_tModS.RxBuf, g_tModS.RxCount);
#endif
	
 	g_tModS.RxCount = 0;					/* 必须清零计数器，方便下次帧同歿*/
}

void TIM5_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); //ù?? TIM3 ??
    TIM_TimeBaseInitStructure.TIM_Period = arr; //??????
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //?????
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //в?????
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);// ú?????? TIM3
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //?????? 3 ?т??
    NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //??? 3 ??
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //????? 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //Ь???? 3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);// ü??? NVIC
    TIM_Cmd(TIM5,ENABLE); //Y????? 3
}

void TIM5_IRQHandler(void)
{
    
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //????
    {
        TIM_ClearITPendingBit(TIM5,TIM_IT_Update); //清除中断标志使
        Tick_10ms ++;
        MODS_Poll();
    }
}

void TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* TIM3 的配置 ---------------------------------------------------
   TIM3 输入时钟(TIM3CLK) 设置为 APB2 时钟 (PCLK2)    
    => TIM3CLK = PCLK2 = SystemCoreClock
   TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock 为48 MHz */
  /* TIM16 时钟使能 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
  /* Time 定时基础设置*/
  TIM_TimeBaseStructure.TIM_Prescaler = 2;//时钟预分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time 定时设置为上升沿计算模式*/
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//开启定时器更新中断
	TIM_SetAutoreload(TIM6, 0xFF);//设置PWM分辨率
	TIM6_NVIC_Config();
  /* TIM3 计算器使能*/
  TIM_Cmd(TIM6, ENABLE);
}
/***********************************************************************/
static void TIM6_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}