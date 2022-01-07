/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * 文件名  ：my_register
 * 作者    ：李振
 * 描述    ：库函数版
 * 内容    ：Define register定义系统使用的寄存器，全局变量
 * 注意		 ：头文件中只能有声明，不能有定义
 * 修改日期：2014-12-9
*********************************************************************/

#include "STM32F4xx.h"
#ifndef __my_register_h__
#define __my_register_h__
//=============================================================================

//=============================================================================
#define NOP   __nop();	  //空指令

//============================================================================
extern struct bitDefine flagA,flagB,flagC,flagD,flagE,flagF,flagG;
#define   Flag_ADC_Full             (flagA.bit1)//DMA传输完成标志位 
#define   Flag_BEEP						    	(flagA.bit2)//蜂鸣器触发标志
#define   Flag_Key_Scan							(flagA.bit3)//按键定时扫描标志
#define   UART_SEND_flag						(flagA.bit4)//串口发送完成标志
#define   UART_Buffer_Rece_flag			(flagA.bit5)//串口接收完成标志
#define   Flag_Swtich_ON      			(flagA.bit6)//启动按键处于ON标志
#define   Flag_0_5S      		      	(flagA.bit7)//显示设置电压闪烁标志

#define   Flag_1S      		         	(flagB.bit0)//显示设置电流闪烁标志
#define   Flag_5S      		        	(flagB.bit1)//退出设置状态标志
#define   Flag_Long                 (flagB.bit2)//按键长按标志
#define   Flag_DAC_OFF              (flagB.bit3)//鏄惁杞崲DAC鎺у埗鍊兼爣蹇?
#define   Flag_SetA                 (flagB.bit5)//进入设置电流标志位
#define   Flag_SetV                 (flagB.bit6)//进入设置电压标志位 
#define   Flag_DisSet               (flagB.bit7)//显示设置电压和电流标志位

#define   Flag_Beep_OFF             (flagC.bit6)//蜂鸣器关闭标志

#define   flag_Tim_USART            (flagD.bit0)//串口计算清零标志
#define   flag_Test                 (flagD.bit1)//自动测试起停标志
#define   flag_Test_PASS            (flagD.bit2)//自动测试PASS标志
#define   flag_Test_FAIL            (flagD.bit3)//自动测试FAIL标志
#define   flag_Test_Time            (flagD.bit4)//自动测试等待时间标志
#define   flag_Test_Start           (flagD.bit5)//自动测试比较开始标志
#define   flag_Test_Min             (flagD.bit6)//自动测试下限标志
#define   flag_Load_CC              (flagD.bit7)//负载CC/CV模式标志位

#define   flag_Test_Door             (flagE.bit0)//自动测试门槛标志
#define   flag_Test_SetTime          (flagE.bit1)//自动测试时间标志 
#define   flag_OVP                   (flagE.bit2)//OVP标志
#define   flag_OCP                   (flagE.bit3)//OCP标志
#define   flag_Doub                  (flagE.bit4)//按键双击
#define   flag_t_Doub                (flagE.bit5)//按键双击计时标志
#define   flag_Time_SW               (flagE.bit6)//定时功能开启标志
#define   flag_CC_MODE               (flagE.bit7)//工作模式标志

#define   flag_Adjust                (flagF.bit0)//
#define   flag_ADJ_ON                (flagF.bit1)//
#define   flag_ADJ_VL                (flagF.bit2)//
#define   flag_ADJ_VH                (flagF.bit3)//
#define   flag_ADJ_ALCC              (flagF.bit4)//
#define   flag_ADJ_AHCC              (flagF.bit5)//
#define   flag_ADJ_ALCV              (flagF.bit6)//
#define   flag_ADJ_AHCV              (flagF.bit7)//

