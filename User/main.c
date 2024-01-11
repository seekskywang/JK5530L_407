/******************************************************************/
/* 名称：main                                                    */
/* 效果：                                                        */
/* 内容：                                                       */
/* 作者：zhan                                                  */
/* 联系方式QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx.h"
#include "clock.h"
#include "tim6.h"
#include "tim1.h"
#include "tim4.h"
#include "adc.h"
#include "clock.h"
#include "gpio.h"
#include "bsp_SysTick.h"
#include "usart.h"
#include "ssd1963.h"
#include "iwdg.h"
#include "MainTask.h"
#include "IIC_24C01.h"
#include "flash.h"
#include "tm1650.h"
#include "ad5541.h"
#include "ad7689.h"
#include "usart.h"
#include "AT24C01.h"

struct bitDefine
{
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
} flagA, flagB,flagC,flagD,flagE,flagF,flagG;
CorrectList CLists;
CAL CalPara;
SetPara Para;
vu8 bootflag;
vu8 POWER_RECHARGE;   	//充电  1  --  正在充电   0  --  未充电
vu8 C_DISCHARGE;      	//放电  1  --  正在放电   0  -- 
vu8 trig_flag_state = 0;     //trig按键状态
vu8 calflag;//校准标志位
vu32 readpowflag;
float max_discahrgeNum = 0;  
float DISS_Voltage;
float DISS_POW_Voltage;
float DISS_Current;
float DISS_POW_Current;
float DISS_R;
float old_I = 0;

float CDC_CsumMah;         //每秒累加的容量
float CDC_DCsumMah;         //每秒累加的容量
short Temperature=0;
u8 tempflag;
u8 jumpflag = 0;
//根据页面
void OnOff_GPOI_ResetSet( vu8 _type, vu8 _value ){
	
	switch( _type ){
		
//		case 1024:   
//			if( _value == 0 ){
//					
//				flag_Load_CC = 1;//CC模式
//				GPIO_ResetBits(GPIOC,GPIO_Pin_12);   //CC		
//			}
//			else if( _value == 1 ){
//					
//				flag_Load_CC=0;//CV模式
//				GPIO_SetBits(GPIOC,GPIO_Pin_12);   //CV
//			}
//		break;
		
		case 2:  //电子负载ON/OFF
			if( _value == 0 ){
				
				Flag_Swtich_ON = 0;
				GPIO_SetBits(GPIOC,GPIO_Pin_7);//OFF	  	
			}
			else if( _value == 1 ){
				
				Flag_Swtich_ON = 1;
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);//On
			}
		break;
		
		case 0:  //稳压电源ON/OFF
		
//			if( _value == 0 ){
//				
//				GPIO_ResetBits(GPIOC,GPIO_Pin_1); //关闭电源输出+
//				Para.CSET_Voltage = 0;
//				Para.CSET_Current = 0;
//				GPIO_ResetBits(GPIOE,GPIO_Pin_2); //关闭电源输出继电器
//			}
//			else if( _value == 1 ){
//				
//				GPIO_SetBits(GPIOE,GPIO_Pin_2);   //打开电源输出继电器
//				GPIO_SetBits(GPIOC,GPIO_Pin_1);   //打开电源输出
//			}
		break;
		
	}
	
}

void delay(vu32 i)
{
	do
	{
	__NOP();				/*????,??????,??????,?????intrins.h?,????????????.h??*/
	__NOP();
	__NOP();
	i--;
	}while(i*28);
}

//复位DS18B20
void Ds18b20Rst(void)	   
{                 
    DS18B20_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT_0; //拉低DQ
    Delay_usS(1250);    //拉低750us
    DS18B20_DQ_OUT_1; //DQ=1 
    Delay_usS(38);     //15US
}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 Ds18b20Check(void) 	   
{   
    u8 retry=0;
    DS18B20_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
    {
        retry++;
        Delay_usS(3);
    };	 
    if(retry>=200)
		return 1;
    else 
		retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
    {
        retry++;
        Delay_usS(3);
    };
    if(retry>=240)
		return 1;	    
    return 0;
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 Ds18b20Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
   GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//DS18B20
	GPIO_Init(GPIOB, &GPIO_InitStructure);//
    
    GPIO_SetBits(GPIOB,GPIO_Pin_10);    //输出1
    
    Ds18b20Rst();
    
    return Ds18b20Check();
}  

