/******************** (C) COPYRIGHT 2015 AVER********************
 * 文件名  ：KEY.C
 * 作者    ：李振
 * 描述    ：3*2矩阵键盘扫描
 * 内容    ：扫描完成后出键值
 * 硬件连接：一路3*2矩阵键盘  一路编码器独立按键
 * 修改日期：2015-08-18
********************************************************************/
#include "my_register.h"
#include "beep.h"
#include "flash.h"
#include "gpio.h"
#include "key.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include  "gui.h"
#include "MainTask.h"
#include "sysflag.h"

/*extern WM_HWIN CreateR(void);  				//内阻测试页面
extern WM_HWIN CreateWindowload(void);   	//程控负载界面
extern WM_HWIN CreateWindowMenu(void);   	//程控电源界面
extern WM_HWIN CreateWGraph(void);			//曲线界面
extern WM_HWIN CreateSysSetting(void);		//系统设置界面
extern WM_HWIN CreateCdCharge(void);		//充放电界面 
extern WM_HWIN CreateSysinfo(void);		//  */

extern void Write_Flash_Bytes();    //写入
extern void Read_Flash_Bytes();	 //读取

extern CorrectList CList;

extern WM_HWIN hWinWind; 
extern WM_HWIN hWinR;
extern WM_HWIN load_wind;
extern WM_HWIN GhWin;
extern WM_HWIN sysSettingWin;
extern WM_HWIN cdchargeWin;  
extern WM_HWIN sysinfoWin;

//=================================================================
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
} flagA, flagB,flagC,flagD,flagE,flagF,flagG;
//====================================================================

vu8 t_MODE;

#define MAX_VOL  3200		
#define MAX_CUR  5100	
vu8 t_KeyScan;
vu8 NewKey;
vu8 t_wei;
vu8 t_beep;
vu8 t_Str;
vu8 t_lock;
vu8 t_onoff;
vu8 t_doub;
vu8 t_key_step;//閿洏鎵弿璁℃暟鍣?
vu16 Key1,Key2,Key3,Key4,Key5,Key6,Key7,Key8;
vu32 Key9;
vu16 KeyCounter;
vu16 MAX_limit;//自动测试上限值
vu16 MIN_limit;//自动测试下限值
vu16 Test_Time;
vu16 Test_Time1;
vu16 Time_sw;//定时
vu16 Test_Daley;//自动测试判断等待时间，此时间用户可设置
vu16 Test_C_Door;//自动测试门槛电流
vu8 OFFSET_Zore;//零点值

vu8 FLAG_WIN;   //页面标记
vu8 FLAG_LANG;  //语言标记
vu8 FIRST_LOAD;  //是否第一次加载页面
vu8 FLAG_OVTest;     //过流测试
vu8 FLAG_INPUTAUTH;  //输入状态
vu8 FLAG_FBUTWIN;    //标记上一个界面




vu32 Key_Scan(void);
void Lift_Move(void);
void Right_Move(void);
void Setvalue_Add(void);


/* 键值定义 */
#define        KEY_1                		 0X4E     //
#define        KEY_2                		 0X56     //
#define        KEY_3                		 0X5E     //
#define        KEY_4                		 0X57      //
#define        KEY_5                		 0X5F      //
#define        KEY_6                		 0X76      //
#define        KEY_7                		 0X67     //
#define        KEY_8                		 0X6F      //
#define        KEY_9                		 0X77     //
#define        KEY_0                		 0X5D      //
#define        KEY_dian              		 0X65     //
#define        KEY_ESC              		 0X6D     //

#define        ENTER                     0X4D    //
#define        PUSH_Lift                 0X45     //
#define        PUSH_Right                0X55      //
#define        PUSH_Up                   0X46      //
#define        PUSH_Down                 0X74      //
#define        PUSH_DISP                 0X47      //
#define        PUSH_SETUP                0X4F     //

#define        KEY_Lock                  0X75     //
#define        KEY_BIAS                  0X6E      //
#define        KEY_Reset                 0X66      //
#define        KEY_TRIG                  0X75      //

#define        KEY_Powe                  0X6C     //
#define        KEY_Face1                 0X64      //
#define        KEY_Face2                 0X5c      //
#define        KEY_Face3                 0X54      //
#define        KEY_Face4                 0X4c      //
#define        KEY_Face5                 0X44      //