#define   flag_DisVL                 (flagG.bit0)//
#define   flag_OverV                 (flagG.bit1)//
#define   flag_OverACC               (flagG.bit2)//
#define   flag_OverACV               (flagG.bit3)//
#define   flag_Just_Usart            (flagG.bit4)//
#define   flag_FAN_ON                (flagG.bit5)//
#define   flag_ADJ_FAN               (flagG.bit6)//
#define   flag_TM7707_CH						 (flagG.bit7)//
//=============================================================================
extern vu32 GE_V;
extern vu32 SHI_V;
extern vu32 BAI_V;
extern vu32 QIAN_V;  //数码管扫描段码
extern vu32 GE_A;
extern vu32 SHI_A;
extern vu32 BAI_A;
extern vu32 QIAN_A;

//--------------------------
extern vu8 NewKey;
extern vu8 DAC_Flag;
extern vu8 Beep_Flag;
extern vu8 t_beep;
//extern vu8 t_KeyScan;
extern vu8 t_LEDScan;  //定时计数器
extern vu8 t_KEYON;
extern vu16 t_0_5S;
extern vu16 t_1S;
extern vu16 t_5S;
extern vu16 t_Basket;
extern vu16 Basket_V;
extern vu16 Basket_A;
extern vu16 t_OFF_V;
extern vu16 t_ON_V;
extern vu16 t_OFF_A;
extern vu16 t_ON_A;
extern vu8 t_USART;
extern vu16 ADJ_Write;//校准时需写入的实际值
extern vu8 OFFSET_Zore;//零点值
extern vu8 ADDR;
extern vu8 t_lock;
extern vu8 t_onoff;
extern vu16 MAX_limit;
extern vu16 MIN_limit;
extern vu16 Test_Time;
extern vu16 Test_Time1;
extern vu16 Time_sw;
extern vu16 Test_Daley;
extern vu16 Test_C_Door;
extern vu16 TEMP_OFFSET;//温度补偿值
//--------------------------
extern vu8 UART_Buffer_Size;//串口接收数据长度
extern vu8 Transmit_BUFFERsize;
//---------------------------
extern vu16 NTC_value;   //AD值
extern vu16 Vmon_POWE_value;//电源电压
extern vu16 Imon_POWE_value;//电源电流
extern vu16 Imon_Load_value;//负载电流
extern vu16 Vmon_Load_value;//负载电压
extern vu16 Rmon_value;//内阻
extern vu16 R1mon_value;//识别电阻1
extern vu16 R2mon_value;//识别电阻2
extern vu32 shutcount1;
extern vu32 shutcount2;
extern float DISS_Voltage;
extern float DISS_POW_Voltage;
extern float DISS_Current;
extern float DISS_POW_Current;
extern float DISS_R;
extern vu8 MODE;
extern vu8 mainswitch;
extern vu8 finishflag;
extern vu32 Run_Control[17];
extern u16 cdcswdelay;
extern vu8 lockflag;
extern vu8 version;
extern short Temperature;
extern vu8 sendwait;
extern vu8 listsend;
extern vu8 overflag;
#define onoff_Load                    Run_Control[0]  //ON/OFF
#define onoff_POWE                    Run_Control[1]  //ON/OFF
#define SET_Voltage 	    	          Run_Control[2]  //电源设定电压
#define SET_Current	    	    		    Run_Control[3]  //电源设定电流
#define SET_Voltage_Laod 					    Run_Control[4]  //负载设定电压
#define SET_Current_Laod					    Run_Control[5]  //负载设定电流
#define LOAD_MODE								      Run_Control[6]  //负载工作模式
#define LOAD_V_SW								      Run_Control[7]  //负载电压档位
#define LOAD_I_SW							        Run_Control[8]  //负载电流档位
#define RCONT_SW							   	    Run_Control[9]  //内阻测量档位

#define POW_Voltage	    	            Run_Control[10]  //电源测量电压
#define Voltage	    	                Run_Control[11]  //负载测量电压
#define Current	    	    		        Run_Control[12]  //电源测量电流
#define Laod_Current	    		        Run_Control[13]  //负载测量电流
#define R_VLUE											  Run_Control[14]  //电池内阻值
#define R1_VLUE											  Run_Control[15]  //识别电阻1
#define R2_VLUE											  Run_Control[16]  //识别电阻2