//从DS18B20读取一个位
//返回值：1/0
u8 Ds18b20ReadBit(void) 			 // read one bit
{
    u8 data;
    DS18B20_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_OUT_0; 
    Delay_usS(5);
    DS18B20_DQ_OUT_1; 
    DS18B20_IN();//SET PA0 INPUT
    Delay_usS(30);
    if(DS18B20_DQ_IN)
		data=1;
    else 
		data=0;	 
    Delay_usS(125);           
    return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
u8 Ds18b20ReadByte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
    for (i=1;i<=8;i++) 
    {
        j=Ds18b20ReadBit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
void Ds18b20WriteByte(u8 dat)     
{             
    u8 j;
    u8 testb;
    DS18B20_OUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
    {
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT_0;// Write 1
            Delay_usS(5);                            
            DS18B20_DQ_OUT_1;
            Delay_usS(150);             
        }
        else 
        {
            DS18B20_DQ_OUT_0;// Write 0
            Delay_usS(150);             
            DS18B20_DQ_OUT_1;
            Delay_usS(5);                          
        }
    }
}


void Ds18b20Start(void)// ds1820 start convert
{   						               
    Ds18b20Rst();	   
    Ds18b20Check();	 
    Ds18b20WriteByte(0xcc);// skip rom
    Ds18b20WriteByte(0x44);// convert
} 

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short Ds18b20GetTemp(void)
{
    u8 temp;
    u8 TL,TH;
    short tem;
    Ds18b20Start ();                    // ds1820 start convert
    Ds18b20Rst();
    Ds18b20Check();	 
    Ds18b20WriteByte(0xcc);// skip rom
    Ds18b20WriteByte(0xbe);// convert	    
    TL=Ds18b20ReadByte(); // LSB   
    TH=Ds18b20ReadByte(); // MSB  
    
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(short)((float)tem*0.625);//转换     
    if(temp)return tem; //返回温度值
    else return -tem;    
} 

//关闭所有 -- 使用的
void Off_GPOI_ResetSet(){
	
	POWER_RECHARGE = 0;						
	C_DISCHARGE = 0;							
	OnOff_GPOI_ResetSet( 0,	POWER_RECHARGE );
	OnOff_GPOI_ResetSet( 2, C_DISCHARGE );
}

void PLimit(void)
{
	
}

void JumpBoot(u8 flag)
{
  	void (*pUserApp)(void);
  uint32_t JumpAddress;
	if(flag==55)
  {		
	__asm("CPSID  I");
        
		JumpAddress = *(volatile uint32_t*) (0x08000000+4);
		pUserApp = (void (*)(void)) JumpAddress;
		TIM_Cmd(TIM1, DISABLE);	
		TIM_DeInit(TIM1);
		TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM2, DISABLE);	
		TIM_DeInit(TIM2);
		TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM3, DISABLE);	
		TIM_DeInit(TIM3);
		TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM4,DISABLE);
		TIM_DeInit(TIM4);
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
	    TIM_Cmd(TIM5,DISABLE);
		TIM_DeInit(TIM5);
		TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM6, DISABLE);	
	    TIM_DeInit(TIM6);
		TIM_ITConfig(TIM6,TIM_IT_Update,DISABLE);
		USART_DeInit(USART1);
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		
		USART_Cmd(USART1,DISABLE);
		USART_DeInit(USART2);
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);		
		USART_Cmd(USART2,DISABLE);
	    USART_DeInit(USART3);
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);		
		USART_Cmd(USART3,DISABLE);
		RCC_DeInit();
		RCC_RTCCLKCmd(DISABLE);
		EXTI_DeInit();
		SysTick->CTRL = 0;
		RTC_DeInit();
		RTC_ITConfig(RTC_IT_WUT,DISABLE);//关闭WAKE UP 定时器中断
		RTC_WakeUpCmd( DISABLE);//关闭WAKE UP 定时器　
		__disable_irq();
		NVIC_DisableIRQ(OTG_FS_IRQn);
		NVIC_DisableIRQ(OTG_FS_WKUP_IRQn);
		NVIC_DisableIRQ(OTG_HS_IRQn);
		NVIC_DisableIRQ(OTG_HS_WKUP_IRQn);
		ADC_DeInit();
		DMA_Cmd(DMA2_Stream0, DISABLE);
		DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,DISABLE);
		ADC_DMACmd(ADC1, DISABLE);  //使能DMA
		__ASM volatile ("cpsid i");
		/* Initialize user application's Stack Pointer */
		__set_PSP(*(volatile uint32_t*) 0x08000000);
		__set_CONTROL(0);
		__set_MSP(*(volatile uint32_t*) 0x08000000);
		
        
		
