/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * 文件名  :MODBUS.C
 * 作者   :
 * 描述    :MODBUS?????
 * 内容    :KL220A_Master
 * 硬件连接: 485
 * 修改日期:2014-12-22
********************************************************************/
#include "my_register.h" 
#include "usart.h" 
#include "modbus.h" 
#include "flash.h"
#include "stm32f4xx_gpio.h"

//===================================================================//
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
} flagA,flagB,flagC,flagD,flagE,flagG;
/*************************校准参数************************************/
vu16 ADJ_Write;//校准时需写入的实际值
vu8 DAC_Flag;//DAC是否加载标志
vu32 Modify_A_READ;
vu32 Modify_C_READ;
vu32 Modify_A_ACT;
	
vu32 Modify_B_READ;
vu32 Modify_D_READ;
vu32 Modify_B_ACT;
vu32 Correct_Parametet[17];//校准参数
vu32 Correct_Strong[17];//校准系数
vu8  correct_por[8];
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count);
/*************************变量定义***********************************/
vu32 Run_Control[17];
vu8 ADDR;
//============================函数声明===============================//
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count);
//===========================MODBUS协议=============================//
void UART_Action(void)
{//RUT格式：
	//ADDR  命令码  读寄存器的起始地址高   读寄存器的起始地址低  读数据字个数高字节   读数据个数低字节  CRC高 CRC低
	//返回格式：ADDR 命令码 返回数据字节数  数据高  数据低 ..... CRC高  CRC低
	if (UART_Buffer_Rece[0] == ADDR)
	{
		if (UART_Buffer_Rece[1] == (0x03))	//命令3 读数据   
		{																		 
			vu8 i;
			vu16 crc_result;
			crc_result = (UART_Buffer_Rece[6] << 8) + UART_Buffer_Rece[7];
			if ((crc_result == Hardware_CRC(UART_Buffer_Rece,6)) ||(crc_result == 0) )
			{
				if (UART_Buffer_Rece[3] < 0x07)    								//如果寄存器在可读范围内
				{
					if ((UART_Buffer_Rece[3] + UART_Buffer_Rece[5]) < 0x0F)		//如果最后一个读取的寄存器地址在可读范围内
					{							
						UART_Buffer_Send[0] = ADDR;
						UART_Buffer_Send[1] = 0x03;
						UART_Buffer_Send[2] = UART_Buffer_Rece[5]*2;
						for (i=0;i<UART_Buffer_Send[2];i++)
						{
							if ((i % 2) == 0) UART_Buffer_Send[3 + i] = Run_Control[UART_Buffer_Rece[3] + i / 2] >> 8;
							else UART_Buffer_Send[3 + i] = Run_Control[UART_Buffer_Rece[3] + i / 2];														
						}
						crc_result = Hardware_CRC(UART_Buffer_Send,UART_Buffer_Send[2] + 3);
						UART_Buffer_Send[3 + UART_Buffer_Send[2]] = crc_result >> 8;
						UART_Buffer_Send[4 + UART_Buffer_Send[2]] = crc_result;
						Transmit_BUFFERsize = UART_Buffer_Send[2] + 5;
						UART_SEND_flag=1;
					}
				}
			}	
		}
	} 
//===============================写寄存器=================================
	if ((UART_Buffer_Rece[0] == 0) ||  (UART_Buffer_Rece[0] == ADDR) || (UART_Buffer_Rece[0] == ((ADDR-1)/4+100)))	 
	{
		vu8 var8;
		vu8 a=0;
		vu16 var16;
		vu16 crc_result;
//=========================以下命令6 写单个寄存器===========================
		if (UART_Buffer_Rece[1] == 6)                                 //判断第二个字节是否为命令6
		{
			if (UART_Buffer_Rece[3] < 0x05)							  //判断需要写的地址是否在可写范围内
			{
				crc_result = (UART_Buffer_Rece[6] << 8) + UART_Buffer_Rece[7];
				if ((crc_result == Hardware_CRC(UART_Buffer_Rece,6)) ||(crc_result == 0) )		  //检查CRC
				{
					var16 = (UART_Buffer_Rece[4] << 8) + UART_Buffer_Rece[5];	//第5 6个字节为要写入的数据
					var8 = UART_Buffer_Rece[3];	        						//第3 4个字节为要写入的地址
					Run_Control[var8] = var16;			    //将数据写入指定的地址

					if (UART_Buffer_Rece[0] == ADDR)							//广播模式下不返回数据
					{
						for (a=0;a<8;a++)
						{UART_Buffer_Send[a] = UART_Buffer_Rece[a];}
						Transmit_BUFFERsize = 8;						//原样数据返回，不计算CRC
						UART_SEND_flag=1;
					}
				}
			}
		}
//=======================================以下是命令16，连写寄存器===========================================
//功能码16格式:
//     地址 命令 写入起始地址高  写入起始地址低 写入字数高  写入字数低 写入字节数  数据高 数据低 ......CRC高 CRC低
//返回数据格式:
//     地址 命令 写入起始地址高  写入起始地址低  写入字节数高 写入字节数低  CRC高  CRC低 
		if (UART_Buffer_Rece[1] == 16)										  
		{	
			if ((UART_Buffer_Rece[6] == 6) && (UART_Buffer_Rece[3] == 0x00))	//规定连写寄存器的个数
			{
				crc_result = (UART_Buffer_Rece[13] << 8) + UART_Buffer_Rece[14];
				if ((crc_result == Hardware_CRC(UART_Buffer_Rece,13)) ||(crc_result == 0) )	   //检查CRC
				{												
					for (var8=0;var8<3;var8++) Run_Control[var8] = (UART_Buffer_Rece[var8*2+7] << 8) + UART_Buffer_Rece[var8*2+8];

					if (UART_Buffer_Rece[0] == ADDR)					  //广播模式不返回数据
					{
						UART_Buffer_Send[0] = ADDR;
						UART_Buffer_Send[1] = 16;
						UART_Buffer_Send[2] = UART_Buffer_Rece[2];
						UART_Buffer_Send[3] = UART_Buffer_Rece[3];
						UART_Buffer_Send[4] = UART_Buffer_Rece[4];
						UART_Buffer_Send[5] = UART_Buffer_Rece[5];
						crc_result = Hardware_CRC(UART_Buffer_Send,6);	 //计算CRC码
						UART_Buffer_Send[6] = crc_result>>8;
						UART_Buffer_Send[7] = crc_result;				 
						Transmit_BUFFERsize = 8;					     //设置发送字节数长度
						UART_SEND_flag=1;
					}
				}
			}			 
		}
	}
/*************************************以下为校准部分**************************************************************************/
	if (((UART_Buffer_Rece[0] == 0x01)&&(UART_Buffer_Rece[2] == 0xA5)))			   //校准开始
	{ 
		vu16 crc_result;
		crc_result = (UART_Buffer_Rece[6] << 8) + UART_Buffer_Rece[7];
		if (crc_result == Hardware_CRC(UART_Buffer_Rece,6) ||crc_result==0)
		{
			/**************负载电压低档校准***************************/
			if(UART_Buffer_Rece[1] == 0x01)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Vmon_Load_value;//测量电压值
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取低段
			}
			if (UART_Buffer_Rece[1] == 0x02)			   //电压测量校准完成
			{
				vu32 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu32 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ =Vmon_Load_value;//测量电压值
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取高段
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_Load_CorrectionV_LOW = var32a;
				var32a=0;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_Load_CorrectionV_LOW;    //内阻电压测量参数低档	
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_LoadV_Offset_LOW = var32b;     //内阻电压测量参数  
					CLists.CPolar0 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_LoadV_Offset_LOW = var32a;
					CLists.CPolar0 &= ~0x01;
				}			
					//参数写进FLASH
				Flag_DAC_OFF=0;
			}
			
			/**************负载电压高档校准***************************/
			if(UART_Buffer_Rece[1] == 0x03)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Vmon_Load_value;//测量电压值
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取低段
			}
			if (UART_Buffer_Rece[1] == 0x04)			   //电压测量校准完成
			{
				vu32 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu32 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ =Vmon_Load_value;//测量电压值
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取高段
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_Load_CorrectionV_HIG = var32a;
				var32a=0;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_Load_CorrectionV_HIG;      //内阻电压测量参数高档
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_LoadV_Offset_HIG = var32b;     //内阻电压测量参数
					CLists.CPolar0 |= 0x04;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_LoadV_Offset_HIG = var32a;
					CLists.CPolar0 &= ~0x04;
				}			
					//参数写进FLASH
				Flag_DAC_OFF=0;
			}
	     /*****************负载电流测量和控制校准*************************/
			if (UART_Buffer_Rece[1] == 0x05)			   //CC模式校准
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Imon_Load_value;//
				Modify_C_READ = Contr_Laod;//
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
			}
			if (UART_Buffer_Rece[1] == 0x06)			   
			{
				vu32 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu32 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_D_READ=0;
				Modify_B_ACT=0;
				
				Modify_B_READ = Imon_Load_value;
				Modify_D_READ = Contr_Laod;
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
				
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_Load_CorrectionA_LOW = var32a;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_Load_CorrectionA_LOW;    //负载CC模式电流测量参数低档
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_LoadA_Offset_LOW = var32b;     //负载CC模式电流测量参数
					CLists.CPolar2 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_LoadA_Offset_LOW = var32a;
					CLists.CPolar2 &= ~0x01;					
				}
	//---------------------------------------------------------------------------------//
				var32c = Modify_B_ACT; 
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=var16a*2;
				var32c=var32c/var16a;
				CLists.CSET_Load_CorrectionA_LOW = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_Load_CorrectionA_LOW;     //负载CC模式控制电流低档
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_LoadA_Offset_LOW = var32d;    //负载CC模式控制电流参数  	
					CLists.CPolar2 |= 0x04;
				}
				else 
				{
					var32c = var32c - var32d;
					CLists.CSET_LoadA_Offset_LOW = var32c;
					CLists.CPolar2 &= ~0x04;
				}
					
				Flag_DAC_OFF =0;
			}
			
			/*****************负载高档电流测量和控制校准*************************/
			if (UART_Buffer_Rece[1] == 0x07)			   //CC模式校准
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Imon_Load_value;//
				Modify_C_READ = Contr_Laod;//
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
			}
			if (UART_Buffer_Rece[1] == 0x08)			   
			{
				vu32 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu32 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_D_READ=0;
				Modify_B_ACT=0;
				
				Modify_B_READ = Imon_Load_value;
				Modify_D_READ = Contr_Laod;
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
				
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_Load_CorrectionA_LOW = var32a;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_Load_CorrectionA_LOW;   //负载CC模式电流测量参数低档
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_LoadA_Offset_HIG = var32b;     //负载CC模式电流测量参数
					CLists.CPolar3 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_LoadA_Offset_HIG = var32a;
					CLists.CPolar3 &= ~0x01;					
				}
	//---------------------------------------------------------------------------------//
				var32c = Modify_B_ACT; 
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=var16a*2;
				var32c=var32c/var16a;
				CLists.CSET_Load_CorrectionA_HIG = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_Load_CorrectionA_HIG;  //负载CC模式电流测量参数高档
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_LoadA_Offset_HIG = var32d;   //负载CC模式控制电流参数
					CLists.CPolar3 |= 0x04;
				}
				else 
				{
					var32c = var32c - var32d;
					CLists.CSET_LoadA_Offset_HIG = var32c;
					CLists.CPolar3 &= ~0x04;
				}
					
				Flag_DAC_OFF =0;
			}
	/*************************负载CV模式电压控制校准低档*******************************/
			if(UART_Buffer_Rece[1] == 0x09)
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_C_READ = Contr_Laod;//设置电压值
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取低段
			}
			if (UART_Buffer_Rece[1] == 0x0A)			   //电压测量校准完成
			{
				vu32 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu32 var16a;
				vu32 var32c;
				vu32 var32d;
				
				Modify_B_READ=0;
				Modify_D_READ=0;
				Modify_B_ACT=0;
			
				Modify_D_READ =Contr_Laod;//设置电压值
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取高段	
				var32c = Modify_B_ACT; //CV模式电压控制校准
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=(var16a*2);
				var32c=var32c/var16a;
				CLists.CSET_Load_CorrectionV_LOW = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_Load_CorrectionV_LOW;   //负载CV模式控制电压抵挡
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_LoadV_Offset_LOW = var32d;    	//负载CV模式控制电压参数   
					CLists.CPolar1 |= 0x01;
				}
				else 
				{
					var32c = var32c - var32d;
					CLists.CSET_LoadV_Offset_LOW = var32c;
					CLists.CPolar1 &= ~0x01;
				}
					//参数写进FLASH
				Flag_DAC_OFF=0;
			}
			/*************************负载CV模式电压控制校准高档*******************************/
			if(UART_Buffer_Rece[1] == 0x0B)
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_C_READ = Contr_Laod;//设置电压值
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取低段
			}
			if (UART_Buffer_Rece[1] == 0x0C)			   //电压测量校准完成
			{
				vu32 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu32 var16a;
				vu32 var32c;
				vu32 var32d;
				
				Modify_B_READ=0;
				Modify_D_READ=0;
				Modify_B_ACT=0;
			
				Modify_D_READ =Contr_Laod;//设置电压值
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取高段	
				var32c = Modify_B_ACT; //CV模式电压控制校准
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=(var16a*2);
				var32c=var32c/var16a;
				CLists.CSET_Load_CorrectionV_LOW = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_Load_CorrectionV_HIG;      //负载CV模式控制电压高档
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_LoadV_Offset_HIG = var32d;
					CLists.CPolar1 |= 0x04;
				}
				else 
				{
					var32c = var32c - var32d;
					CLists.CSET_LoadV_Offset_HIG = var32c;      //负载CV模式控制电压参数
					CLists.CPolar1 &= ~0x04;
				}
					//参数写进FLASH
				Flag_DAC_OFF=0;
			}
	/************************内阻校准低档**********************************/
			if(UART_Buffer_Rece[1] == 0x0D)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Rmon_value;//测量电阻值
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取低段
			}
			if (UART_Buffer_Rece[1] == 0x0E)			   
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;		
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ =Rmon_value;//测量电阻值
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取高段
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_CorrectionR_LOW = var32a;
				var32a=0;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_CorrectionR_LOW;  //内阻测量参数低档
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_ReadR_Offset_LOW = var32b;         //内阻测量系数
					CLists.CPolar4 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_ReadR_Offset_LOW = var32a;
					CLists.CPolar4 &= ~0x01;
				}
					//参数写进FLASH
				Flag_DAC_OFF=0;
			}		
			/************************内阻高档校准**********************************/
			if(UART_Buffer_Rece[1] == 0x0F)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Rmon_value;//测量电阻值
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取低段
			}
			if (UART_Buffer_Rece[1] == 0x10)			   
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;		
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ =Rmon_value;//测量电阻值
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取高段
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_CorrectionR_HIG = var32a;
				var32a=0;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_CorrectionR_HIG;    //内阻测量参数高档
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_ReadR_Offset_HIG = var32b;     //内阻测量系数
					CLists.CPolar4 |= 0x04;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_ReadR_Offset_HIG = var32a;
					CLists.CPolar4 &= ~0x04;
				}
					//参数写进FLASH
				Flag_DAC_OFF=0;
			}	
			
			/*******************************电源电压控制和测量校准******************************************/	
			if (UART_Buffer_Rece[1] == 0x11)			   //电流测量校准
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Vmon_POWE_value;//测量电流
				Modify_C_READ = Contr_Voltage;//设置电流
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
			}

			if (UART_Buffer_Rece[1] == 0x12)			   //电流测量校准完成
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				
				Modify_D_READ=0;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				
				Modify_D_READ = Contr_Voltage;
				Modify_B_READ = Vmon_POWE_value;
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_POWER_CorrectionV = var32a;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_POWER_CorrectionV;    //电源测量电压参数
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_POWERV_Offset = var32b;     //电源CV模式电压测量参数  
					CLists.CPolar5 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_POWERV_Offset = var32a;
					CLists.CPolar5 &= ~0x01;					
				}
				var32c = Modify_B_ACT; //设置电压校准
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=var16a*2;
				var32c=var32c/var16a;
				CLists.CSET_POWER_CorrectionV = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_POWER_CorrectionV;    //电源电压控制参数
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_POWERV_Offset = var32d;    //电源CV模式电压控制参数    
					CLists.CPolar5 |= 0x04;
				}
				else 
				{
					var32c = var32c - var32d;
					CLists.CSET_POWERV_Offset = var32c;
					CLists.CPolar5 &= ~0x04;
				}
					
				Flag_DAC_OFF=0;
			}
	/*******************************电源CC模式电流测量和控制校准******************************************/	
			if (UART_Buffer_Rece[1] == 0x13)			   //电流测量校准
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Imon_POWE_value;//测量电流
				Modify_C_READ = Contr_Current;//设置电流
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
			}
			if (UART_Buffer_Rece[1] == 0x14)			   //电流测量校准完成
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				
				Modify_D_READ=0;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				
				Modify_D_READ = Contr_Current;
				Modify_B_READ = Imon_POWE_value;
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_POWER_CorrectionA_CC = var32a;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_POWER_CorrectionA_CC;    //电源CC模式电流测量参数
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_POWERA_Offset_CC = var32b;    //电源CC模式电流测量参数 
					CLists.CPolar6 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_POWERA_Offset_CC = var32a;
					CLists.CPolar6 &= ~0x01;					//电流测量系数和零点校准完成
				}
				var32c = Modify_B_ACT; //设置电流校准
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=var16a*2;
				var32c=var32c/var16a;
				CLists.CSET_POWER_CorrectionA = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_POWER_CorrectionA;    //电源电流控制参数
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_POWERA_Offset = var32d;    //电源CC模式电流控制参数 
					CLists.CPolar6 |= 0x04;
				}
				else 
				{
					var32c = var32c - var32d;
					CLists.CSET_POWERA_Offset = var32c;
					CLists.CPolar6 &= ~0x04;
				}
				
				Flag_DAC_OFF=0;
			}
	
	/****************电源CV电流测量校准**********************************/
			if (UART_Buffer_Rece[1] == 0x15)			  
			{ 
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Imon_POWE_value;
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
			}

			if (UART_Buffer_Rece[1] == 0x16)			   
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ = Imon_POWE_value;
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_POWER_CorrectionA_CV = var32a;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_POWER_CorrectionA_CV;   //电源CV模式电流测量参数
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_POWERA_Offset_CV = var32b;   //电源CC模式电流测量参数
					CLists.CPolar6 |= 0x08;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_POWERA_Offset_CV = var32a;
					CLists.CPolar6 &= ~0x08;					
				}
					
				Flag_DAC_OFF=0;
			}
	/****************************识别电阻1校准*****************************************/
			if(UART_Buffer_Rece[1] == 0x17)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				//Modify_A_READ = R1mon_value;//测量电压值
				Modify_A_READ=(R1mon_value*1000)/(65535-R1mon_value);
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取低段
			}
			if (UART_Buffer_Rece[1] == 0x18)			   //电压测量校准完成
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				//Modify_B_READ =R1mon_value;//测量电压值
				Modify_B_READ=(R1mon_value*1000)/(65535-R1mon_value);
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取高段
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_CorrectionR1 = var32a;     //识别电阻1参数
				var32a=0;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_CorrectionR1;
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_ReadR1_Offset = var32b;    //内阻测量系数
					CLists.CPolar7 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_ReadR1_Offset = var32a;
					CLists.CPolar7 &= ~0x01;
				}
					//参数写进FLASH
				Flag_DAC_OFF=0;
			}	
			/***************************识别电阻2校准**************************************/
			if(UART_Buffer_Rece[1] == 0x19)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = R2mon_value;//测量电压值
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取低段
			}
			if (UART_Buffer_Rece[1] == 0x1A)			   //电压测量校准完成
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ =R2mon_value;//测量电压值
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//读取高段
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_CorrectionR2 = var32a;
				var32a=0;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_CorrectionR2;    	//识别电阻2参数    
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_ReadR2_Offset = var32b;    //内阻测量系数  
					CLists.CPolar7 |= 0x04;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_ReadR2_Offset = var32a;
					CLists.CPolar7 &= ~0x04;
				}
					//参数写进FLASH
				Flag_DAC_OFF=0;
			}	
	
			Write_Flash_Bytes();
			
	/***********接受DAC*******************************************/
			if (UART_Buffer_Rece[1] == 0x1B)			   
			{
				Contr_Laod = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
				Flag_DAC_OFF=1;
			}
			if (UART_Buffer_Rece[1] == 0x1C)			   
			{
				Contr_Voltage = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
				Flag_DAC_OFF=1;
			}
			if (UART_Buffer_Rece[1] == 0x1D)			   
			{
				Contr_Current = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
				Flag_DAC_OFF=1;
			}
		}