extern vu16 Contr_Voltage;//
extern vu16 Contr_Current;//
extern vu16 Contr_Laod;//
extern vu32 Correct_Parametet[17];
#define  REG_Load_CorrectionV_LOW  			  Correct_Parametet[0]//内阻电压测量参数低档
#define  REG_Load_CorrectionV_HIG   			Correct_Parametet[1]//内阻电压测量参数高档
#define  REG_Load_CorrectionA_LOW			    Correct_Parametet[2]//负载CC模式电流测量参数低档
#define  REG_Load_CorrectionA_HIG		      Correct_Parametet[3]//负载CC模式电流测量参数高档
#define  SET_Load_CorrectionV_LOW			    Correct_Parametet[4]//负载CV模式控制电压低档
#define  SET_Load_CorrectionV_HIG			    Correct_Parametet[5]//负载CV模式控制电压高档
#define  SET_Load_CorrectionA_LOW			    Correct_Parametet[6]//负载CC模式控制电流低档
#define  SET_Load_CorrectionA_HIG			    Correct_Parametet[7]//负载CC模式控制电流高档

#define  REG_POWER_CorrectionV			      Correct_Parametet[8]//电源测量电压参数
#define  REG_POWER_CorrectionA_CC			    Correct_Parametet[9]//电源CC模式电流测量参数
#define  REG_POWER_CorrectionA_CV			    Correct_Parametet[10]//电源CV模式电流测量参数
#define  SET_POWER_CorrectionV 			      Correct_Parametet[11]//电源电压控制参数
#define  SET_POWER_CorrectionA 			      Correct_Parametet[12]//电源电流控制参数

#define  REG_CorrectionR_LOW              Correct_Parametet[13]//内阻测量参数低档
#define  REG_CorrectionR_HIG              Correct_Parametet[14]//内阻测量参数高档

#define  REG_CorrectionR1                 Correct_Parametet[15]//识别电阻1参数
#define  REG_CorrectionR2                 Correct_Parametet[16]//识别电阻2参数

extern vu32 Correct_Strong[17];//校准系数
#define  REG_LoadV_Offset_LOW             Correct_Strong[0]//内阻电压测量参数
#define  REG_LoadV_Offset_HIG             Correct_Strong[1]//内阻电压测量参数
#define  REG_LoadA_Offset_LOW             Correct_Strong[2]//负载CC模式电流测量参数
#define  REG_LoadA_Offset_HIG             Correct_Strong[3]//负载CC模式电流测量参数
#define  SET_LoadV_Offset_LOW             Correct_Strong[4]//负载CV模式控制电压参数
#define  SET_LoadV_Offset_HIG  					  Correct_Strong[5]//负载CV模式控制电压参数
#define  SET_LoadA_Offset_LOW   					Correct_Strong[6]//负载CC模式控制电流参数
#define  SET_LoadA_Offset_HIG   					Correct_Strong[7]//负载CC模式控制电流参数

#define  REG_POWERV_Offset                Correct_Strong[8]//电源CV模式电压测量参数
#define  REG_POWERA_Offset_CC             Correct_Strong[9]//电源CC模式电流测量参数
#define  REG_POWERA_Offset_CV             Correct_Strong[10]//电源CC模式电流测量参数
#define  SET_POWERV_Offset	              Correct_Strong[11]//电源CV模式电压控制参数
#define  SET_POWERA_Offset                Correct_Strong[12]//电源CC模式电流控制测量参数

#define  REG_ReadR_Offset_LOW             Correct_Strong[13]//内阻测量参数
#define  REG_ReadR_Offset_HIG             Correct_Strong[14]//内阻测量参数

#define  REG_ReadR1_Offset             Correct_Strong[15]//内阻测量参数
#define  REG_ReadR2_Offset             Correct_Strong[16]//内阻测量参数

