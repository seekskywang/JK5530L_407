/******************************************************************/
/* 名称TIM2 PWM                                                */
/* 效果：                                                        */
/* 内容：产生一个200HZ 正占空比：60.9% 负占空比：30.9%的PWM      */
/* 作者：李振                                                    */
/* 联系方式：QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
/*****************************************************************/
extern struct bitDefine
{
	unsigned bit0: 1;
	unsigned bit1: 1;
	unsigned bit2: 1;
	unsigned bit3: 1;
	unsigned bit4: 1;
	unsigned bit5: 1;
	unsigned bit6: 1;
	unsigned bit7: 1;
} flagA,flagB,flagC,flagD;

vu32 abc_time;
vu32 ms_time;
vu32 shutcount1;
vu32 shutcount2;
vu32 shutcount3;
vu32 shutcount4;
vu32 recharge;
vu32 reboot;
vu8 shorttime;
vu8 shortflag;
float testv;
vu8 finishflag;
vu32 finishdelay;
vu8 lockflag;
vu8 version=20;
vu32 battery_c;
vu8 charge_step;
vu8 loop;
vu32 ctime;
vu32 dctime;
u8 sendmodepow[6] = {0x01,0x53,0x00,0x00,0x00,0x01};
u8 sendmodeload[6] = {0x01,0x53,0x00,0x00,0x00,0x02};
u8 sendmodestop[6] = {0x01,0x52,0x00,0x00,0x00,0x04};
u8 sendmodegap[6] = {0x01,0x53,0x00,0x00,0x00,0x03};
u16 cdcswdelay;
vu16 watchcurrent[1000];
u16 watchcount;
uint8_t vccbuf[13];
float shortold_I = 0;
vu8 overflag;
vu32 capwait;
/*****************************************************************/
void TIM4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//调用GPIO结构体
  /* 配置GPIO管脚复用*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//将设置输入函数
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//将设置输入函数
}
/*****************************************************************/
void TIM4_PWM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* TIM3 的配置 ---------------------------------------------------
   TIM3 输入时钟(TIM3CLK) 设置为 APB2 时钟 (PCLK2)    
    => TIM3CLK = PCLK2 = SystemCoreClock
   TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock 为48 MHz */
  /* TIM4 时钟使能 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM4_Config();

  /* Time 定时基础设置*/
  TIM_TimeBaseStructure.TIM_Prescaler = 6;//时钟预分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time 定时设置为上升沿计算模式*/
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

  /* 频道1的PWM 模式设置 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0x0f;//使能频道1配置
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_SetAutoreload(TIM4, 0x7530);//璁剧疆PWM鍒嗚鲸鐜?
	TIM_ARRPreloadConfig(TIM4, ENABLE);
  /* TIM4 浣胯兘*/
  TIM_Cmd(TIM4, ENABLE);
	
  /* TIM4 寮�濮嬭緭鍑?*/
  TIM_CtrlPWMOutputs(TIM4, ENABLE);	
}
/**************************************************************************************/
void Temp_Comapre(void)	  //温度来控制风扇
{
	if(NTC_value<=2650)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);//开启风扇
		flag_FAN_ON=1;
	}
	else if(NTC_value>=2700)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);//关闭风扇
		flag_FAN_ON=0;
	}
}