/* 定义按键动作 */
#define        KEY_NULL_VALUE             0X00    //无动作
#define        KEY_PRESS                  0X10    //单击
#define        KEY_LONG                   0X20    //长按
#define        KEY_CONTINUE               0X40    //连击
#define        KEY_UP                     0X80    //按键抬起
/* 定义按键处理步骤 */
#define        KEY_INIT_STATE               0      //初态
#define        KEY_WOBBLE_STATE             1      //消抖
#define        KEY_PRESS_STATE              2      //触发单键
#define        KEY_LONG_STATE               3      //长按触发
#define        KEY_CONTINUE_STATE           4      //连续触发
#define        KEY_RELEASE_STATE            5      //恢复初态
/* 长按时间 */
#define         KEY_LONG_PERIOD             50                /* 长按时间1S */
#define        KEY_CONTINUE_PERIOD          10                /* 双击时间500ms */

/************************************************************************************************************************/
vu32 Key_Scan(void)
{
	static vu8 KeyState = KEY_INIT_STATE;
	static vu8 IsKeyRelease = 1;

	static vu8 KeyValueTemp = 0;
	vu8 KeyValue ;
	KeyValue= KEY_NULL_VALUE;
	
	if((NewKey>0x37)&&(IsKeyRelease==1))
	{
		if(KeyState == KEY_INIT_STATE)                    /*无按键按下*/
		{
			KeyState = KEY_WOBBLE_STATE;
			return KEY_NULL_VALUE;
		}
		else if(KeyState == KEY_WOBBLE_STATE)                /* 消抖 */
		{
			KeyState = KEY_PRESS_STATE;
			return KEY_NULL_VALUE;
		}
		else if(KeyState == KEY_PRESS_STATE)                /* 有按键按下，返回按键值 */
		{
			KeyValue = NewKey;
			KeyValueTemp = KeyValue;
			KeyState = KEY_CONTINUE_STATE;
			return KeyValue;
		}
		else if(KeyState == KEY_CONTINUE_STATE)           /*双击按键*/
		{
			KeyCounter ++;
			if(KeyCounter == KEY_CONTINUE_PERIOD)
			{
				 KeyCounter = 0;
				 KeyState = KEY_LONG_STATE;
			}
		}
		else if(KeyState == KEY_LONG_STATE)                        /* 长按按键 */
		{
			KeyCounter ++;
			if(KeyCounter == KEY_LONG_PERIOD)
			{
				KeyCounter = 0;
				Flag_Long=1; //长按触发标志位
			}
		}
	}
	else if(NewKey<=0x37)
	{
		KeyState = KEY_INIT_STATE;        /* 误触发，返回到初始状态 */
		IsKeyRelease = 1;
		Flag_Long=0;
	}
	return KEY_NULL_VALUE;
}
/***************************************************************************************************************************/
void Key_Funtion(void)
{
	vu32 KeyValue = 0;
	static vu8 t_KEYON;
	static vu8 a,b;
/****************************************************鐭寜鍔熻兘************************************************************/
	if(Flag_Key_Scan==1){
		
		Flag_Key_Scan = 0;
		
		KeyValue = Key_Scan();
		
		switch(KeyValue){
			
			case KEY_1 :{
	
				key_input_window("1");
				BEEP_Tiggr();
			}
			break;
			
			case KEY_2 :{
				
				key_input_window("2");
				BEEP_Tiggr();
			}
			break;
			
			case KEY_3 :{
				
				key_input_window("3");
				BEEP_Tiggr();
			}
			break;
			
			case KEY_4 :{
				
				key_input_window("4");
				BEEP_Tiggr();
			}
			break;
			
			case KEY_5 :{
				
				key_input_window("5");	
				BEEP_Tiggr();
			}
			break;	
			
			case KEY_6 :{
				
				key_input_window("6");	
				BEEP_Tiggr();
			}
			break;
			
			case KEY_7 :{
				
				key_input_window("7");	
				BEEP_Tiggr();
			}
			break;
			
			case KEY_8 :{
				
				key_input_window("8");	
				BEEP_Tiggr();
			}
			break;
			
			case KEY_9 :{
				
				key_input_window("9");
				BEEP_Tiggr();
			}
			break;
			
			case KEY_0 :{
				
				key_input_window("0");
				BEEP_Tiggr();
			}
			break;
			
			case KEY_dian :{
				
				key_input_window(".");
				BEEP_Tiggr();
			}
			break;
			
			case KEY_ESC :  // 退格
			{
				key_delete_input();
				BEEP_Tiggr();
				
			}
			break;
			
			case PUSH_Lift :  {		
			
				key_down_window( WIN_KEYLEFT );	
				BEEP_Tiggr();
			}
			break;
			case PUSH_Right :{	
			
				key_down_window( WIN_KEYRIGHT );
				BEEP_Tiggr();
			}
			break;
			case PUSH_Up :{		
			
				key_down_window( WIN_KEYUP );
				BEEP_Tiggr();				
			}
			break;
			case PUSH_Down :{
				
				key_down_window( WIN_KEYDOWN );	
				BEEP_Tiggr();
			}
			break;
			
			case ENTER :  //确认键
			{
				ent_input_win();
				BEEP_Tiggr();
			}
			break;

			case KEY_BIAS:{   //空白键
			
				if( FLAG_INPUTAUTH != 1 && FLAG_WIN != 6 ){
					
					delteWindow();
					
					Off_GPOI_ResetSet();
					
					FLAG_FBUTWIN = 6;
					
					FLAG_WIN = 6;
					
					CreateSysinfo();
				}
				BEEP_Tiggr();
			}
			break;
			case PUSH_DISP :
			{
				if( FLAG_INPUTAUTH != 1 && FLAG_WIN == 5){
					
					delteWindow(); 
					
					Off_GPOI_ResetSet();
					
					switch( FLAG_FBUTWIN ){						
						case 0 : 
							
							FLAG_WIN = 0 ;	
							CreateWindowMenu();  	
						break;
						case 1 : 
					
							FLAG_WIN = 1 ;	
							CreateR();  			
						break;
						case 2 :
						 
							FLAG_WIN = 2 ; 
							CreateWindowload(); 	
						break;
						case 3 :
						
							FLAG_WIN = 3 ;
							CreateCdCharge();  		
						break;
						case 6 :
						
							FLAG_WIN = 6 ;
							CreateSysinfo();  		
						break;
						
					}
					
				}
				BEEP_Tiggr();
			}
			break;
			 case PUSH_SETUP :
			{
				if( FLAG_INPUTAUTH != 1 && FLAG_WIN != 5 ){
					delteWindow();
					Off_GPOI_ResetSet();
					FLAG_WIN = 5;
					CreateSysSetting();
				}
				BEEP_Tiggr();
			}
			break;
			
			case KEY_TRIG:
			{
				if( FLAG_INPUTAUTH != 1 ){
					switch( FLAG_WIN ){
						
						case 0:{         //程控电源
							
							POWER_RECHARGE = (POWER_RECHARGE == 1) ? 0 : 1;			
							trig_power_charge();	
						}
						
						break;
						
						case 2: {        //程控负载
						
							C_DISCHARGE = (C_DISCHARGE == 1) ? 0 : 1;
							trig_load_discharge();
						}	
						
						break;
						
						case 1:	{
							
							trig_R_charge();
						}
						break;			
						case 3:	{
							
							trig_Cdc_charge();
						}
						
						case 4:
						case 5:
							
						break;
						
					}
				}
				BEEP_Tiggr();
			} 
			break;
			case KEY_Reset://稳压电源ON/OFF
			{
				 switch(FLAG_WIN){
						case 0:

							if(LOAD_V_SW == 0){
							
								LOAD_V_SW = 1;

							}else{
								LOAD_V_SW = 0;
							}

						break;

						case 1 :

							if(RCONT_SW == 0){

								RCONT_SW = 1;
							}else{
								RCONT_SW = 0;
							}

						break;

						case 2:

							if(LOAD_I_SW == 0){	

								LOAD_I_SW = 1;//自动切换为高档位

							}else{
								LOAD_I_SW = 0;
							}
						  break;
				 }
			}
			break; 
			case KEY_Powe :
			{
				
				

			}
			break;
			case KEY_Face1 :
			{
				if( FLAG_INPUTAUTH != 1 && FLAG_WIN != 1  ){
					delteWindow();
					Off_GPOI_ResetSet();
					FLAG_FBUTWIN = 1;
					FLAG_WIN = 1;
					CreateR();
				}
				BEEP_Tiggr();
			}
			break;
			case KEY_Face2 :
			{
				if( FLAG_INPUTAUTH != 1 && FLAG_WIN != 0  ){
					delteWindow();
					Off_GPOI_ResetSet();
					FLAG_FBUTWIN = 0;
					FLAG_WIN = 0;
					CreateWindowMenu();
				}
				BEEP_Tiggr();
			}
			break;
			case KEY_Face3 :
			{
				if( FLAG_INPUTAUTH != 1 && FLAG_WIN != 2  ){
					delteWindow();
					Off_GPOI_ResetSet();
					FLAG_FBUTWIN = 2;
					FLAG_WIN = 2;
					CreateWindowload();
				}
				BEEP_Tiggr();
			}
			break;
			case KEY_Face4 :  
			{	
			
				if ( FLAG_INPUTAUTH != 1 && FLAG_WIN != 3 ){
					delteWindow();
					Off_GPOI_ResetSet();
					FLAG_FBUTWIN = 3;
					FLAG_WIN = 3;
					CreateCdCharge();
				}
				BEEP_Tiggr();
			}
			break;
			case KEY_Face5 :
			{
				if( FLAG_INPUTAUTH != 1 && FLAG_WIN != 5 ){
					delteWindow();
					Off_GPOI_ResetSet();
					FLAG_WIN = 5;
					CreateSysSetting();
				}
				BEEP_Tiggr();
			}
			break;
			default:
			break;
		}	
	}
}