extern vu8  correct_por[8];
#define  Polar0            			correct_por[0]//负载电压测量和控制系数
#define  Polar1           			 correct_por[1]//负载电流测量和控制系数
#define  Polar2						 correct_por[2]//电源电压控制和测量系数
#define  Polar3						 correct_por[3]//电源电流测量和控制系数
#define  Polar4						 correct_por[4]//内阻测量高低档系数
#define  Polar5						 correct_por[5]//识别电阻测量系数
#define  Polar6						 correct_por[6]//识别电阻测量系数
#define  Polar7						 correct_por[7]//识别电阻测量系数


#define SLAVE_REG_P00		0x0000       //R_VOLU
#define SLAVE_REG_P01		0x0001      //Load_Voltage
#define SLAVE_REG_P02		0x0002      //Load_Current
#define SLAVE_REG_P03		0x0003		//Change_Voltage
#define SLAVE_REG_P04		0x0004		//Change_Current
#define SLAVE_REG_P05		0x0005		//Load_OCP
#define SLAVE_REG_P06		0x0006		//Change_OCP
#define SLAVE_REG_P07		0x0007		//Short_Time
#define SLAVE_REG_P08		0x0008		//Leak_Current
#define SLAVE_REG_P09		0x0009		//R1_Volu
#define SLAVE_REG_P10		0x000A		//R2_Volu
#define SLAVE_REG_P11		0x000B		//Temper
#define SLAVE_REG_P12		0x000C		//DHQ_Voltage
#define SLAVE_REG_P13		0x000D		//MODE
#define SLAVE_REG_P14		0x000E		//Load_Mode
#define SLAVE_REG_P15		0x000F		//Load_SET_Voltage
#define SLAVE_REG_P16		0x0010		//Load_SET_Current
#define SLAVE_REG_P17		0x0011		//Change_SET_Voltage
#define SLAVE_REG_P18		0x0012		//Change_SET_Current

/* RTU 应答代码 */
#define RSP_OK				0		/* 成功 */
#define RSP_ERR_CMD			0x01	/* 不支持的功能码 */
#define RSP_ERR_REG_ADDR	0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		0x04	/* 写入失败 */

#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128