void SendToPC(u8 mode)
{
	
	
	vccbuf[0] = 0x01;
	vccbuf[2] = 0x0A;
	if(mode == 0)
	{
		vccbuf[1] = 0x50;
		
		vccbuf[3] = (vu16)(DISS_Voltage*1000)>>8;
		vccbuf[4] = (vu16)(DISS_Voltage*1000);
		
		vccbuf[5] = (vu16)(DISS_Current*1000)>>8;
		vccbuf[6] = (vu16)(DISS_Current*1000);
		
		vccbuf[7] = battery_c>>24;
		vccbuf[8] = battery_c>>16;
		vccbuf[9] = battery_c>>8;
		vccbuf[10] = battery_c;
		vccbuf[11] = Temperature>>8;
		vccbuf[12] = (vu8)Temperature;
	}else if(mode == 1){
		vccbuf[1] = 0x51;
		
		vccbuf[3] = (vu16)(Para.CPOW_Voltage*10)>>8;
		vccbuf[4] = (vu16)(Para.CPOW_Voltage*10);
		
		vccbuf[5] = (vu16)(Para.CCurrent*10)>>8;
		vccbuf[6] = (vu16)(Para.CCurrent*10);
		
		vccbuf[7] = battery_c>>24;
		vccbuf[8] = battery_c>>16;
		vccbuf[9] = battery_c>>8;
		vccbuf[10] = battery_c;
		vccbuf[11] = Temperature>>8;
		vccbuf[12] = (vu8)Temperature;
	}else if(mode == 2){
		vccbuf[1] = 0x52;
		
		
		if(mode_sw == 0)
		{
			vccbuf[3] = (vu16)(Para.CPOW_Voltage*10)>>8;
			vccbuf[4] = (vu16)(Para.CPOW_Voltage*10);
			vccbuf[5] = (vu16)(Para.CCurrent*10)>>8;
			vccbuf[6] = (vu16)(Para.CCurrent*10);
		}else if(mode_sw == 1){
			vccbuf[3] = (vu16)(DISS_Voltage*1000)>>8;
			vccbuf[4] = (vu16)(DISS_Voltage*1000);
			vccbuf[5] = (vu16)(DISS_Current*1000)>>8;
			vccbuf[6] = (vu16)(DISS_Current*1000);
		}else if(mode_sw == 2){
			vccbuf[5] = (vu16)(Para.CCurrent*10)>>8;
			vccbuf[6] = (vu16)(Para.CCurrent*10);
		}else if(mode_sw == 3){
			vccbuf[5] = (vu16)(DISS_Current*1000)>>8;
			vccbuf[6] = (vu16)(DISS_Current*1000);
		}
		
		vccbuf[7] = battery_c>>24;
		vccbuf[8] = battery_c>>16;
		vccbuf[9] = battery_c>>8;
		vccbuf[10] = battery_c;
		vccbuf[11] = Temperature>>8;
		vccbuf[12] = (vu8)Temperature;
	}
	MODS_SendWithCRC(vccbuf, 13);
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //使用定时器之前都必须开启定时器时钟，基本定时器属于APB1 总线外设。
    //开启Time x Clk， x
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
   
		/*  累计 TIM_Period 个后产生一个更新或者中断  */
    //  当定时器从0 计数到4999，即为5000 次，为一个定时周期
    TIM_TimeBaseInitStructure.TIM_Period = arr;  

    //定时器时钟源TIMxCLK = 2 * PCLK1
    // PCLK1 = HCLK / 4
    // => TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
    // 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz   
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 

    //计数方式
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 

		//采样时钟分频
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;

		//初始化定时器TIMx, x[1,8]
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	  //允许定时器x 更新中断
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 

		//定时器x 中断
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;   

		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; 

		// 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;   

		/* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

		// 使能定时器
    TIM_Cmd(TIM3,ENABLE); 
}

void TIM3_IRQHandler(void){
	
	float ftemp = 0;
	float step_c = 0;
	int step_time = 0;
	float LPower,PPower;
	u8 i;

	
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET) 
    {
		readpowflag ++;//读取电源数据计时
		
		if( ms_time > (1 << 31) - 2 )
			
			ms_time = 0;
		
		if( abc_time > (1 << 31) - 2 )
			
			abc_time = 0;
		if(finishdelay != 0)
		{
			finishdelay --;
		}
		
		switch(MODE){
			
			//程控负载
			case 0:{
				
				if(rmtrig[0] == 1)
				{
					if( ms_time%1000 == 0 && ms_time > 999 ){
						ftemp = DISS_Current * 1000 * 1/3600;
						CDC_CsumMah += ftemp ;
						battery_c = CDC_CsumMah;
						SendToPC(0);
					}
					
				}else{
					CDC_CsumMah = 0;
					battery_c = 0;
				}
				if(mainswitch == 1)
				{
					if(watchcount < 999)
					{
						watchcurrent[watchcount] = Vmon_Load_value;
						watchcount++;
					}
				}else{
					watchcount = 0;
				}
			}
			break;
			
			//程控电源
			case 1:{
				if(rmtrig[1] == 1)
				{
					if( ms_time%1000 == 0 && ms_time > 999 ){
	
						ftemp = DISS_POW_Current * 1000 * 1/3600;
						
						CDC_CsumMah += ftemp ;
						battery_c = CDC_CsumMah;
						SendToPC(1);
					}
					
					
				}else{
					CDC_CsumMah = 0;
					battery_c = 0;
				}
			}
			break;
			
			//过流
			case 2:{
				
				if( trig_flag_state == 1 && C_DISCHARGE == 1){		
//					finishflag = 0;
					step_c =  (float)Para.IR_Step_C / 1000 ;
					
					step_time = (int)Para.IR_Step_Time;
					
					if( step_time < 1 ){
						
						step_time = 1;
					}
					
													
					
					if( trig_flag_state != 0 )
					{						
						if( DISS_Current > old_I )
								
							old_I = DISS_Current;

						if( ms_time%step_time == 0)
						{
								
//							max_discahrgeNum += step_c;
//									
//							Para.CSET_Current_Laod = (int)(max_discahrgeNum * 1000);
							
							max_discahrgeNum = old_I;
							Para.CSET_Current_Laod += Para.IR_Step_C;
						}
						if( old_I != 0 ){
						
							if( old_I - DISS_Current > 0.002 /*DISS_Current < old_I*/ || DISS_Voltage < 0.1){
								max_discahrgeNum = old_I;
								g_tModS.TxBuf[13] = (int)(max_discahrgeNum*100)>>8;
								g_tModS.TxBuf[14] = (int)(max_discahrgeNum*100);
								trig_flag_state = 0;
								Off_GPOI_ResetSet();  
//								mainswitch = 0;
								if(Para.ACT_DELAY != 0)
								{
									recharge = 1000*Para.ACT_DELAY;
								}else{
									recharge = 1000;
								}
								Para.POWER_OutPut_V = (int)(testv*1000) + 3000;
								Para.POWER_Limit_C = 1000;
								mainswitch = 1;
								sendwait = 1;
//								OnOff_GPOI_ResetSet(0,1);
							}
						}else{
							if(DISS_Voltage < 0.1){
								max_discahrgeNum = old_I;
								Off_GPOI_ResetSet();
								g_tModS.TxBuf[13] = (int)(max_discahrgeNum*100)>>8;
								g_tModS.TxBuf[14] = (int)(max_discahrgeNum*100);
								trig_flag_state = 0;
	//							Off_GPOI_ResetSet();  
//								mainswitch = 0;
								if(Para.ACT_DELAY != 0)
								{
									recharge = 1000*Para.ACT_DELAY;
								}else{
									recharge = 1000;
								}
								Para.POWER_OutPut_V = (int)(testv*1000) + 3000;
								Para.POWER_Limit_C = 1000;
								mainswitch = 1;
								sendwait = 1;
							}
						}
					}								
				}
				if(recharge != 0)
				{
					recharge --;
					if(recharge == 0)
					{
						mainswitch = 0;
						sendwait = 3;
//						Off_GPOI_ResetSet();
						LOAD_MODE = 1;
						GPIO_SetBits(GPIOC,GPIO_Pin_12);//CV模式
						shortflag = 1;
						Para.CSET_Voltage_Laod = 0;
//						Para.CSET_Current_Laod = (int)(max_discahrgeNum*1000) + 3000;
						OnOff_GPOI_ResetSet(2,1);
					}
				}
				if(shortflag == 1)
				{
					shorttime ++;
//					if( DISS_Current > shortold_I )
//					{
//						shortold_I = DISS_Current;
//					}
//					if( shortold_I != 0 )
//					{
//						if( shortold_I - DISS_Current > 0.002 /*DISS_Current < old_I*//* || DISS_Voltage < 0.1*/){
//							shortflag = 0;
//							Off_GPOI_ResetSet();
//							g_tModS.TxBuf[15] = (shorttime)>>8;
//							g_tModS.TxBuf[16] = (shorttime);
//							MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);
//							if(Para.ACT_DELAY != 0)
//							{
//								reboot = 1000*Para.ACT_DELAY;
//							}else{
//								reboot = 1000;
//							}
//	//						reboot = 5000;
//							Para.POWER_OutPut_V = (int)(testv*1000) + 3000;
//							Para.POWER_Limit_C = 1000;
//							mainswitch = 1;
//							sendwait = 1;
//							shortold_I = 0;
//						}
//					}
					if(DISS_Voltage/testv < 0.4 || shorttime==255)
					{
						shortflag = 0;
						Off_GPOI_ResetSet();
						g_tModS.TxBuf[15] = (shorttime)>>8;
						g_tModS.TxBuf[16] = (shorttime);
						MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);
						if(Para.ACT_DELAY != 0)
						{
							reboot = 1000*Para.ACT_DELAY;
						}else{
							reboot = 1000;
						}
//						reboot = 5000;
						Para.POWER_OutPut_V = (int)(testv*1000) + 3000;
						Para.POWER_Limit_C = 1000;
						mainswitch = 1;
						sendwait = 1;
					}
				}
				if(reboot != 0)
				{
					reboot --;
					if(reboot == 0)
					{
						mainswitch = 0;
						sendwait = 3;
//						Off_GPOI_ResetSet();
						Para.CSET_Current_Laod =  Para.IR_Start_C;
						LOAD_MODE = 0;
						GPIO_ResetBits(GPIOC,GPIO_Pin_12);//CC模式
						finishflag = 1;
						mainswitch = 0;
						if(Para.ACT_DELAY != 0)
						{
							finishdelay = 1000*Para.ACT_DELAY;
						}else{
							finishdelay = 1000;
						}
//						finishdelay = 5000;
					}
				}
			}
			break;
			