//删除窗口
void delteWindow(void){
	
	trig_flag_state = 0;
	
	WM_DeleteWindow(hWinR);
	
	WM_DeleteWindow(hWinWind);
	
	WM_DeleteWindow(cdchargeWin);
	
	WM_DeleteWindow(load_wind);
	
	WM_DeleteWindow(GhWin);
	
	WM_DeleteWindow(sysSettingWin);
	
	WM_DeleteWindow(sysinfoWin);
	
}

/*   按键输入 ----  上下左右    */
void key_down_window( vu8 direction )
{
	
	if	( FLAG_INPUTAUTH == 1 ) 
	{
		switch( FLAG_WIN )
		{			
			case 0:	KEY_DOWN_POWER(direction); break;
			
			case 1: KEY_DOWN_IRT(direction); break;
			
			case 2: KEY_DOWN_LOAD(direction); break;
			
			case 3: KEY_DOWN_CDC(direction); break;
			
			case 4: KEY_DOWN_GRAPH(direction); break;
			
			case 5: KEY_DOWN_SYSSET(direction); break;
			
			case 6: KEY_DOWN_SYSINFO(direction); break;
		}
	}
	
}

/*   按键输入 ----   数字    */
void key_input_window(char *strx)
{
	if	( FLAG_INPUTAUTH == 1 ) 
	{	
		switch( FLAG_WIN )
		{
					
			case 0:	KEY_POWER_INPUT(strx); 	break;
			
			case 1: KEY_IRT_INPUT(strx);	break;
			
			case 2: KEY_LOAD_INPUT(strx);	break;
			
			case 3: KEY_CDC_INPUT(strx);	break;
			
			case 4: KEY_GRAPH_INPUT(strx);	break;
			
			case 5: KEY_SYSSET_INPUT(strx);	break;
			
			case 6: KEY_SYSINFO_INPUT(strx);	break;
		}
	}
}

// 确认键
void ent_input_win(){
	
	switch( FLAG_WIN ){		
	
		case 0:	Power_ent(); 	break;
		
		case 1: R_ent();		break;
		
		case 2: Load_ent();		break;
		
		case 3: Cdc_ent();		break;
		
		case 4: Graph_ent();	break;
		
		case 5: Sysset_ent();	break;
		
		case 6: Sysinfo_ent();	break;
		
	}
}

//删除输入字符
void key_delete_input(){
	
	switch( FLAG_WIN ){
		
		case 0:	Power_delete_input(); 	break;
		
		case 1: R_delete_input();		break;
		
		case 2: Load_delete_input();	break;
		
		case 3: Cdc_delete_input();		break;
		
		case 4: Graph_delete_input();	break;
		
		case 5: Sysset_delete_input();	break;
		
		case 6: Sysinfo_delete_input();	break;
	}
}