#define POWER_SW_DELAY		10
//---------------------------
extern vu8 UART_Buffer_Rece[16];
extern vu8 UART_Buffer_Send[20];
extern vu16 Hardware_CRC(vu8 *p_buffer,vu8 count);
typedef struct {
	
//	vu32 Conoff_Load;                	//ON/OFF
//	vu32 Conoff_POWE;               	//ON/OFF  
//	vu32 CSET_Voltage; 	    	   		//电源设定电压     
//	vu32 CSET_Current;	    	   		//电源设定电流 		   
//	vu32 CSET_Voltage_Laod; 			//负载设定电压
//	vu32 CSET_Current_Laod;				//负载设定电流	   
//	vu32 CLOAD_MODE;					//负载工作模式		    
//	vu32 CLOAD_V_SW;					//负载电压档位		     
//	vu32 CLOAD_I_SW;					//负载电流档位	       
//	vu32 CRCONT_SW;						//内阻测量档位   	    

//	vu32 CPOW_Voltage;	    	    	//电源测量电压     
//	vu32 CVoltage;	    	        	//负载测量电压    
//	vu32 CCurrent;    	    			//电源测量电流     
//	vu32 CLaod_Current;	    			//负载测量电流       
//	vu32 CR_VLUE;						//电池内阻值					 
//	vu32 CR1_VLUE;						//识别电阻1						
//	vu32 CR2_VLUE;						//识别电阻2							

	vu32 CREG_Load_CorrectionV_LOW;  	//内阻电压测量参数低档			 
	vu32 CREG_Load_CorrectionV_HIG;  	//内阻电压测量参数高档		
	vu32 CREG_Load_CorrectionA_LOW;		//负载CC模式电流测量参数低档		    
	vu32 CREG_Load_CorrectionA_HIG;		//负载CC模式电流测量参数高档    
	vu32 CSET_Load_CorrectionV_LOW;		//负载CV模式控制电压抵挡	   
	vu32 CSET_Load_CorrectionV_HIG;		//负载CV模式控制电压高档	   
	vu32 CSET_Load_CorrectionA_LOW;		//负载CC模式控制电流低档	   
	vu32 CSET_Load_CorrectionA_HIG;		//负载CC模式控制电流高档	    

	vu32 CREG_POWER_CorrectionV;		//电源测量电压参数	    
	vu32 CREG_POWER_CorrectionA_CC;		//电源CC模式电流测量参数
	vu32 CREG_POWER_CorrectionA_CV;		//电源CV模式电流测量参数
	vu32 CSET_POWER_CorrectionV; 		//电源电压控制参数
	vu32 CSET_POWER_CorrectionA; 		//电源电流控制参数		     

	vu32 CREG_CorrectionR_LOW;       	//内阻测量参数低档     
	vu32 CREG_CorrectionR_HIG;       	//内阻测量参数高档

	vu32 CREG_CorrectionR1;        		//识别电阻1参数        
	vu32 CREG_CorrectionR2;          	//识别电阻2参数     

	vu32 CREG_LoadV_Offset_LOW;     	//内阻电压测量参数        
	vu32 CREG_LoadV_Offset_HIG;     	//内阻电压测量参数
	vu32 CREG_LoadA_Offset_LOW;     	//负载CC模式电流测量参数        
	vu32 CREG_LoadA_Offset_HIG;      	//负载CC模式电流测量参数    
	vu32 CSET_LoadV_Offset_LOW;      	//负载CV模式控制电压参数     
	vu32 CSET_LoadV_Offset_HIG;  		//负载CV模式控制电压参数  				
	vu32 CSET_LoadA_Offset_LOW;   		//负载CC模式控制电流参数  				
	vu32 CSET_LoadA_Offset_HIG;    		//负载CC模式控制电流参数  				
	
	vu32 CREG_POWERV_Offset;        	//电源CV模式电压测量参数       
	vu32 CREG_POWERA_Offset_CC;     	//电源CC模式电流测量参数      
	vu32 CREG_POWERA_Offset_CV;     	//电源CC模式电流测量参数       
	vu32 CSET_POWERV_Offset;	        //电源CV模式电压控制参数      
	vu32 CSET_POWERA_Offset;         	//电源CC模式电流控制参数         

	vu32 CREG_ReadR_Offset_LOW;      	//内阻测量系数  
	vu32 CREG_ReadR_Offset_HIG;      	//内阻测量系数       

	vu32 CREG_ReadR1_Offset;     		//内阻测量系数     
	vu32 CREG_ReadR2_Offset;     		//内阻测量系数         

	vu8 CPolar0;           				//负载电压测量和控制系数
	vu8 CPolar1;           				//负载电流测量和控制系数
	vu8 CPolar2;			  			//电源电压控制和测量系数
	vu8 CPolar3;			  			//电源电流测量和控制系数
	vu8 CPolar4;			  			//内阻测量高低档系数
	vu8 CPolar5;						//识别电阻测量系数			
	vu8 CPolar6;						//识别电阻测量系数		
	vu8 CPolar7;						//识别电阻测量系数	
	
}CorrectList;

typedef struct {
	float TestLV[2];//测量负载电压k
	float SetPV[1];//设置电源电压k
	float TestPV[1];//测量电源电压k
	
	float OffsetLV[2];//测量负载电压b
	float OffsetSetPV[1];//设置电源电压b
	float OffsetPV[1];//测量电源电压b
		
/////////////////////////////////////		
	float SetLC[5];//设置CC负载电流k
	float SetCV[2];//设置CV负载电压k
	float TestLC[5];//测量负载电流k
	float SetPC[1];//设置电源电流k
	float TestPC[1];//测量电源电流k
	
	float OffsetLC[5];//设置CC负载电流b
	float OffsetCV[2];//设置CV负载电压b
	float OffsetTestLC[5];//测量负载电流b
	float OffsetPC[1];//设置电源电流b
	float OffsetTestPC[1];//测量电源电流b
	
/////////////////////////////////////	
	float TestR[3];//内阻k
	float TestR1[1];//识别电阻1k
	float TestR2[1];//识别电阻2k
	
	float OffsetR[3];//内阻b
	float OffsetR1[1];//识别电阻1b
	float OffsetR2[1];//识别电阻2b
}CAL;

