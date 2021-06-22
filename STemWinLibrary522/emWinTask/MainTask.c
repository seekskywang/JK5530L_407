/******************************************************************/
/* åç§°ï¼š                                                  				*/
/* æ•ˆæœï¼š                                                        */
/* å†…å®¹ï¼šå†…é˜»ä»ªç•Œé¢                                              */
/* ä½œè€…ï¼šzhan                                                  */
/* è”ç³»æ–¹å¼QQ:363116119                                        */

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

extern WM_HWIN CreateR(void);  				//å†…é˜»æµ‹è¯•é¡µé¢
extern WM_HWIN CreateWindowload(void);   	//ç¨‹æ§è´Ÿè½½ç•Œé¢
extern WM_HWIN CreateWindowMenu(void);   	//ç¨‹æ§ç”µæºç•Œé¢
extern WM_HWIN CreateWGraph(void);			//æ›²çº¿ç•Œé¢
extern WM_HWIN CreateSysSetting(void);		//ç³»ç»Ÿè®¾ç½®ç•Œé¢
extern WM_HWIN CreateCdCharge(void);		//å……æ”¾ç”µç•Œé¢


/**********************************************************************************************************
*	º¯ Êı Ãû: MainTask
*	¹¦ÄÜËµÃ÷: GUIÖ÷º¯Êı
*	ĞÎ    ²Î£ºÎŞ
*	·µ »Ø Öµ: ÎŞ
*********************************************************************************************************
*/
void MainTask(void) 
{ 
	unsigned char  ucKeyCode;
	vu8 i = 0;
	GUI_Init();
	WM_SetDesktopColor(GUI_BLUE);  
	GUI_Clear();//ÇåÆÁ
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

	FLAG_WIN = 1;  //é¡µé¢æ ‡å¿—ä½
	FLAG_FBUTWIN = 1;
    CDC_CsumMah = 0;   //ç”µæ± å®¹é‡

	if( CLists.SYS_LANUGAGE == 0xFF ) FLAG_LANG = 0;
	else FLAG_LANG = CLists.SYS_LANUGAGE; 
	
	if( CLists.SYS_Overflow == 0xFF )	FLAG_OVTest = 1;
	else	FLAG_OVTest = CLists.SYS_Overflow;

	FLAG_INPUTAUTH = 0;
	for( i = 0; i < 8; i++){
		FLAG_FIRSTWIN[i] = 1;
	}
  

	CreateR();//¿ª»ú½øÈëÄÚ×è²âÊÔ½çÃæ
	LOAD_MODE = 0;//¸ºÔØCCÄ£Ê½
	LOAD_V_SW = 1;
	LOAD_I_SW = 0;
	GPIO_SetBits(GPIOC,GPIO_Pin_1);//OFF
	Flag_Swtich_ON = 0;
    flag_CC_MODE = 1;

	while (1)
	{
	
		TIM_SetCompare3(TIM4,Contr_Voltage);//ÎÈÑ¹µçÔ´µçÁ÷DAC
		TIM_SetCompare4(TIM4,Contr_Current);//ÎÈÑ¹µçÔ´µçÑ¹DAC
		AD5541_Send(Contr_Laod);//¼ÓÔØDACÖµ
		MODE_CONT();//µçÑ¹µçÁ÷µµÎ»ÇĞ»»
		if(Flag_DAC_OFF==0)
		{
			Transformation_ADC();
		}
		if(Flag_ADC_Full==1)
		{
			Flag_ADC_Full=0;
			ADC_CH_Scan();//ADCÂË²¨
		}
		AD7689_Scan_CH();//¶ÁÈ¡AD7689¸÷Í¨µÀADÖµ
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
		Key_Funtion();//¼üÅÌ´¦Àí
		Temp_Comapre();//·çÉÈ¿ØÖÆ
		BEEP_Ring();//·äÃùÆ÷
		GUI_Delay(2);//GUIË¢ĞÂ
	}
  
}
/***************************** AVER(END OF FILE) *********************************/