//		NVIC_SystemReset();
		pUserApp();
	}
}


int main(void)
{
		u8 i;
	 int t=0;
	u8 ret;
	u16 len;	
	u8 datatemp[30];
	u8 len1 =0;
		
 	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x20000);
	__enable_irq();
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);//开启CRC时钟，STEMWIN授权使用
	RCC_Configuration();
	SysTick_Init();
	GPIO_Conf();

    abc_time = 0;
    ms_time = 0;

	TIM1_PWM_Config();
	TIM4_PWM_Config();
	TIM3_Int_Init(10-1,8400-1);
//	TIM3_Int_Init(1,8400-1);
	TIM5_Int_Init(90-1,8400-1);
	TIM6_Config();
	ADC1_DMA_Init();
	USART_Configuration();
	USART2_Configuration();
	USART3_Configuration();
	i2c_CfgGpio();//24C01引脚初始化
	TM1650_GPIO_INT();		/*TM1650引脚初始化 */
	AD5541_GPIOCoing();//DAC初始化
	AD7689_InitializeSPI1();//AD7689初始化
//	LCD_Initializtion();//液晶屏初始化
	//EEPROM_READ_Coeff();//读取校准参数
	RCONT_SW = 0;
	Read_Flash_Bytes();
	Read_Boot_Flag();
	Off_GPOI_ResetSet();
	LOAD_MODE = 0;//负载CC模式
	LOAD_V_SW = 0;
	LOAD_I_SW = 0;
	Flag_Swtich_ON = 0;
	flag_CC_MODE = 1;
	Temperature = Ds18b20Init();
	while(1)
	{
		if(tempflag == 20)
		{
			tempflag = 0;
			Temperature = Ds18b20GetTemp();
		}else{
			tempflag ++;
		}
		if(readpowflag > 200 && sendwait == 0 && listsend == 0/* && mainswitch == 1*/)
		{
			ReadPowData();
			readpowflag = 0;
		}
//		PLimit();//功率限制保护
		if(USART_RX_STA&0x8000)	   //判断是否接收完数据
		{					   

//			len1=USART_RX_STA&0x3f;
			len1=USART_RX_CNT;
			len = SerialRemoteHandleL(len1,USART_RX_BUF);
			len1 =0;
			for(t=0;t<len;t++)								  //通过长度进行循环
			{
				USART_SendData(USART2, USART_RX_BUF[t]);      //将数组采集好的数据 一次向串口1发送
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
			}
			memset(USART_RX_BUF,0,USART_REC_LEN);
			
			USART_RX_STA=0;			  //清零 虚拟寄存器		  为一下次采集做准备
		}
		if(jumpflag == 1)
		{
			JumpBoot(55);
		}
		if(USART3_Recive_flg == 1)
		{
			USART3HANDLE();
		}
//		TIM_SetCompare3(TIM4,Contr_Voltage);//稳压电源电流DAC
//		TIM_SetCompare4(TIM4,Contr_Current);//稳压电源电压DAC
		AD5541_Send(Contr_Laod);//加载DAC值
		MODE_CONT();//电压电流档位切换
		if(Flag_DAC_OFF==0 && calflag == 0)
		{
			Transformation_ADC();
		}
		if(Flag_ADC_Full==1)
		{
			Flag_ADC_Full=0;
			ADC_CH_Scan();//ADC滤波
		}
		AD7689_Scan_CH();//读取AD7689各通道AD值
		Temp_Comapre();//风扇控制
	}
	//IWDG_Inte();
//	MainTask();
}


/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