typedef struct {
	
	vu32 Conoff_Load;                	//ON/OFF
	vu32 Conoff_POWE;               	//ON/OFF  
	vu32 CSET_Voltage; 	    	   		//电源设定电压     
	vu32 CSET_Current;	    	   		//电源设定电流 		   
	vu32 CSET_Voltage_Laod; 			//负载设定电压
	vu32 CSET_Current_Laod;				//负载设定电流	   
	vu32 CLOAD_MODE;					//负载工作模式		    
	vu32 CLOAD_V_SW;					//负载电压档位		     
	vu32 CLOAD_I_SW;					//负载电流档位	       
	vu32 CRCONT_SW;						//内阻测量档位   	    

	vu32 CPOW_Voltage;	    	    	//电源测量电压     
	vu32 CVoltage;	    	        	//负载测量电压    
	vu32 CCurrent;    	    			//电源测量电流     
	vu32 CLaod_Current;	    			//负载测量电流       
	vu32 CR_VLUE;						//电池内阻值					 
	vu32 CR1_VLUE;						//识别电阻1						
	vu32 CR2_VLUE;						//识别电阻2							
	
	/* 内阻测试 */
	vu32 IR_Start_C;   			//起始电流     
	vu16 IR_Step_C;				//步进电流  
	vu32 IR_Threshold_V;		//门槛电压
	vu16 IR_Step_Time;			//步进时间
	vu8  ACT_DELAY;				//激活延时

	/* 程控电源 */
	vu16 POWER_OutPut_V;		//输出电压    
	vu16 POWER_Limit_C;			//限制电流
	vu16 POWER_Cutoff_C;;		//充电截止电流	

	/* 程控负载 */
	vu32 LOAD_C;				//负载电流
	vu16 LOAD_V;				//负载电压
	vu32 LOAD_P;				//负载电压
	vu16 LOAD_Cutoff_V;			//负载电压
	
	/* 充放电 */
	vu16 CDC_OutPut_V;          //输出电压     
	vu16 CDC_Limit_C;			//限制电流
	vu32 CDC_Ccutoff_V;			//截止电压
	vu16 CDC_Ccutoff_C;			//截止电流
	vu32 CDC_Load_C;			//负载电流    
	vu32 CDC_Dcutoff_V;			//截止电压  
	vu8  CDC_Cycle_Time;		//循环次数

	/* 系统设置 */
	vu8 SYS_Comparator;			//分选开关
	vu8 SYS_Overflow;			//过流测试 --- FLAG_OVTest 
	vu8 SYS_Beep;				//报警声音
	vu32 SYS_UL_V;				//电压上限
	vu32 SYS_LL_V;				//电压下限
	vu16 SYS_UL_II;				//内阻上限
	vu16 SYS_LL_IR;				//内阻下限
	vu16 SYS_UL_Overflow;		//过流上限
	vu16 SYS_LL_Overflow;		//过流下限

	vu8 SYS_BatteryType; 		//电池类型
	vu8 SYS_LANUGAGE;  			//语言 ---  FLAG_LANG 
	
	/* 系统信息 */
	vu16 SYS_SYEARS;
	vu8  SYS_SMONTHS;
	vu8	 SYS_SDAYS;
	vu32 SYS_SerialNO;
	
}SetPara;