//			//程控负载	
//			case 2:{
//				
//				
//			}
//			break;
			
			//充放电
			case 3:{
				
				
			}
			break;
			//曲线图
			case 4:{
				
				
			}
			break;
			//充放电
			case 5:{
				if(rmtrig[2] == 1)
				{
					if( ms_time%1000 == 0 && ms_time > 999 ){
				
						if( mode_sw == 1 ){
							dctime++;	
							ftemp = DISS_Current * 1000 * 1/3600;
							CDC_DCsumMah += ftemp;	 
							battery_c = CDC_DCsumMah;
						}else if( mode_sw == 0 ){
							ctime++;
//							DISS_POW_Current=(float)Para.CCurrent/100;	
							ftemp = DISS_POW_Current * 1000 * 1/3600;
							CDC_CsumMah += ftemp;
							battery_c = CDC_CsumMah;
						}else if(mode_sw == 2){
							battery_c = CDC_CsumMah;
						}else if(mode_sw == 3){
							battery_c = CDC_DCsumMah;
						}
						SendToPC(2);
						
						
					}
					if(mode_sw == 0)
					{
						if(sendmodeflag == 1)
						{
							for(i=0;i<3;i++)
							{
								MODS_SendWithCRC(sendmodepow,6);
								Delay_ms(SENDPCDELAY);
							}
							sendmodeflag = 0;
						}
						if(cdcswdelay != 0)
						{
							cdcswdelay --;
						}
						if(Para.CaPCTIME == 0)
						{
//							if(charge_step == 1)
//							{
//								if(Para.CPOW_Voltage*10 >= Para.CDC_Ccutoff_V && Para.CDC_Ccutoff_V != 0 && cdcswdelay == 0)
//								{
//									Para.CSET_Voltage = Para.CDC_Ccutoff_V;
//									charge_step = 2;
//									cdcswdelay = 5000;
//								}
//							}else if(charge_step == 2){
								if(Para.CCurrent*10 < Para.CDC_Ccutoff_C && Para.CDC_Ccutoff_C != 0 && cdcswdelay == 0)
								{
									charge_step = 1;
	//								GPIO_ResetBits(GPIOC,GPIO_Pin_1); //关闭电源输出
	//								Para.CSET_Voltage = 0;
	//								Para.CSET_Current = 0;
	//								GPIO_ResetBits(GPIOE,GPIO_Pin_2); //关闭电源输出继电器
									mainswitch = 0;
									ctime=0;
									listsend = 1;
									mode_sw = 2;
//									mode_sw = 1;
									sendmodeflag = 1;
									capwait = Para.CDC_Gap_Time*1000;
//									cdcswdelay = 5000;
								}
//							}
						}else{
							if(ctime >= Para.CaPCTIME && cdcswdelay == 0)
							{
								charge_step = 1;
//								GPIO_ResetBits(GPIOC,GPIO_Pin_1); //关闭电源输出
//								Para.CSET_Voltage = 0;
//								Para.CSET_Current = 0;
//								GPIO_ResetBits(GPIOE,GPIO_Pin_2); //关闭电源输出继电器
								mainswitch = 0;
								ctime=0;
								listsend = 1;
								mode_sw = 2;
//									mode_sw = 1;
								sendmodeflag = 1;
								capwait = Para.CDC_Gap_Time*1000;
//									cdcswdelay = 5000;
							}
						}
					}else if(mode_sw == 1){//放电
						if(sendmodeflag == 1)
						{
							for(i=0;i<3;i++)
							{
								MODS_SendWithCRC(sendmodeload,6);
								Delay_ms(SENDPCDELAY);
							}
							sendmodeflag = 0;
							Delay_ms(100);
							Flag_Swtich_ON = 1;
							GPIO_ResetBits(GPIOC,GPIO_Pin_7);//On
						}
						if(cdcswdelay != 0)
						{
							cdcswdelay --;
						}
						if(Para.CaPDCTIME == 0)
						{
							if(Para.CVoltage < Para.CDC_Dcutoff_V && cdcswdelay == 0)
							{
								Flag_Swtich_ON = 0;
								GPIO_SetBits(GPIOC,GPIO_Pin_7);//OFF
								if(loop == Para.CDC_Cycle_Time)
								{
									loop = 1;
									battery_c = battery_c/Para.CDC_Cycle_Time;
									for(i=0;i<3;i++)
									{
										if(rmtrig[2] == 1)
										{
	//										SendToPC(2);
										}
										Delay_ms(SENDPCDELAY);
										MODS_SendWithCRC(sendmodestop,6);
										Delay_ms(SENDPCDELAY);
									}
									rmtrig[2] = 0;
									CDC_CsumMah = 0;
									CDC_DCsumMah = 0;
									dctime = 0;
								}else{
	//								CDC_CsumMah = 0;
	//								CDC_DCsumMah = 0;
									Flag_Swtich_ON = 0;
									GPIO_SetBits(GPIOC,GPIO_Pin_7);//OFF
									Delay_ms(100);
									dctime = 0;
									mode_sw = 3;
									sendmodeflag = 1;
									capwait = Para.CDC_Gap_Time*1000;
//									mode_sw = 0;
	//								GPIO_SetBits(GPIOE,GPIO_Pin_2);   //打开电源输出继电器
	//								Para.CSET_Voltage = Para.CDC_OutPut_V;
	//								Para.CSET_Current = Para.CDC_Limit_C;
	//								GPIO_SetBits(GPIOC,GPIO_Pin_1);   //打开电源输出
//									mainswitch = 1;
//									listsend = 1;
//									sendmodeflag = 1;
//									charge_step = 1;
//									cdcswdelay = 5000;
//									loop ++;	
								}
							}
						}else{
							if(dctime >= Para.CaPDCTIME && cdcswdelay == 0)
							{
								Flag_Swtich_ON = 0;
								GPIO_SetBits(GPIOC,GPIO_Pin_7);//OFF
								if(loop == Para.CDC_Cycle_Time)
								{
									loop = 1;
									battery_c = battery_c/Para.CDC_Cycle_Time;
									for(i=0;i<3;i++)
									{
										if(rmtrig[2] == 1)
										{
	//										SendToPC(2);
										}
										Delay_ms(SENDPCDELAY);
										MODS_SendWithCRC(sendmodestop,6);
										Delay_ms(SENDPCDELAY);
									}
									rmtrig[2] = 0;
									CDC_CsumMah = 0;
									CDC_DCsumMah = 0;
									dctime = 0;
								}else{
	//								CDC_CsumMah = 0;
	//								CDC_DCsumMah = 0;
									Flag_Swtich_ON = 0;
									GPIO_SetBits(GPIOC,GPIO_Pin_7);//OFF
									Delay_ms(100);
									dctime = 0;
									mode_sw = 3;
									sendmodeflag = 1;
									capwait = Para.CDC_Gap_Time*1000;
//									mode_sw = 0;
	//								GPIO_SetBits(GPIOE,GPIO_Pin_2);   //打开电源输出继电器
	//								Para.CSET_Voltage = Para.CDC_OutPut_V;
	//								Para.CSET_Current = Para.CDC_Limit_C;
	//								GPIO_SetBits(GPIOC,GPIO_Pin_1);   //打开电源输出
//									mainswitch = 1;
//									listsend = 1;
//									sendmodeflag = 1;
//									charge_step = 1;
//									cdcswdelay = 5000;
//									loop ++;	
								}
							}
						}
					}else if(mode_sw == 2){//充电搁置
						if(sendmodeflag == 1)
						{
							for(i=0;i<3;i++)
							{
								MODS_SendWithCRC(sendmodegap,6);
								Delay_ms(SENDPCDELAY);
								
							}
							sendmodeflag = 0;
						}
						if(capwait == 0)
						{
							mode_sw = 1;
							sendmodeflag = 1;
							cdcswdelay = 5000;
						}else{
							capwait --;
						}
					}else if(mode_sw == 3){//放电搁置
						if(sendmodeflag == 1)
						{
							for(i=0;i<3;i++)
							{
								MODS_SendWithCRC(sendmodegap,6);
								Delay_ms(SENDPCDELAY);
								
							}
							sendmodeflag = 0;
						}
						if(capwait == 0)
						{
							mode_sw = 0;
							mainswitch = 1;
							listsend = 1;
							sendmodeflag = 1;
							charge_step = 1;
							charge_step = 1;
							cdcswdelay = 5000;
							loop ++;
						}else{
							capwait --;
						}
					}
				}
			}	
			break;
			
			//系统信息
			case 6:{
				
				
			}  	  
			break;
		}
		
		if( ms_time % 1000 == 0 && ms_time > 999 )
			
			abc_time++; 
	
		ms_time++;
		if(calflag == 0)
		{
			PPower = DISS_POW_Voltage * DISS_POW_Current;
			LPower = DISS_Voltage * DISS_Current;
		}else{
			PPower=0;
			LPower=0;
		}
