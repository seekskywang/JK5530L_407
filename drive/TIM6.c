/******************************************************************/
/* ���ƣHTIM3 PWM                                                */
/* Ч����                                                        */
/* ���ݣ�����һ��200HZ ��ռ�ձȣ�60.9% ��ռ�ձȣ�30.9%��PWM      */
/* ���ߣ�����                                                    */
/* ��ϵ��ʽ��QQ:363116119                                        */
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
	/* ����3.5���ַ�ʱ���ִ��MODH_RxTimeOut()������ȫ�ֱ��� g_rtu_timeout = 1; ֪ͨ������ʼ��m*/
//	if (g_mods_timeout == 0)	
//	{
//		return;								/* û�г�ʱ���������ա���Ҫ���g_tModS.RxCount */
//	}

    testi=g_tModS.RxCount;
    testi=g_tModS.RxCount;
    testi=g_tModS.RxCount;
	if(testi>7)				/* ??????��?4???????? */
	{
		testi=testi;
	}
	testi=g_tModS.RxCount;
    if(testi==8)				/* ??????��?4???????? */
	{
		testi=testi+1;
	}
	//??????��???
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
      else //????��???
         return;
	}
	else   //???10ms��?????
		return;
	//g_mods_timeout = 0;	 					/* ??? */

	if (g_tModS.RxCount < 4)				/* ??????��?4???????? */
	{
		goto err_ret;
	}

	/* ??CRC��?? */
// 	crc1 = CRC16(g_tModS.RxBuf, g_tModS.RxCount);
// 	if (crc1 != 0)
// 	{
// 		goto err_ret;
// 	}

// 	/* ??? (1??�� */
// 	addr = g_tModS.RxBuf[0];				/* ?1?? ?? */
// 	if (addr != SADDR485)		 			/* ???????????��??? */
// 	{
// 		goto err_ret;
// 	}

	/* ����Ӧ�ò�Э��*/
    if(g_tModS.RxBuf[2] == 0xA5)
    {
        UART_Action();
    }else{
//        usartocflag = 1;
        RecHandle();
    }
							
	
err_ret:
#if 0										/* �˲���Ϊ�˴��ڴ�ӡ�ᖨʵ�������пɲ�Ҫ */
	g_tPrint.Rxlen = g_tModS.RxCount;
	memcpy(g_tPrint.RxBuf, g_tModS.RxBuf, g_tModS.RxCount);
#endif
	
 	g_tModS.RxCount = 0;					/* ��������������������´�֡ͬ�{*/
}

void TIM5_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); //��?? TIM3 ??
    TIM_TimeBaseInitStructure.TIM_Period = arr; //??????
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //?????
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //��?????
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);// ��?????? TIM3
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //?????? 3 ?��??
    NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //??? 3 ??
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //????? 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //��???? 3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);// ��??? NVIC
    TIM_Cmd(TIM5,ENABLE); //Y????? 3
}

void TIM5_IRQHandler(void)
{
    
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //????
    {
        TIM_ClearITPendingBit(TIM5,TIM_IT_Update); //����жϱ�־ʹ
        Tick_10ms ++;
        MODS_Poll();
    }
}

void TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* TIM3 ������ ---------------------------------------------------
   TIM3 ����ʱ��(TIM3CLK) ����Ϊ APB2 ʱ�� (PCLK2)    
    => TIM3CLK = PCLK2 = SystemCoreClock
   TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock Ϊ48 MHz */
  /* TIM16 ʱ��ʹ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
  /* Time ��ʱ��������*/
  TIM_TimeBaseStructure.TIM_Prescaler = 2;//ʱ��Ԥ��Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time ��ʱ����Ϊ�����ؼ���ģʽ*/
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//������ʱ�������ж�
	TIM_SetAutoreload(TIM6, 0xFF);//����PWM�ֱ���
	TIM6_NVIC_Config();
  /* TIM3 ������ʹ��*/
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