typedef struct {
	/* 内阻测试 */
	vu16 IR_Start_C;   			//起始电流     
	vu16 IR_Step_C;				//步进电流 
	vu32 IR_Threshold_V;		//门槛电压
	vu8 IR_Step_Time;			//步进时间
	

	/* 程控电源 */
	vu16 POWER_OutPut_V;		//输出电压    
	vu16 POWER_Limit_C;			//限制电流
	vu16 POWER_Cutoff_C;		//截止电流 	

	/* 程控负载 */
	vu16 LOAD_C;				//负载电流
	vu16 LOAD_V;				//负载电流
	vu16 LOAD_Cutoff_V;			//截止电压

	/* 充放电 */
	vu16 CDC_OutPut_V;          //输出电压     
	vu16 CDC_Limit_C;			//限制电流
	vu32 CDC_Ccutoff_V;			//截止电压
	vu16 CDC_Ccutoff_C;			//截止电流
	vu16 CDC_Load_C;			//负载电流    
	vu32 CDC_Dcutoff_V;			//截止电压  
	vu8  CDC_Cycle_Time;		//循环次数
}SETPARA;
extern CorrectList CLists;
extern CAL CalPara;
extern SetPara Para; 
extern vu8 bootflag;
extern vu32 powsetdelay;

extern vu32 abc_time;      //计时器 -s
extern vu32 ms_time;       //计时器 -ms

extern vu8 FLAG_FBUTWIN;    //记录当前页面  为setup按键
extern vu8 FLAG_WIN;      	//界面标志  0/程控电源  1/内阻测试   2/程控负载   3/充放电   4/曲线  5/系统设置   6/系统信息
extern vu8 FLAG_LANG;  		//语言标记
extern vu8 FLAG_OVTest;  	//是否开启过流测试
extern vu8 FLAG_BEEP;   	//报警标记
extern vu8 FLAG_SORT;   	//分选开关标记
extern vu8 FLAG_BATTERY;  	//电池
extern vu8 FLAG_INPUTAUTH;  //输入权限标记  0/不可输入状态  1/可输入状态
extern char displayStr[20];  //输入的字符
extern vu8 FLAG_FIRSTWIN[8]; //判断加载页面的行为是否为断电重启状态
extern void Write_Flash_Bytes();    //写入 	
extern void Read_Flash_Bytes();	 //读取
extern void Write_Boot_Flag(void);
extern void Read_Boot_Flag(void);
extern vu8 POWER_RECHARGE;   //充电
extern vu8 C_DISCHARGE;       //放电
extern vu8 calflag;//校准标志位
extern vu8 shorttime;
extern float testv;
extern float CDC_CsumMah;         //每秒累加的容量
extern float CDC_DCsumMah;
extern struct MODS_T g_tModS;
extern vu8 trig_flag_state; 
extern u8 rmtrig[3];
extern u8 sendmodeflag;
extern vu8 mode_sw;
extern vu8 charge_step;
extern vu8 loop;
extern vu8 finishflag;
extern vu8 Rraly;
extern vu32 finishdelay;

extern float max_discahrgeNum;    
extern float old_I;  
extern vu8 off_state; 
extern float v_memory;
extern vu32 readpowflag;
void  Delay_ms(vu16 mstime);
void Delay_usS(vu16 ustime);
void OnOff_GPOI_ResetSet( vu8 _type, vu8 _value );
void Off_GPOI_ResetSet();
uint16_t CRC16(uint8_t *_pBuf, uint16_t _usLen);
void RecHandle(void);
void MODS_Poll(void);
void MODS_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen);
void MODE_PARASET(vu8 value);
void MODE_ONOFF(vu8 value);
void JumpBoot(u8 flag);
void SetPowerC(vu16 powc);
extern void ReadPowData(void);
extern u8 USART3_Recive_flg;
struct MODS_T
{
	uint8_t RxBuf[S_RX_BUF_SIZE];
	uint8_t TxBuf[S_TX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;
	uint8_t RspCode;
	
	uint8_t TxCount;
};
//============================================================================= 
#define Receive_BUFFERSIZE   10
//=============================================================================
#endif
/******************* (C) COPYRIGHT 2015 KUNKIN *****END OF FILE*************************/