//		if((PPower > 80) || (LPower > 80))
//		{
//			Off_GPOI_ResetSet();
//			shutcount1 = 0;
//			shutcount2 = 0;
//			mainswitch = 0;
//		}
//		if((PPower >= 300) || (LPower >= 600))
//		{
//			shutcount3++;
//			if(shutcount3 >= 500)
//			{
//				Off_GPOI_ResetSet();
//				shutcount1 = 0;
//				shutcount2 = 0;
//				shutcount3 = 0;
//				mainswitch = 0;
//			}
//		}else if((PPower > 100 && PPower < 200) || (LPower > 500 && LPower < 600)){
//			shutcount1 ++;
//			if(shutcount1 >= 180000)
//			{
//				Off_GPOI_ResetSet();
//				mainswitch = 0;
//				shutcount1 = 0;
//			}			
//		}else if((PPower >= 200 && PPower < 300) || (LPower >= 500 && LPower < 600)){
//			shutcount2 ++;
//			if(shutcount2 >= 10000)
//			{
//				Off_GPOI_ResetSet();
//				mainswitch = 0;
//				shutcount2 = 0;
//			}
//		}else{
//			shutcount1 = 0;
//			shutcount2 = 0;
//			shutcount3 = 0;
//		}
//		if(HARDVER == 1)
//		{
//			if(MODE != 2)
//			{
//				if((PPower >= 600) || (LPower >= 1800) || (DISS_Current >= 100))
//				{
//					shutcount3++;
//					if(shutcount3 >= 500)
//					{
//						overflag = 1;
//						Off_GPOI_ResetSet();
//	//					shutcount1 = 0;
//	//					shutcount2 = 0;
//						shutcount3 = 0;
//						mainswitch = 0;
//					}
//				}else{
//	//				shutcount1 = 0;
//	//				shutcount2 = 0;
//					shutcount3 = 0;
//				}
//				if(DISS_Current > 60 && DISS_Current < 100)
//				{
//					shutcount4 ++;
//					if(shutcount4 >= 20000)
//					{
//						overflag = 1;
//						Off_GPOI_ResetSet();
//						mainswitch = 0;
//						shutcount1 = 0;
//					}
//				}else{
//					shutcount4 = 0;
//				}
//			}else{
//				if((PPower >= 600) || (LPower >= 3600) || DISS_Current >= 160)
//				{
//					shutcount3++;
//					if(shutcount3 >= 500)
//					{
//						overflag = 1;
//						Off_GPOI_ResetSet();
//	//					shutcount1 = 0;
//	//					shutcount2 = 0;
//						shutcount3 = 0;
//						mainswitch = 0;
//					}
//				}else{
//	//				shutcount1 = 0;
//	//				shutcount2 = 0;
//					shutcount3 = 0;
//				}
//				if(DISS_Current > 60 && DISS_Current < 160)
//				{
//					shutcount4 ++;
//					if(shutcount4 >= 2000)
//					{
//						overflag = 1;
//						Off_GPOI_ResetSet();
//						mainswitch = 0;
//						shutcount4 = 0;
//					}
//				}else{
//					shutcount4 = 0;
//	//				shutcount2 = 0;
//	//				shutcount3 = 0;
//				}
//			}
//		}else if(HARDVER == 2){
			if(MODE != 2)
			{

				if((PPower >= 1200) || (LPower >= 1800) || (DISS_Current >= 120))
				{
					shutcount3++;
					shutcount2++;
					if(shutcount3 >= 500)
					{
						overflag = 1;
						Off_GPOI_ResetSet();
	//					shutcount1 = 0;
	//					shutcount2 = 0;
						shutcount3 = 0;
						shutcount2 = 0;
						mainswitch = 0;
					}
				}else{
	//				shutcount1 = 0;
	//				shutcount2 = 0;
						shutcount3 = 0;
				}
				if(DISS_Current >= 100 && DISS_Current < 120)
				{
					shutcount2 ++;
					if(shutcount2 >= 20000)
					{
						overflag = 1;
						Off_GPOI_ResetSet();
						mainswitch = 0;
						shutcount2 = 0;
					}
				}else{
					if(DISS_Current<100)
						shutcount2 = 0;
				}
			}else{
				if((PPower >= 1200) || (LPower >= 3600) || DISS_Current >= 160)
				{
					shutcount2++;
					shutcount4 ++;
					shutcount3++;
					if(shutcount3 >= 500)
					{
						overflag = 1;
						Off_GPOI_ResetSet();
	//					shutcount1 = 0;
						shutcount2 = 0;
						shutcount3 = 0;
						shutcount4  =0;
						mainswitch = 0;
					}
				}else{
	//				shutcount1 = 0;
	//				shutcount2 = 0;
						shutcount3 = 0;
				}
				if(DISS_Current > 100 && DISS_Current < 120)
				{
					shutcount2 ++;
					if(shutcount2 >= 20000)
					{
						overflag = 1;
						Off_GPOI_ResetSet();
						mainswitch = 0;
						shutcount2 = 0;
					}
				}else{
					if(DISS_Current<100)
						shutcount2 = 0;
				}
				if(DISS_Current >= 120 && DISS_Current < 160)
				{
					shutcount2++;
					shutcount4 ++;
					if(shutcount4 >= 2000)
					{
						overflag = 1;
						Off_GPOI_ResetSet();
						mainswitch = 0;
						shutcount4 = 0;
						shutcount2 = 0;
					}
				}else{
					if(DISS_Current<120)
						shutcount4 = 0;
	//				shutcount2 = 0;
	//				shutcount3 = 0;
				}
			}
//		}
//		else if((PPower > 100 && PPower < 200) || (LPower > 100 && LPower < 200)){
//			shutcount1 ++;
//			if(shutcount1 >= 180000)
//			{
//				Off_GPOI_ResetSet();
//				mainswitch = 0;
//				shutcount1 = 0;
//			}			
//		}else if((PPower >= 200 && PPower < 300) || (LPower >= 200 && LPower < 300)){
//			shutcount2 ++;
//			if(shutcount2 >= 10000)
//			{
//				Off_GPOI_ResetSet();
//				mainswitch = 0;
//				shutcount2 = 0;
//			}
//		}
		
		
//		if((PPower > 80) || (LPower > 80))
//		{
//			Off_GPOI_ResetSet();
//			shutcount1 = 0;
//			shutcount2 = 0;
//			mainswitch = 0;
//		}
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
	  
}




