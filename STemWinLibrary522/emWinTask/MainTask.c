/******************************************************************/
/* 名称：                                                  				*/
/* 效果：                                                        */
/* 内容：内阻仪界面                                              */
/* 作者：zhan                                                  */
/* 联系方式QQ:363116119                                        */

#include "MainTask.h"
#include  "gui.h"
#include  "adc.h"
#include  "tim4.h"
#include  "key.h"
#include  "beep.h"
#include "my_register.h"
#include "stm32f4xx_dac.h"
#include "modbus.h"
#include "ssd1963.h"
#include "tm1650.h"
#include "ad5541.h"
#include "ad7689.h"
#include "cont.h"
#include "sysflag.h"

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

extern WM_HWIN CreateR(void);  				//内阻测试页面
extern WM_HWIN CreateWindowload(void);   	//程控负载界面
extern WM_HWIN CreateWindowMenu(void);   	//程控电源界面
extern WM_HWIN CreateWGraph(void);			//曲线界面
extern WM_HWIN CreateSysSetting(void);		//系统设置界面
extern WM_HWIN CreateCdCharge(void);		//充放电界面


/**********************************************************************************************************
*	�� �� ��: MainTask
*	����˵��: GUI������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MainTask(void) 
{ 
	unsigned char  ucKeyCode;
	vu8 i = 0;
	GUI_Init();
	WM_SetDesktopColor(GUI_BLUE);  
	GUI_Clear();//����
	WM_SetCreateFlags(WM_CF_MEMDEV);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);

	FLAG_WIN = 1;  //页面标志位
	FLAG_FBUTWIN = 1;
    CDC_CsumMah = 0;   //电池容量

	if( CLists.SYS_LANUGAGE == 0xFF ) FLAG_LANG = 0;
	else FLAG_LANG = CLists.SYS_LANUGAGE; 
	
	if( CLists.SYS_Overflow == 0xFF )	FLAG_OVTest = 1;
	else	FLAG_OVTest = CLists.SYS_Overflow;

	FLAG_INPUTAUTH = 0;
	for( i = 0; i < 8; i++){
		FLAG_FIRSTWIN[i] = 1;
	}
  

	CreateR();//��������������Խ���
	LOAD_MODE = 0;//����CCģʽ
	LOAD_V_SW = 1;
	LOAD_I_SW = 0;
	GPIO_SetBits(GPIOC,GPIO_Pin_1);//OFF
	Flag_Swtich_ON = 0;
    flag_CC_MODE = 1;

	while (1)
	{
	
		TIM_SetCompare3(TIM4,Contr_Voltage);//��ѹ��Դ����DAC
		TIM_SetCompare4(TIM4,Contr_Current);//��ѹ��Դ��ѹDAC
		AD5541_Send(Contr_Laod);//����DACֵ
		MODE_CONT();//��ѹ������λ�л�
		if(Flag_DAC_OFF==0)
		{
			Transformation_ADC();
		}
		if(Flag_ADC_Full==1)
		{
			Flag_ADC_Full=0;
			ADC_CH_Scan();//ADC�˲�
		}
		AD7689_Scan_CH();//��ȡAD7689��ͨ��ADֵ
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			UART_Action();
		}
		if(Flag_Key_Scan==1)
		{
			Flag_Key_Scan=0;
			NewKey=TM1650_Read_KEY();
			Flag_Key_Scan = 1;
		}
		Key_Funtion();//���̴���
		Temp_Comapre();//���ȿ���
		BEEP_Ring();//������
		GUI_Delay(2);//GUIˢ��
	}
  
}
/***************************** AVER(END OF FILE) *********************************/