//===================================================================================
	}
}
//===============================AD值转换成测量值============================================//
void Transformation_ADC(void)  
{
	vu32 var32;
	vu32 var32a;
/*****************************负载测量电压转换******************************************/
	if(LOAD_V_SW==0)//低档
	{
		var32 = Vmon_Load_value;
		var32 = var32 * CalPara.TestLV[0] + CalPara.OffsetLV[0];  
//		if ((CLists.CPolar0 & 0x01) == 0x01)		  
//		{
//			if (var32 < CLists.CREG_LoadV_Offset_LOW) 
//			{
//				var32 = 0;
//			}
//			else var32 = var32 - CLists.CREG_LoadV_Offset_LOW;
//		}
//		else var32 = var32 + CLists.CREG_LoadV_Offset_LOW;
//		var32 = var32 >> 12;
		if (var32 < 10) var32 = 0;				  //40mV以下清零
		Para.CVoltage = var32;
//		DISS_Voltage=Para.CVoltage;
		DISS_Voltage=(float)Para.CVoltage/1000;//计算显示电压
// 		if( DISS_Voltage >= 10 ){
// 				CLists.CLOAD_V_SW  = 1;
// 		}
// 	//如果DISS_Voltage的值  >=10 CLists.CLOAD_V_SW  =1
		var32 = 0;
	}
	else//高档
	{
		var32 = Vmon_Load_value;
		var32 = var32 * CalPara.TestLV[1] + CalPara.OffsetLV[1]; 
//		if ((CLists.CPolar0 & 0x04) == 0x04)		  
//		{
//			if (var32 < CLists.CREG_LoadV_Offset_HIG) 
//			{
//				var32 = 0;
//			}
//			else var32 = var32 - CLists.CREG_LoadV_Offset_HIG;
//		}
//		else var32 = var32 + CLists.CREG_LoadV_Offset_HIG;
//		var32 = var32 >> 12;
		if (var32 < 10) var32 = 0;				  //40mV以下清零
		Para.CVoltage = var32;
//		DISS_Voltage=Para.CVoltage;
		DISS_Voltage=(float)Para.CVoltage/1000;//计算显示电压
// 		if( DISS_Voltage < 12 ){
// 				CLists.CLOAD_V_SW  = 0;
// 		}
   //如果DISS_Voltage的值  <12  CLists.CLOAD_V_SW  = 0
		var32 = 0;
	}
	
	/*******************负载测量电流转换**************************************/
	if(LOAD_I_SW==0)//低档
	{
		var32 = Imon_Load_value;
		var32 = var32 * CalPara.TestLC[0] + CalPara.OffsetTestLC[0];  
//		if ((CLists.CPolar2 & 0x01) == 0x01)		  
//		{
//			if (var32 < CLists.CREG_LoadA_Offset_LOW) 
//			{
//				var32 = 0;
//			}
//			else var32 = var32 - CLists.CREG_LoadA_Offset_LOW;
//		}
//		else var32 = var32 + CLists.CREG_LoadA_Offset_LOW;
//		var32 = var32 >> 12;
		Para.CLaod_Current = var32;
//		DISS_Current=Para.CLaod_Current;
		DISS_Current=(float)Para.CLaod_Current/1000;//计算显示电流
		var32 = 0;	
	}
	else//高档
	{
		var32 = Imon_Load_value;
		var32 = var32 * CalPara.TestLC[1] + CalPara.OffsetTestLC[1];   
//		if ((CLists.CPolar4 & 0x01) == 0x01)		  
//		{
//			if (var32 < CLists.CREG_LoadA_Offset_HIG) 
//			{
//				var32 = 0;
//			}
//			else var32 = var32 - CLists.CREG_LoadA_Offset_HIG;
//		}
//		else var32 = var32 + CLists.CREG_LoadA_Offset_HIG;
//		var32 = var32 >> 12;
		Para.CLaod_Current = var32;
		if(Para.CLaod_Current < 1000)
		{
			Para.CLaod_Current = 0;
		}
//		DISS_Current=Para.CLaod_Current;
		DISS_Current=(float)Para.CLaod_Current/1000;//计算显示电流
		var32 = 0;
	}
	
	/*************************负载电压和电流控制转换**************************************/
	if(LOAD_MODE==0)//CC
	{
		if(LOAD_I_SW==0)//低档
		{
			var32 = Para.CSET_Current_Laod;
			var32 = Para.CSET_Current_Laod * CalPara.SetLC[0] + CalPara.OffsetLC[0];
//			var32=var32<<12;   
//			if ((CLists.CPolar2 & 0x04) == 0)			   
//			{
//				if (var32 < CLists.CSET_LoadA_Offset_LOW) var32 = 0;
//				else var32 = var32 - CLists.CSET_LoadA_Offset_LOW;
//			}
//			else var32 = var32 + CLists.CSET_LoadA_Offset_LOW;
//			var32 = var32/CLists.CSET_Load_CorrectionA_LOW;
//			var32=var32>>1;
			Contr_Laod = var32;
			if(Para.CSET_Current_Laod==0)
			{
				Contr_Laod=0;
			}
			var32 = 0;
		}
		else//高档
		{
			var32 = Para.CSET_Current_Laod;
			var32 = Para.CSET_Current_Laod * CalPara.SetLC[1] + CalPara.OffsetLC[1];  
//			if ((CLists.CPolar3 & 0x04) == 0)			   
//			{
//				if (var32 < CLists.CSET_LoadA_Offset_HIG) var32 = 0;
//				else var32 = var32 - CLists.CSET_LoadA_Offset_HIG;
//			}
//			else var32 = var32 + CLists.CSET_LoadA_Offset_HIG;
//			var32 = var32/CLists.CSET_Load_CorrectionA_HIG;
//			var32=var32>>1;
			Contr_Laod = var32;
			if(Para.CSET_Current_Laod==0)
			{
				Contr_Laod=0;
			}
			var32 = 0;
		}
		
	}
	else if(LOAD_MODE==1)//CV
	{
		if(LOAD_V_SW==0)//低档
		{
			var32 = Para.CSET_Voltage_Laod;
			var32 = Para.CSET_Voltage_Laod * CalPara.SetCV[0] + CalPara.OffsetCV[0];   
//			if ((CLists.CPolar2 & 0x04) == 0)			   
//			{
//				if (var32 < CLists.CSET_LoadV_Offset_LOW) var32 = 0;
//				else var32 = var32 - CLists.CSET_LoadV_Offset_LOW;
//			}
//			else var32 = var32 + CLists.CSET_LoadV_Offset_LOW;
//			var32 = var32/CLists.CSET_Load_CorrectionV_LOW;
//			var32=var32>>1;
			Contr_Laod = var32;
			if(Para.CSET_Voltage_Laod==0)
			{
				Contr_Laod=0;
			}
			var32 = 0;
		}
		else//高档
		{
			var32 = Para.CSET_Voltage_Laod;
			var32 = Para.CSET_Voltage_Laod * CalPara.SetCV[1] + CalPara.OffsetCV[1];  
//			if ((CLists.CPolar2 & 0x04) == 0)			   
//			{
//				if (var32 < CLists.CSET_LoadV_Offset_HIG) var32 = 0;
//				else var32 = var32 - CLists.CSET_LoadV_Offset_HIG;
//			}
//			else var32 = var32 + CLists.CSET_LoadV_Offset_HIG;
//			var32 = var32/CLists.CSET_Load_CorrectionV_HIG;
//			var32=var32>>1;
			Contr_Laod = var32;
//			if(Para.CSET_Voltage_Laod==0)
//			{
//				Contr_Laod=0;
//			}
//			var32 = 0;
		}
		
	}
/*****************************内阻值转换*******************************************/
	if(RCONT_SW==0)//低档
	{
		var32 = Rmon_value;
		var32 = var32 * CalPara.TestR[0] + CalPara.OffsetR[0];  
//		if ((CLists.CPolar4 & 0x01) == 0x01)		  
//		{
//			if (var32 < CLists.CREG_ReadR_Offset_LOW) 
//			{
//				var32 = 0;
//			}
//			else var32 = var32 - CLists.CREG_ReadR_Offset_LOW;
//		}
//		else var32 = var32 + CLists.CREG_ReadR_Offset_LOW;
//		var32 = var32 >> 12;
//		if (var32 < 1)
//		{
//			var32 = 0;				  //清零
//		}
		Para.CR_VLUE = var32;
		if(Rmon_value > 60000 && finishdelay == 0)
		{
			if(finishflag == 1)
			{
				finishflag = 0;
			}
		}
//		if(Para.CR_VLUE > 250)
//		{
//			Para.CR_VLUE = 0;
//		}
		var32 = 0;	
	}
	else//高档
	{
		var32 = Rmon_value;
		var32 = var32 * CalPara.TestR[1] + CalPara.OffsetR[1];  
//		if ((CLists.CPolar4 & 0x04) == 0x04)		  
//		{
//			if (var32 < CLists.CREG_ReadR_Offset_HIG) 
//			{
//				var32 = 0;
//			}
//			else var32 = var32 - CLists.CREG_ReadR_Offset_HIG;
//		}
//		else var32 = var32 + CLists.CREG_ReadR_Offset_HIG;
//		var32 = var32 >> 12;
//		if (var32 < 1)
//		{
//			var32 = 0;				  //清零
//		}
		Para.CR_VLUE = var32;
		if(Para.CR_VLUE > 1000)
		{
			var32 = Rmon_value;
			var32 = var32 * CalPara.TestR[2] + CalPara.OffsetR[2];
			Para.CR_VLUE = var32;
		}
		if(Rmon_value > 60000 && finishdelay == 0)
		{
			if(finishflag == 1)
			{
				finishflag = 0;
			}
		}
		var32 = 0;	
	}
	
	/*****************************稳压电源测量电压转换*******************************************/
//	var32 = Vmon_POWE_value;
//	var32 = var32 * CalPara.TestPV[0] + CalPara.OffsetPV[0];  
//	//	if ((CLists.CPolar5 & 0x01) == 0x01)		  
//	//	{
//	//		if (var32 < CLists.CREG_POWERV_Offset) 
//	//		{
//	//			var32 = 0;
//	//		}
//	//		else var32 = var32 - CLists.CREG_POWERV_Offset;
//	//	}
//	//	else var32 = var32 + CLists.CREG_POWERV_Offset;
//	//	var32 = var32 >> 12;
//	//	if (var32 < 40) var32 = 0;				  //40mV以下清零
//	Para.CPOW_Voltage = var32;
//	//	DISS_POW_Voltage=Para.CPOW_Voltage;
	DISS_POW_Voltage=(float)Para.CPOW_Voltage/1000;//计算显示电压
//	var32 = 0;
/*****************************稳压电源测量电流转换*******************************************/
//	var32 = Imon_POWE_value;
//	if(flag_CC_MODE==1)
//	{
//		var32 = var32 * CalPara.TestPC[0] + CalPara.OffsetTestPC[0];	   
////		if ((CLists.CPolar6 & 0x01) == 0x01)			   
////		{
////			if (var32 < CLists.CREG_POWERA_Offset_CC) var32 = 0;
////			else var32 = var32 - CLists.CREG_POWERA_Offset_CC;
////		}
////		else
////		{
////			var32 = var32 + CLists.CREG_POWERA_Offset_CC;
////		}	
////		var32 = var32 >> 12;
//		Para.CCurrent = var32;
////		DISS_POW_Current=Para.CCurrent;
		DISS_POW_Current=(float)Para.CCurrent/100;//计算显示电流
//	}
//	else
//	{
//		var32 = var32 * CLists.CREG_POWER_CorrectionA_CV;	   
//		if ((CLists.CPolar6 & 0x08) == 0x08)			   
//		{
//			if (var32 < CLists.CREG_POWERA_Offset_CV) var32 = 0;
//			else var32 = var32 - CLists.CREG_POWERA_Offset_CV;
//		}
//		else
//		{
//			var32 = var32 + CLists.CREG_POWERA_Offset_CV;
//		}	
//		var32 = var32 >> 12;
//		Para.CCurrent = var32;;
////		DISS_POW_Current=Para.CCurrent;
//		DISS_POW_Current=(float)Para.CCurrent/1000;//计算显示电流
//	}
/**************************稳压电源设置电压转换******************************************/
	var32 = Para.CSET_Voltage;
	var32 = var32 * CalPara.SetPV[0] + CalPara.OffsetSetPV[0];   
//	if ((CLists.CPolar5 & 0x04) == 0)			   
//	{
//		if (var32 < CLists.CSET_POWERV_Offset) var32 = 0;
//		else var32 = var32 - CLists.CSET_POWERV_Offset;
//	}
//	else var32 = var32 + CLists.CSET_POWERV_Offset;
//	var32 = var32/CLists.CSET_POWER_CorrectionV;
//	var32=var32>>1;
	Contr_Voltage = var32;
	if(Para.CSET_Voltage==0)
	{
		Contr_Voltage=0;
	}
	var32 = 0;
/**************************稳压电源设置电流转换**************************************/
	var32 = Para.CSET_Current;
	var32 = var32 * CalPara.SetPC[0] + CalPara.OffsetPC[0];  
//	if ((CLists.CPolar6 & 0x04) == 0)			   
//	{
//		if (var32 < CLists.CSET_POWERA_Offset) var32 = 0;
//		else var32 = var32 - CLists.CSET_POWERA_Offset;
//	}
//	else var32 = var32 + CLists.CSET_POWERA_Offset;
//	var32 = var32/CLists.CSET_POWER_CorrectionA;
//	var32=var32>>1;
	Contr_Current = var32;
	if(Para.CSET_Current==0)
	{
		Contr_Current=0;
	}
	var32 = 0;
	
/********************识别电阻1转换******************************/
//	var32=(R1mon_value*1000)/(65535-R1mon_value);
	var32 = (R1mon_value*1000)/(65535-R1mon_value);
	var32 = var32 * CalPara.TestR1[0] + CalPara.OffsetR1[0];  
//	if ((CLists.CPolar7 & 0x01) == 0x01)		  
//	{
//		if (var32 < CLists.CREG_ReadR1_Offset) 
//		{
//			var32 = 0;
//		}
//		else var32 = var32 - CLists.CREG_ReadR1_Offset;
//	}
//	else var32 = var32 + CLists.CREG_ReadR1_Offset;
//	var32 = var32 >> 12;
//	if (var32 < 1)
//	{
//		var32 = 0;				  //清零
//	}
	Para.CR1_VLUE = var32;
//	if(R1mon_value > 34000)
//	{
//		Para.CR1_VLUE = 100;
//	}
	var32 = 0;	
	
	
/********************识别电阻2转换******************************/	
	var32 = (R2mon_value*1000)/(65535-R2mon_value);
	var32 = var32 * CalPara.TestR2[0] + CalPara.OffsetR2[0];   
//	if ((CLists.CPolar7 & 0x04) == 0x04)		  
//	{
//		if (var32 < CLists.CREG_ReadR2_Offset) 
//		{
//			var32 = 0;
//		}
//		else var32 = var32 - CLists.CREG_ReadR2_Offset;
//	}
//	else var32 = var32 + CLists.CREG_ReadR2_Offset;
//	var32 = var32 >> 12;
//	if (var32 < 1)
//	{
//		var32 = 0;				  //清零
//	}
	Para.CR2_VLUE = var32;
	var32 = 0;	
}
/********************************************************************************
  * 名称 :   MODBUS_Conrtl
  * 作者 :  zhan
  * 版本 :  V1.0
  * 日期 :  2016-xx-xx
  * 作用 :  MODBUS解析函数
  ******************************************************************************/ 
//-----------------------------CRC检测--------------------------------------------//
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count)    //CRC16
{
	vu16 CRC_Result=0xffff;
	vu8 i;
	if(count==0)
	{
		count=1;
	}
	while(count--)
	{
		CRC_Result^=*p_buffer;
		for(i=0;i<8;i++)
		{
			if(CRC_Result&1)
			{
				CRC_Result>>=1;
				CRC_Result^=0xA001;
			}
			else 
			{
				CRC_Result>>=1;
			}
		}
		p_buffer++;
	}
	return CRC_Result;
}