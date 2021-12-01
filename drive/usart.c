/******************************************************************/
/* ���ƣHuasrt                                           */
/* Ч����                                                        */
/* ���ݣ�                                                        */
/* ���ߣ�����                                                    */
/* ��ϵ��ʽ��QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "string.h"
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
vu8 UART_Buffer_Rece[16];
vu8 UART_Buffer_Send[20];
vu8 UART_Buffer_Size;
vu8 Transmit_BUFFERsize;
vu8 t_USART;
vu8 MODE;
vu8 mainswitch;
vu8 Rraly;
u16 USART_RX_STA=0;       //����״̬���
u16 USART_RX_CNT=0;       //���ռ���
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
extern vu16 ADJ_Write;//У׼ʱ��д���ʵ��ֵ
extern vu8 DAC_Flag;//DAC�Ƿ���ر�־
extern vu32 Modify_A_READ;
extern vu32 Modify_C_READ;
extern vu32 Modify_A_ACT;
	
extern vu32 Modify_B_READ;
extern vu32 Modify_D_READ;
extern vu32 Modify_B_ACT;
extern vu32 Correct_Parametet[17];//У׼����
extern vu32 Correct_Strong[17];//У׼ϵ��
extern vu8  correct_por[8];
uint32_t x1,y1,x2,y2,x3,y3,x4,y4;
vu32 powsetdelay;
vu8 sendwait;
vu8 listsend;
vu32 watchtest;
void SetPowerC(vu16 powc);
void ReadPowData(void);
void PowerSwitch(vu8 bit);
u8    USART3_Recive_leng = 0;                           //����1���ݽ��ճ���
u8    USART3_Recive_flg  = 0;                           //����1���ݽ�����ɱ�ʶ
u8 MasterSendbuf[MAXRxTxLen];
u8 MasterRecbuf[MAXRxTxLen];
u8 listocwatch,listpowwatch1,listpowwatch2;
char CmdStr[CmdNumb][CmdLen] =
{
	{"STATUS_\0"},        //״̬��ȡ
	{"MOD_\0"},           //ģʽ�л�   
	{"TRIG_\0"},           //�򿪹ر�
	{"SETPARA_\0"},           //ģʽ�л�    1���ַ�  
	{"SETLIST_\0"},           //��ȡCS1237���������� 
	{"OPRESET_\0"},          //�����ʱ�־
	{"POWI_\0"},            //����LM5116 1���ַ� 
	{"ADCV_\0"},          //ADC��ѹ
	{"LOWI_\0"},          //������ʱ���� 1���ַ� 
	{"HORL_\0"},          //�иߵ͵�λ  1���ַ� 
	{"LVL_\0"},           //DAC8562��� A· 0~5V
	{"SWITCH_\0"},        //�ܿ���     1���ַ� 
	{"CALV_\0"},        //13��ѹУ׼     1���ַ� 
	{"CALI_\0"},        //14����У׼     1���ַ� 
	{"CALR_\0"},        //15����У׼     1���ַ� 
	{"DAC_\0"},        //16DAC
	{"OVER_\0"},        //17�����������
	{"TEMP_\0"},        //18�¶�
	{"BOOT_\0"},        //19�¶�
};


/*****************************************************************/
unsigned short IsDigitChar(unsigned char a)             //���a��ֵ�����ֵĻ�����1�����򷵻�0
{
	int b;
  if(a<0x30 || a>0x39)                     //����1-9
  	{
  	   b=0;
  	}
  else
  	{
  	   b=1;
  	}
  return b;
}
void SetPowerV(vu16 powv)
{
	MasterSendbuf[0] = 0x01;
	MasterSendbuf[1] = 0x06;
	MasterSendbuf[2] = 0x00;
	MasterSendbuf[3] = 0x30;
	MasterSendbuf[4] = (u8)(powv/10>>8);
	MasterSendbuf[5] = (u8)(powv/10);
	MasterSendbuf[6] = (u8)(Hardware_CRC(MasterSendbuf,6));
	MasterSendbuf[7] = (u8)(Hardware_CRC(MasterSendbuf,6)>>8);	
	USART3WriteDMA(8);
}

void SetPowerC(vu16 powc)
{
	MasterSendbuf[0] = 0x01;
	MasterSendbuf[1] = 0x06;
	MasterSendbuf[2] = 0x00;
	MasterSendbuf[3] = 0x31;
	MasterSendbuf[4] = (u8)(powc/10>>8);
	MasterSendbuf[5] = (u8)(powc/10);
	MasterSendbuf[6] = (u8)(Hardware_CRC(MasterSendbuf,6));
	MasterSendbuf[7] = (u8)(Hardware_CRC(MasterSendbuf,6)>>8);	
	USART3WriteDMA(8);
}

void SetPowerModeV(void)
{
	SetPowerV(Para.POWER_OutPut_V);
	sendwait = 2;
}

void SetPowerModeC(void)
{
	SetPowerC(Para.POWER_Limit_C);
	sendwait = 3;
}

void SetPowerModeS(void)
{
	if(mainswitch == 1)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_2);
		Delay_ms(POWER_SW_DELAY);
		PowerSwitch(mainswitch);
	}else if(mainswitch == 0)
	{
		PowerSwitch(mainswitch);
		Delay_ms(POWER_SW_DELAY);
		GPIO_ResetBits(GPIOE,GPIO_Pin_2); //�رյ�Դ����̵���
	}
	
	sendwait = 0;
}
void SetListPV(void)
{
	SetPowerV(Para.CDC_OutPut_V);
	listsend = 2;
}

void SetListPC(void)
{
	SetPowerC(Para.CDC_Limit_C);
	listsend = 3;
}


void SetListPS(void)
{
	if(mainswitch == 1)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_2);
		Delay_ms(POWER_SW_DELAY);
		PowerSwitch(mainswitch);
	}
	
	if(mainswitch == 0)
	{
		PowerSwitch(mainswitch);
		Delay_ms(POWER_SW_DELAY);
		GPIO_ResetBits(GPIOE,GPIO_Pin_2); //�رյ�Դ����̵���
	}
	
	listsend = 0;
}

void PowerSwitch(vu8 bit)
{
	MasterSendbuf[0] = 0x01;
	MasterSendbuf[1] = 0x06;
	MasterSendbuf[2] = 0x00;
	MasterSendbuf[3] = 0x01;
	MasterSendbuf[4] = (u8)(bit>>8);
	MasterSendbuf[5] = (u8)(bit);
	MasterSendbuf[6] = (u8)(Hardware_CRC(MasterSendbuf,6));
	MasterSendbuf[7] = (u8)(Hardware_CRC(MasterSendbuf,6)>>8);	
	USART3WriteDMA(8);
}

void ReadPowData(void)
{
	MasterSendbuf[0] = 0x01;
	MasterSendbuf[1] = 0x03;
	MasterSendbuf[2] = 0x00;
	MasterSendbuf[3] = 0x10;
	MasterSendbuf[4] = 0x00;
	MasterSendbuf[5] = 0x02;
	MasterSendbuf[6] = (u8)(Hardware_CRC(MasterSendbuf,6));
	MasterSendbuf[7] = (u8)(Hardware_CRC(MasterSendbuf,6)>>8);	
	USART3WriteDMA(8);
}

void USART3HANDLE(void)
{
	vu16 crcres;

	if(MasterRecbuf[0] == 0x01)
	{
		if(MasterRecbuf[1] == 0x03)
		{
			crcres = (MasterRecbuf[8] << 8) + MasterRecbuf[7];
			if(crcres == Hardware_CRC(MasterRecbuf,7))
			{
				 
				Para.CPOW_Voltage = (MasterRecbuf[3] << 8) + MasterRecbuf[4];
				Para.CCurrent = (MasterRecbuf[5] << 8) + MasterRecbuf[6];
				memset(MasterRecbuf,0,sizeof(MasterRecbuf));
			}
				if(sendwait == 3)
				{
					SetPowerModeS();
				}
				if(sendwait == 2)
				{
					SetPowerModeC();
				}
				if(sendwait == 1)
				{
					SetPowerModeV();
				}
				if(listsend == 3)
				{
					SetListPS();
				}
				if(listsend == 2)
				{
					SetListPC();
				}
				if(listsend == 1)
				{
					SetListPV();
				}
		}else if(MasterRecbuf[1] == 0x06){
			memset(MasterRecbuf,0,sizeof(MasterRecbuf));
			watchtest ++;
		}
	}	

	
	USART3_Recive_flg = 0;
}

void MODE_ONOFF(vu8 value)
{
	switch(MODE)
	{
		case 0://���Ӹ���ON/OFF
		{
			if( value == 0 ){
				
				Flag_Swtich_ON = 0;
				GPIO_SetBits(GPIOC,GPIO_Pin_7);//OFF	  	
			}
			else if( value == 1 ){
				
				Flag_Swtich_ON = 1;
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);//On
			}
		}break;
		case 1://��ѹ��ԴON/OFF
		{
			sendwait = 1;
//			if( value == 0 ){
//				sendwait = 1;
////				PowerSwitch(0);//�رյ�Դ���
////				Delay_ms(1000);
////				GPIO_ResetBits(GPIOE,GPIO_Pin_2); //�رյ�Դ����̵���
////				GPIO_ResetBits(GPIOC,GPIO_Pin_1); //�رյ�Դ���
//////				Delay_ms(1000);
////				Para.CSET_Voltage = 0;
////				Para.CSET_Current = 0;
//////				Delay_ms(1000);
////				GPIO_ResetBits(GPIOE,GPIO_Pin_2); //�رյ�Դ����̵���
//			}
//			else if( value == 1 ){
////				GPIO_SetBits(GPIOE,GPIO_Pin_2);   //�򿪵�Դ����̵���
//////				Delay_ms(1000);
////				PowerSwitch(1);//�򿪵�Դ���
//				
//////				
////				Para.CSET_Voltage = Para.POWER_OutPut_V;
////				Para.CSET_Current = Para.POWER_Limit_C;
//////				Delay_ms(1000);
////				GPIO_SetBits(GPIOC,GPIO_Pin_1);   //�򿪵�Դ���
//				
//			}
		}break;
		case 2://��������ON/OFF
		{
			if( value == 0 ){
				
				Off_GPOI_ResetSet();
				trig_flag_state = 0;
				max_discahrgeNum = 0;
				Para.CSET_Current_Laod =  Para.IR_Start_C;
			}
			else if( value == 1 ){
				LOAD_MODE = 0;
				GPIO_ResetBits(GPIOC,GPIO_Pin_12);//CCģʽ
				Para.CSET_Current_Laod =  Para.IR_Start_C;
				testv = DISS_Voltage;
				Para.CSET_Voltage = (int)(DISS_Voltage*1000) + 1000;
				Para.CSET_Current = 1000;
				shorttime = 0;
				old_I = 0;
				max_discahrgeNum = 0;
				trig_flag_state = 1;
				C_DISCHARGE = 1;        	
				Flag_Swtich_ON = 1;
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);//On 
				max_discahrgeNum = (float)Para.IR_Start_C / 1000;
			}
		}break;
	}
}

void LIST_ONOFF(vu8 value)
{
	switch(MODE)
	{
		case 0://���Ӹ���ON/OFF
		{
			LOAD_MODE=0;
			GPIO_ResetBits(GPIOC,GPIO_Pin_12);//CCģʽ
			if( value == 0 ){
				
				Flag_Swtich_ON = 0;
				GPIO_SetBits(GPIOC,GPIO_Pin_7);//OFF	  	
			}
			else if( value == 1 ){
				
				Flag_Swtich_ON = 1;
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);//On
			}
		}break;
		case 1://��ѹ��ԴON/OFF
		{
			if( value == 0 ){
				listsend = 3;
				listpowwatch1 ++;
//				PowerSwitch(0);
//				GPIO_ResetBits(GPIOC,GPIO_Pin_1); //�رյ�Դ���
//				Para.CSET_Voltage = 0;
//				Para.CSET_Current = 0;
//				GPIO_ResetBits(GPIOE,GPIO_Pin_2); //�رյ�Դ����̵���
			}
			else if( value == 1 ){
				listsend = 1;
				listpowwatch2 ++;
//				PowerSwitch(1);
//				Para.CSET_Voltage = Para.CDC_OutPut_V;
//				Para.CSET_Current = Para.CDC_Limit_C;
//				GPIO_SetBits(GPIOE,GPIO_Pin_2);   //�򿪵�Դ����̵���
//				GPIO_SetBits(GPIOC,GPIO_Pin_1);   //�򿪵�Դ���
			}
		}break;
		case 2://��������ON/OFF
		{
			listocwatch++;
			if( value == 0 ){
				
				Off_GPOI_ResetSet();
				trig_flag_state = 0;
				max_discahrgeNum = 0;
				Para.CSET_Current_Laod =  Para.IR_Start_C;
			}
			else if( value == 1 ){
				LOAD_I_SW = 1;
				testv = DISS_Voltage;
				Para.CSET_Voltage = (int)(DISS_Voltage*1000) + 1000;
				Para.CSET_Current = 1000;
				Para.CSET_Current_Laod =  Para.IR_Start_C;
				shorttime = 0;
				old_I = 0;
				max_discahrgeNum = 0;
				trig_flag_state = 1;
				C_DISCHARGE = 1;        	
				Flag_Swtich_ON = 1;
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);//On 
				max_discahrgeNum = (float)Para.IR_Start_C / 1000;				
//				testv = DISS_Voltage;
//				Para.CSET_Voltage = (int)(DISS_Voltage*1000) + 1000;
//				Para.CSET_Current = 1000;
//				Para.CSET_Current_Laod = Para.IR_Start_C;
//				shorttime = 0;
//				old_I = 0;
//				max_discahrgeNum = 0;
//				trig_flag_state = 1;
//				C_DISCHARGE = 1;
//				Flag_Swtich_ON = 1;
//				GPIO_ResetBits(GPIOC,GPIO_Pin_7);//On 
//				max_discahrgeNum = (float)Para.IR_Start_C / 1000;
			}
		}break;
		case 3://�������ON/OFF
		{
			if( value == 0 ){
				
				GPIO_ResetBits(GPIOC,GPIO_Pin_1); //�رյ�Դ���
				Para.CSET_Voltage = 0;
				Para.CSET_Current = 0;
				GPIO_ResetBits(GPIOE,GPIO_Pin_2); //�رյ�Դ����̵���
			}
			else if( value == 1 ){
				Para.CSET_Voltage = Para.CDC_OutPut_V;
				Para.CSET_Current = Para.CDC_Limit_C;
				GPIO_SetBits(GPIOE,GPIO_Pin_2);   //�򿪵�Դ����̵���
				GPIO_SetBits(GPIOC,GPIO_Pin_1);   //�򿪵�Դ���
			}
		}break;
		case 4://���Ӹ���ON/OFF
		{
			if( value == 0 ){
				
				Flag_Swtich_ON = 0;
				GPIO_SetBits(GPIOC,GPIO_Pin_7);//OFF	  	
			}
			else if( value == 1 ){
				
				Flag_Swtich_ON = 1;
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);//On
			}
		}break;
	}
}

void MODE_PARASET(vu8 value)
{
	switch(MODE)
	{
		case 0://���Ӹ���
		{
//			OnOff_GPOI_ResetSet( 2, 0 );
//			mainswitch = 0;
//			sendwait = 3;
			LOAD_MODE = Para.CLOAD_MODE;
			Para.CSET_Current_Laod = Para.LOAD_C;
			Para.CSET_Voltage_Laod = Para.LOAD_V;
			if(Para.CSET_Current_Laod > 20000)
			{
				LOAD_I_SW = 1;
			}else{
				LOAD_I_SW = 0;
			}
		}break;
		case 1://��ѹ��Դ
		{
//			OnOff_GPOI_ResetSet( 2, 0 );
//			mainswitch = 0;
//			if(USART3_Recive_flg == 1)
//			{
				sendwait = 1;
//			}else{
//				SetPowerMode();
//			}
//			Para.CSET_Voltage = Para.POWER_OutPut_V;
//			SetPowerV(Para.POWER_OutPut_V);
//	//			Para.CSET_Current = Para.POWER_Limit_C;
//			SetPowerC(Para.POWER_Limit_C);
		}break;
		case 2://�ŵ��������
		{
//			OnOff_GPOI_ResetSet( 2, 0 );
//			mainswitch = 0;
//			sendwait = 3;
			
			LOAD_MODE = 0;
			LOAD_I_SW = 1;
			Para.CSET_Current = 1000;
			Para.CSET_Current_Laod = Para.IR_Start_C;
		}break;
		case 3://�б�
		{
//			OnOff_GPOI_ResetSet( 2, 0 );
//			mainswitch = 0;
//			sendwait = 3;
			
			LOAD_MODE = 0;
			GPIO_ResetBits(GPIOC,GPIO_Pin_12);//CCģʽ
			LOAD_I_SW = 1;
//			Para.CSET_Current = 1000;
		}break;
		case 4://����������
		{
//			OnOff_GPOI_ResetSet( 2, 0 );
//			mainswitch = 0;
//			sendwait = 3;
			
			sendwait = 1;
		}break;
	}
}

//У׼����
void CalHandle(u8 mode,u8 range,u32 data)
{
	if(mode == 0)//������ѹ
	{
		if(range == 0)//�͵�λ�Ͷ�
		{
			calflag = 1;
			LOAD_V_SW = 0;
			x1 = Vmon_Load_value;
			y1 = data;
		}else if(range == 1){//�͵�λ�߶�
			
			x2 = Vmon_Load_value;
			y2 = data;
			CalPara.TestLV[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetLV[0] = (float)y2 - CalPara.TestLV[0]*(float)x2;
			
			Flag_DAC_OFF=0;
			calflag = 0;
		}else if(range == 2){//�ߵ�λ�Ͷ�
			calflag = 1;
			LOAD_V_SW = 1;
			x1 = Vmon_Load_value;
			y1 = data;
		}else if(range == 3){//�ߵ�λ�߶�
			
			x2 = Vmon_Load_value;
			y2 = data;
			CalPara.TestLV[1] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetLV[1] = (float)y2 - CalPara.TestLV[1]*(float)x2;
				//����д��FLASH
			Flag_DAC_OFF=0;
			calflag = 0;
		}else if(range == 4){//��Դ���Ƶ�ѹ�Ͳ���1
			calflag = 1;
			Contr_Voltage = 3500;
			Contr_Current = 5000;
			OnOff_GPOI_ResetSet(0,1);
		}else if(range == 5){//��Դ���Ƶ�ѹ�Ͳ���2
			x1 = data;
			y1 = Contr_Voltage;
			x3 = Vmon_POWE_value;
			y3 = data;
			Contr_Voltage = 10000;
			Contr_Current = 5000;
		}else if(range == 6){//��Դ���Ƶ�ѹ�Ͳ���3
			x2 = data;
			y2 = Contr_Voltage;
			x4 = Vmon_POWE_value;
			y4 = data;
			
			CalPara.SetPV[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetSetPV[0] = (float)y2 - CalPara.SetPV[0]*(float)x2;
			
			CalPara.TestPV[0] = ((float)y4 - (float)y3)/((float)x4 - (float)x3);
			CalPara.OffsetPV[0] = (float)y4 - CalPara.TestPV[0]*(float)x4;
			
			Flag_DAC_OFF=0;
			calflag = 0;
			Off_GPOI_ResetSet();
		}else if(range == 7){//CV���Ƶ͵�1
			LOAD_MODE = 1;
			LOAD_V_SW=0;
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//��ѹ��λΪ�͵�
			calflag = 1;
			Contr_Laod = 1000;
			OnOff_GPOI_ResetSet(2,1);
		}else if(range == 8){//CV���Ƶ͵�2
			LOAD_MODE = 1;
			LOAD_V_SW=0;
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//��ѹ��λΪ�͵�
			x1 = data;
			y1 = Contr_Laod;
			Contr_Laod = 8000;
//			OnOff_GPOI_ResetSet(2,1);
		}else if(range == 9){//CV���Ƶ͵�3
			x2 = data;
			y2 = Contr_Laod;
			CalPara.SetCV[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetCV[0] = (float)y2 - CalPara.SetCV[0]*(float)x2;
				//����д��FLASH
			Flag_DAC_OFF=0;
			calflag = 0;
			Off_GPOI_ResetSet();
		}else if(range == 10){//CV���Ƹߵ�1
			LOAD_MODE = 1;
			LOAD_V_SW=1;
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//��ѹ��λΪ�ߵ�
			calflag = 1;
			Contr_Laod = 1000;
			OnOff_GPOI_ResetSet(2,1);
		}else if(range == 11){//CV���Ƹߵ�2
			LOAD_MODE = 1;
			LOAD_V_SW=1;
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//��ѹ��λΪ�ߵ�
			x1 = data;
			y1 = Contr_Laod;
			Contr_Laod = 8000;
//			OnOff_GPOI_ResetSet(2,1);
		}else if(range == 12){//CV���Ƹߵ�3
			x2 = data;
			y2 = Contr_Laod;
			CalPara.SetCV[1] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetCV[1] = (float)y2 - CalPara.SetCV[1]*(float)x2;
				//����д��FLASH
			Flag_DAC_OFF=0;
			calflag = 0;
			Off_GPOI_ResetSet();
		}
	}else if(mode == 1){
		if(range == 0)//���ص����͵�λ1
		{		
			LOAD_I_SW = 0;
			Contr_Laod = 8000;
			calflag = 1;
			OnOff_GPOI_ResetSet(2,1);
		}
		else if(range == 1)//���ص����͵�λ2
		{
			LOAD_I_SW = 0;			
			x1 = data;
			y1 = Contr_Laod;
			x3 = Imon_Load_value;
			y3 = data;
			Contr_Laod = 56000;
		}else if(range == 2){//���ص����͵�λ3
			
			x2 = data;
			y2 = Contr_Laod;
			x4 = Imon_Load_value;
			y4 = data;
			
			CalPara.SetLC[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetLC[0] = (float)y2 - CalPara.SetLC[0]*(float)x2;
			
			CalPara.TestLC[0] = ((float)y4 - (float)y3)/((float)x4 - (float)x3);
			CalPara.OffsetTestLC[0] = (float)y4 - CalPara.TestLC[0]*(float)x4;
				
			Flag_DAC_OFF =0;
			calflag = 0;
			Off_GPOI_ResetSet();
		}else if(range == 3){//���ص����ߵ�λ1
			LOAD_I_SW = 1;
			GPIO_ResetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�ߵ�λ
			Contr_Laod = 6500;
			calflag = 1;
			OnOff_GPOI_ResetSet(2,1);
		}else if(range == 4){//���ص����ߵ�λ2
			
			LOAD_I_SW = 1;
			GPIO_ResetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�ߵ�λ
			x1 = data;
			y1 = Contr_Laod;
			x3 = Imon_Load_value;
			y3 = data;
			Contr_Laod = 27000;
		}else if(range == 5){//���ص����ߵ�λ3
			
			x2 = data;
			y2 = Contr_Laod;
			x4 = Imon_Load_value;
			y4 = data;
			
			CalPara.SetLC[1] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetLC[1] = (float)y2 - CalPara.SetLC[1]*(float)x2;
			
			CalPara.TestLC[1] = ((float)y4 - (float)y3)/((float)x4 - (float)x3);
			CalPara.OffsetTestLC[1] = (float)y4 - CalPara.TestLC[1]*(float)x4;
				
			Flag_DAC_OFF =0;
			calflag = 0;
			Off_GPOI_ResetSet();
		}else if(range == 6){//��Դ����1
			Contr_Voltage = 3500;
			Contr_Current = 4000;
			calflag = 1;
			OnOff_GPOI_ResetSet(0,1);
		}else if(range == 7){//��Դ����2
			
			x1 = data;
			y1 = Contr_Current;
			x3 = Imon_POWE_value;
			y3 = data;
			Contr_Voltage = 3500;
			Contr_Current = 14000;
		}else if(range == 8){//��Դ����3
			
			x2 = data;
			y2 = Contr_Current;
			x4 = Imon_POWE_value;
			y4 = data;
			
			CalPara.SetPC[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetPC[0] = (float)y2 - CalPara.SetPC[0]*(float)x2;
			
			CalPara.TestPC[0] = ((float)y4 - (float)y3)/((float)x4 - (float)x3);
			CalPara.OffsetTestPC[0] = (float)y4 - CalPara.TestPC[0]*(float)x4;
			
			Flag_DAC_OFF=0;
			calflag = 0;
			Off_GPOI_ResetSet();
		}
	}else if(mode == 2){
		if(range == 0)//����͵�λ�Ͷ�
		{
			RCONT_SW = 0;
			calflag = 1;
			x1 = Rmon_value;
			y1 = data;
			
//			Modify_A_READ=0;
//			Modify_A_ACT=0;
//			Modify_A_READ = Rmon_value;//��������ֵ
//			Modify_A_ACT = data;//��ȡ�Ͷ�
		}else if (range == 1)//����͵�λ�߶�
		{

			x2 = Rmon_value;
			y2 = data;
			CalPara.TestR[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetR[0] = (float)y2 - CalPara.TestR[0]*(float)x2;
				//����д��FLASH
			Flag_DAC_OFF=0;
			calflag = 0;
		}else if(range == 2)//����ߵ�λ�Ͷ�
		{
			RCONT_SW = 1;
			calflag = 1;
			x1 = Rmon_value;
			y1 = data;
		}else if (range == 3)//����ߵ�λ�߶�
		{
			
			x2 = Rmon_value;
			y2 = data;
			CalPara.TestR[1] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetR[1] = (float)y2 - CalPara.TestR[1]*(float)x2;
				//����д��FLASH
			Flag_DAC_OFF=0;
			calflag = 0;
		}else if(range == 4)//ʶ�����1�Ͷ�
		{
			calflag = 1;
//			x1 = (R1mon_value*1000)/(65535-R1mon_value);
			x1 = (R1mon_value*1000)/(65535-R1mon_value);
			y1 = data;
		}else if (range == 5)//ʶ�����1�߶�
		{
			
//			x2 = (R1mon_value*1000)/(65535-R1mon_value);
			x2 = (R1mon_value*1000)/(65535-R1mon_value);
			y2 = data;
			CalPara.TestR1[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetR1[0] = (float)y2 - CalPara.TestR1[0]*(float)x2;
				//����д��FLASH
			Flag_DAC_OFF=0;
			calflag = 0;
		}else if(range == 6)//ʶ�����2�Ͷ�
		{
			calflag = 1;
			x1 = (R2mon_value*1000)/(65535-R2mon_value);
			y1 = data;
		}else if (range == 7)//ʶ�����2�߶�
		{
			
			x2 = (R2mon_value*1000)/(65535-R2mon_value);
			y2 = data;
			CalPara.TestR2[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetR2[0] = (float)y2 - CalPara.TestR2[0]*(float)x2;
				//����д��FLASH
			Flag_DAC_OFF=0;
			calflag = 0;
		}else if(range == 8)//����ߵ�λ�Ͷ�
		{
			RCONT_SW = 1;
			calflag = 1;
			x1 = Rmon_value;
			y1 = data;
		}else if (range == 9)//����ߵ�λ�߶�
		{
			
			x2 = Rmon_value;
			y2 = data;
			CalPara.TestR[2] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
			CalPara.OffsetR[2] = (float)y2 - CalPara.TestR[2]*(float)x2;
				//����д��FLASH
			Flag_DAC_OFF=0;
			calflag = 0;
		}
		
	}
		

	Write_Flash_Bytes();
//	else if(mode == 1){//��������
//		if(range == 1)
//		{
//			x1 = abs(I_CS);
//			y1 = data;
//		}else if(range == 2){
//			x2 = abs(I_CS);
//			y2 = data;
//			SaveDebug.TestIcal[1] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
//			SaveDebug.OffsetI[1] = (float)y2 - SaveDebug.TestIcal[1]*(float)x2;
//		}else if(range == 4){
//			x1 = abs(I_CS);
//			y1 = data;
//		}else if(range == 5){
//			x2 = abs(I_CS);
//			y2 = data;
//			SaveDebug.TestIcal[4] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
//			SaveDebug.OffsetI[3] = (float)y2 - SaveDebug.TestIcal[4]*(float)x2;
//		}else if(range == 0){
//			SaveDebug.TestIcal[range] = (float)data/(float)(I_CS - SaveDebug.ClearI[0]);
//		}else if(range == 3){
//			SaveDebug.TestIcal[range] = (float)data/(float)(abs(I_CS - SaveDebug.ClearI[0]));
//		}
////		else if(range == 4){
////			x1 = V_CS;
////			y1 = data;
////		}else if(range == 5){
////			x2 = V_CS;
////			y2 = data;
////			SaveDebug.TestVcal[2] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
////			SaveDebug.OffsetV[2] = (float)y2 - SaveDebug.TestVcal[2]*(float)x2;
////		}
//		
////		if(range < 3)
////		{
////			if(range == 0)
////			{
////				SaveDebug.TestIcal[range] = (float)data/(float)(I_CS - SaveDebug.ClearI[0]);
////			}else{
////				SaveDebug.TestIcal[range] = (float)data/(float)(I_CS - SaveDebug.ClearI[1]);
////			}			
////		}else{
////			if(range == 3)
////			{
////				SaveDebug.TestIcal[range] = (float)data/(float)(abs(I_CS - SaveDebug.ClearI[0]));
////			}else{
////				SaveDebug.TestIcal[range] = (float)data/(float)(abs(I_CS - SaveDebug.ClearI[1]));
////			}
////		}
////		calflag = 1;
//	}else if(mode == 2){//���Ƶ�ѹ
//		if(range == 0)
//		{
//			_D_ = 1000;
//			x1 = data;
//			y1 = _D_;
//		}else if(range == 1){
//			_D_ = 7000;
//			x2 = data;
//			y2 = _D_;
//			SaveDebug.ControlV[0] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
//			SaveDebug.OffsetCV[0] = (float)y2 - SaveDebug.ControlV[0]*(float)x2;
//		}else if(range == 2){
//			_D_ = 10000;
//			x1 = data;
//			y1 = _D_;
//		}else if(range == 3){
//			_D_ = 13000;
//			x2 = data;
//			y2 = _D_;
//			SaveDebug.ControlV[1] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
//			SaveDebug.OffsetCV[1] = (float)y2 - SaveDebug.ControlV[1]*(float)x2;
//		}else if(range == 4){
//			_D_ = 17000;
//			x1 = data;
//			y1 = _D_;
//		}else if(range == 5){
//			_D_ = 20000;
//			x2 = data;
//			y2 = _D_;
//			SaveDebug.ControlV[2] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
//			SaveDebug.OffsetCV[2] = (float)y2 - SaveDebug.ControlV[2]*(float)x2;
//		}else if(range == 6){
//			_D_ = 24000;
//			x1 = data;
//			y1 = _D_;
//		}else if(range == 7){
//			_D_ = 27000;
//			x2 = data;
//			y2 = _D_;
//			SaveDebug.ControlV[3] = ((float)y2 - (float)y1)/((float)x2 - (float)x1);
//			SaveDebug.OffsetCV[3] = (float)y2 - SaveDebug.ControlV[3]*(float)x2;
//		}
//		
////		if(range == 0)
////		{
////			SaveDebug.ControlV[range] = (float)1000/(float)data;
////		}else if(range == 1){
////			SaveDebug.ControlV[range] = (float)10000/(float)data;
////		}else if(range == 2){
////			SaveDebug.ControlV[range] = (float)15000/(float)data;
////		}else if(range == 3){
////			SaveDebug.ControlV[range] = (float)19000/(float)data;
////		}
////		calflag = 1;
//	}else if(mode == 3){//��������
//		SaveDebug.ClearI[range] = I_CS;
////		calflag = 1;
//	}
	
}

u16 SetErr_ACK(char *buf, u8 addr ,u8 ucErr)
{
		u16 i=0;
		u8 len = 0;
		char stradd[3] = {0};
		int addlen=0;
		memset(buf,0,sizeof(buf));
    	buf[i++] = ChrStartS;
		sprintf(stradd,"%d",addr);
		addlen=strlen(stradd);
		strncat(&buf[i],stradd,addlen);
		i+=addlen;
		buf[i++] = '/';
		switch(ucErr)
        {
          case 1:         //
          	   len = 9;
          	   memmove(&buf[i],"PARA ERR!",len);   
          	   break;
	      case 2:         //
          	   len = 8;
          	   memmove(&buf[i],"CMD ERR!",len);   
          	   break;
	      case 3:         //
          	   len = 8;
          	   memmove(&buf[i],"REM ERR!",len);   
          	   break;
		  default:    
               len = 8;
          	   memmove(&buf[i],"CMD ERR!",len);   
               break;
	    }
	    i+=len;
		buf[i++] = ChrEndR;
		buf[i++] = ChrEndS;
		return i;
}

u16 SerialRemoteHandleL(u8 len,char* buf)
{

  
    u16 currCharNum;
    u32 temp1;
  u8 Gmode_Vale;
	u8 i,j;
	
	u8 addr = 0;
	char str[200]={0};
	int cmd_flag=255;
	u8 templen =0;
	u8 tmpFg;
	u8 LM_S_Vale,LOW_I_Vale,H_L_Vale,SWITCH_Vale;
	u16 I_ADC_Vale,V_ADC_Vale;
	int32_t V_CS_Vale,I_CS_Vale;
	float Lvl_Vale,fadcx,fsingal, fVale;
 
//	u8 len = 11;
	u8 pntlen = 0;


	
	currCharNum=0;
	if((buf[currCharNum] != ChrStartR) || (buf[len-2] != ChrEndR)||(buf[len-1] != ChrEndS))
    {
		return SetErr_ACK(buf,addr ,CMD_ERR); 
    } 
	str[currCharNum++] = ChrStartR;
	sprintf(&str[currCharNum],"%d",addr);
	strncat(str,"/",1);
	currCharNum =strlen(str);
	cmd_flag=strncmp(buf,str,strlen(str));
	//testflag = cmd_flag;
	if(0!=cmd_flag)
	{
		return SetErr_ACK(buf, addr ,CMD_ERR);
		
	}
	for (j=0;j<CmdNumb;j++)
    {
		templen = strlen(CmdStr[j]);
		cmd_flag=strncmp(&buf[currCharNum],CmdStr[j],templen);
	    if(0==cmd_flag)
	    {

		  buf[0] = ChrStartS;
		  currCharNum+=templen;
		  switch(j)
          {
			case 0:
				pntlen = 5;    //
				if(buf[currCharNum]=='?')
				{

					
					sprintf(&buf[currCharNum],"%05d",Para.CVoltage);
					currCharNum+=pntlen;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%06d",Para.CLaod_Current);
					currCharNum+=6;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%05d",Para.CPOW_Voltage);
					currCharNum+=pntlen;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%05d",Para.CCurrent);
					currCharNum+=pntlen;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%05d",Para.CR_VLUE);
					currCharNum+=pntlen;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%04d",Para.CR1_VLUE);
					currCharNum+=4;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%04d",Para.CR2_VLUE);
					currCharNum+=4;

					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%06d",(int)(max_discahrgeNum*1000));
					currCharNum+=6;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%03d",shorttime);
					currCharNum+=3;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%1d",mainswitch);
					currCharNum+=1;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%1d",finishflag);
					currCharNum+=1;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%1d",lockflag);
					currCharNum+=1;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%2d",version);
					currCharNum+=2;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%03d",Temperature);
					currCharNum+=3;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%1d",overflag);
					currCharNum+=1;
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%d",H_L);
//					currCharNum++;
					
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%d",LM_S);
//					currCharNum++;
					
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%02d",(int)temperature);
//					currCharNum+=2;
					
				  buf[currCharNum++ ] = ChrEndR;
				  buf[currCharNum++ ] = ChrEndS;
				}
				else
				{
					return SetErr_ACK(buf, addr ,CMD_ERR);
				}
				break;
			case 1:
				pntlen = 1;
				if(buf[currCharNum]=='?')
				{
					  sprintf(&buf[currCharNum],"%1d",MODE);
				    currCharNum+=pntlen;
				    buf[currCharNum ++] = ChrEndS;
					
				}
				else
				{
					pntlen = 1;				
					for(i=0,temp1=0;i<pntlen;i++)
					{
						  temp1 = temp1*10+(buf[currCharNum++]-0x30);
					}		                   
//					if(temp1>1)
//					{
//						return SetErr_ACK(buf, addr ,PARA_ERR);
//					}
					MODE=temp1;
//					Off_GPOI_ResetSet();
					OnOff_GPOI_ResetSet( 2, 0 );
					mainswitch = 0;
					sendwait = 3;
					MODE_PARASET(MODE);
	//				Change_LM_Val(LM_S_Vale);
					buf[currCharNum++] = ChrEndR;
				}
				break;
				case 2:
				pntlen = 1;
				if(buf[currCharNum]=='?')
				{
					sprintf(&buf[currCharNum],"%1d",C_DISCHARGE);
				    currCharNum+=pntlen;
				    buf[currCharNum ++] = ChrEndS;
					
				}
				else
				{
					pntlen = 1;				
					for(i=0,temp1=0;i<pntlen;i++)
					{
						  temp1 = temp1*10+(buf[currCharNum++]-0x30);
					}		                   
					if(temp1>1)
					{
						return SetErr_ACK(buf, addr ,PARA_ERR);
					}
//					MODE=temp1;
					
					
					mainswitch = temp1;
//						Para.CSET_Voltage = 5000;
//						Para.CSET_Current = 1000;
					
						
					MODE_ONOFF(mainswitch);
	//				Change_LM_Val(LM_S_Vale);
					buf[currCharNum++] = ChrEndR;
		        }
				break;
				case 3:
				pntlen = 1;
				if(buf[currCharNum]=='?')
				{
					  sprintf(&buf[currCharNum],"%1d",C_DISCHARGE);
				    currCharNum+=pntlen;
				    buf[currCharNum ++] = ChrEndS;
					
				}
				else
				{
					if(currCharNum == 0x0b)
					{						
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}		
					Para.IR_Start_C = temp1;//��ʼ����
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.IR_Step_C = temp1;//��������
					temp1 = 0;
					 
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.IR_Threshold_V = temp1;//�ż���ѹ
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<3;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.IR_Step_Time = temp1;//����ʱ��
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.POWER_OutPut_V = temp1;//��Դ��ѹ
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.POWER_Limit_C = temp1;//��Դ����
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.POWER_Cutoff_C = temp1;//���ó���ֹ����
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.LOAD_V = temp1;//���ص�ѹ
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.LOAD_C = temp1;//���ص���
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.LOAD_Cutoff_V = temp1;//���ø��ؽ�ֹ��ѹ
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.CDC_OutPut_V = temp1;//�����������Գ���ѹ
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.CDC_Limit_C = temp1;//�����������Գ�����Ƶ���
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.CDC_Ccutoff_V = temp1;//�����������Գ���ֹ��ѹ
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.CDC_Ccutoff_C = temp1;//�����������Գ���ֹ����
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.CDC_Load_C = temp1;//�����������Էŵ����
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.CDC_Dcutoff_V = temp1;//�����������Էŵ��ֹ��ѹ
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<3;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.CDC_Cycle_Time = temp1;//ѭ������
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<3;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					if(MODE == 0)
					{
						Para.CLOAD_MODE = temp1;//����ģʽ
					}
					temp1 = 0;
//					MODE_PARASET(MODE);
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<3;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Rraly = temp1;//���赵λ
					temp1 = 0;
					
					if(buf[currCharNum++] == ',')
					{
						for(i=0,temp1=0;i<3;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}	
					Para.ACT_DELAY = temp1;//������ʱ
					temp1 = 0;
					
					MODE_PARASET(MODE);
					
					 buf[currCharNum++] = ChrEndR;
		        }
				break;
				case 4:
				pntlen = 1;
				if(buf[currCharNum]=='?')
				{
					  sprintf(&buf[currCharNum],"%1d",C_DISCHARGE);
				    currCharNum+=pntlen;
				    buf[currCharNum ++] = ChrEndS;
					
				}
				else
				{
					if(currCharNum == 0x0b)
					{						
						for(i=0,temp1=0;i<2;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}
					MODE = temp1;//��Ŀ
					temp1 = 0;
					
					if(MODE == 0)
					{
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						Para.CSET_Current_Laod = temp1;//���ص���
						temp1 = 0;
						
	
						if(Para.CSET_Current_Laod > 20000)
						{
							LOAD_I_SW = 1;
							GPIO_ResetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�ߵ�λ
						}else{
							LOAD_I_SW = 0;
							GPIO_SetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�͵�λ
						}
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<1;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}
						mainswitch = temp1;//����
						if(mainswitch == 0)
						{
							LOAD_I_SW = 1;
							GPIO_ResetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�ߵ�λ
						}
					}else if(MODE == 1){
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						Para.CDC_OutPut_V = temp1;//��Դ��ѹ
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						Para.CDC_Limit_C = temp1;//��Դ����
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<1;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}
						mainswitch = temp1;//����
					}else if(MODE == 2){
						LOAD_I_SW = 1;
						GPIO_ResetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�ߵ�λ
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						Para.IR_Start_C = temp1;//��ʼ����
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						Para.IR_Step_C = temp1;//��������
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<1;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}
						mainswitch = temp1;//����
						
						if(mainswitch == 1)
						{
							finishflag = 0;
						}
					}else if(MODE == 3){
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						Para.CDC_OutPut_V = temp1;//��Դ��ѹ
						temp1 = 0;
						
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						Para.CDC_Limit_C = temp1;//��Դ����
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<1;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}
						mainswitch = temp1;//����
						
						
					}else if(MODE == 4){
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						Para.CSET_Current_Laod = temp1;//���ص���
						temp1 = 0;
						
	
						if(Para.CSET_Current_Laod > 20000)
						{
							LOAD_I_SW = 1;
							GPIO_ResetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�ߵ�λ
						}else{
							LOAD_I_SW = 0;
							GPIO_SetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�͵�λ
						}
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<1;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}
						mainswitch = temp1;//����
						if(mainswitch == 0)
						{
							LOAD_I_SW = 1;
							GPIO_ResetBits(GPIOC,GPIO_Pin_11);//��������Ϊ�ߵ�λ
						}
					}else if(MODE == 5){
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<1;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}
						mainswitch = temp1;//����
						if(mainswitch == 0)
						{
							mainswitch = 0;
						}
					}else if(MODE == 6){
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<5;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}	
						temp1 = 0;
						
						if(buf[currCharNum++] == ',')
						{
							for(i=0,temp1=0;i<1;i++)
							{
								temp1 = temp1*10+(buf[currCharNum++]-0x30);
							}
						}
						mainswitch = temp1;//����
					}
					
					temp1 = 0;
//					MODE_PARASET(MODE);
					Off_GPOI_ResetSet();
					LIST_ONOFF(mainswitch);
					buf[currCharNum++] = ChrEndR;
		        }
				break;
				case 12://��ѹУ׼
				{
					pntlen = 8;
					if(buf[currCharNum]=='?')
					{
						buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
						sprintf(&buf[currCharNum],"%03.3f",Lvl_Vale);
						currCharNum+=pntlen;
						buf[currCharNum++ ] = ChrEndS;
					}
					else
					{
						tmpFg = 1;
						for(i=0;i<pntlen;i++)
						{
						   if(i==0||i==4)
							  continue;
						   if(IsDigitChar(buf[currCharNum+i])==1)
						   {
							  tmpFg = (tmpFg&0x01);
						   }
						   else
						   {
							  tmpFg = 0;
						   }
						}
						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
						{
							return SetErr_ACK(buf, addr ,CMD_ERR);
							
						}
						temp1=0;
						for(i=0;i<pntlen;i++)
						{
						   if(i==0)
						   {
							   fsingal = buf[currCharNum++]-0x30;
							   continue; 
						   } 
						   else if(i==4)
						   {
							   currCharNum++;
							   continue; 
						   }
						   else									 
						   temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}	
						CalHandle(0,fsingal,temp1);
						
					}
				}break;
				case 5:
				pntlen = 1;
				if(buf[currCharNum]=='?')
				{
					sprintf(&buf[currCharNum],"%1d",C_DISCHARGE);
				    currCharNum+=pntlen;
				    buf[currCharNum ++] = ChrEndS;
					
				}
				else
				{
					pntlen = 1;				
					for(i=0,temp1=0;i<pntlen;i++)
					{
						  temp1 = temp1*10+(buf[currCharNum++]-0x30);
					}		                   
					if(temp1>1)
					{
						return SetErr_ACK(buf, addr ,PARA_ERR);
					}
//					MODE=temp1;
					
					
					overflag = temp1;
//						Para.CSET_Voltage = 5000;
//						Para.CSET_Current = 1000;
					
						
//					MODE_ONOFF(mainswitch);
	//				Change_LM_Val(LM_S_Vale);
					buf[currCharNum++] = ChrEndR;
		        }
				break;
				case 13://����У׼
				{
					pntlen = 8;
					if(buf[currCharNum]=='?')
					{
//						buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
//									sprintf(&buf[currCharNum],"%03.3f",Lvl_Vale);
//						currCharNum+=pntlen;
//						buf[currCharNum++ ] = ChrEndS;
					}
					else
					{
						tmpFg = 1;
						for(i=0;i<pntlen;i++)
						{
						   if(i==0||i==4)
							  continue;
						   if(IsDigitChar(buf[currCharNum+i])==1)
						   {
							  tmpFg = (tmpFg&0x01);
						   }
						   else
						   {
							  tmpFg = 0;
						   }
						}
						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
						{
							return SetErr_ACK(buf, addr ,CMD_ERR);
							
						}
						temp1=0;
						for(i=0;i<pntlen;i++)
						{
						   if(i==0)
						   {
							   fsingal = buf[currCharNum++]-0x30;
							   continue; 
						   } 
						   else if(i==4)
						   {
							   currCharNum++;
							   continue; 
						   }
						   else									 
						   temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}	
						CalHandle(1,fsingal,temp1);
						
					}
				}break;
				case 14://����У׼
				{
					pntlen = 6;
					if(buf[currCharNum]=='?')
					{
						buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
									sprintf(&buf[currCharNum],"%03.3f",Lvl_Vale);
						currCharNum+=pntlen;
						buf[currCharNum++ ] = ChrEndS;
					}
					else
					{
						tmpFg = 1;
						for(i=0;i<pntlen;i++)
						{
						   if(i==0||i==4)
							  continue;
						   if(IsDigitChar(buf[currCharNum+i])==1)
						   {
							  tmpFg = (tmpFg&0x01);
						   }
						   else
						   {
							  tmpFg = 0;
						   }
						}
						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
						{
							return SetErr_ACK(buf, addr ,CMD_ERR);
							
						}
						temp1=0;
						for(i=0;i<pntlen;i++)
						{
						   if(i==0)
						   {
							   fsingal = buf[currCharNum++]-0x30;
							   continue; 
						   } 
//						   else if(i==4)
//						   {
//							   currCharNum++;
//							   continue; 
//						   }
//						   else									 
						   temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
						CalHandle(2,fsingal,temp1);
						
					}
				}break;
				case 18:
				{
					if(buf[currCharNum]=='?')
					{
//						sprintf(&buf[currCharNum],"%1d",C_DISCHARGE);
//						currCharNum+=pntlen;
//						buf[currCharNum ++] = ChrEndS;
						
					}
					else
					{
						pntlen = 1;				
						for(i=0,temp1=0;i<pntlen;i++)
						{
							  temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}		                   
						if(temp1>1)
						{
							return SetErr_ACK(buf, addr ,PARA_ERR);
						}
	//					MODE=temp1;
						
						bootflag = 1;
						Write_Boot_Flag();
						if(temp1 == 1)
						{
							JumpBoot(55);
						}
					}
				}break;
//				case 15:
//				{
//					pntlen = 1;
//					if(buf[currCharNum]=='?')
//					{
//						sprintf(&buf[currCharNum],"%1d",Gmode_Vale);
//						currCharNum+=pntlen;
//						buf[currCharNum] = ChrEndR;
//						currCharNum++;
//	//				    buf[currCharNum ++] = ChrEndS;
//						
//					}
//					else
//					{
//						tmpFg = 1;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(IsDigitChar(buf[currCharNum+i])==1)
//						   {
//							  tmpFg = (tmpFg&0x01);
//						   }
//						   else
//						   {
//							  tmpFg = 0;
//						   }
//						}
//						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//						{
//							return SetErr_ACK(buf, addr ,CMD_ERR);
//							
//						}					
//						for(i=0,temp1=0;i<pntlen;i++)
//						{
//							  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//						}		                   
//						if(temp1 >Mode_H)
//						 {
//							 return SetErr_ACK(buf, addr ,PARA_ERR);
//							 
//						 }
//						 if(H_L == 1)
//						 {
//							CalHandle(3,1,temp1);
//						 }else{
//							 CalHandle(3,0,temp1);
//						 }
//						 
////								 Mode=temp1;
////								Change_MODE_Val(Mode);
////						 buf[currCharNum++] = ChrEndS;
//					}
//					break;
//				}break;
//				case 16:
//				{
//					pntlen = 3;
//					if(buf[currCharNum]=='?')
//					{
//						sprintf(&buf[currCharNum],"%1d",Gmode_Vale);
//						currCharNum+=pntlen;
//						buf[currCharNum] = ChrEndR;
//						currCharNum++;
//	//				    buf[currCharNum ++] = ChrEndS;
//						
//					}
//					else
//					{
//						tmpFg = 1;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(IsDigitChar(buf[currCharNum+i])==1)
//						   {
//							  tmpFg = (tmpFg&0x01);
//						   }
//						   else
//						   {
//							  tmpFg = 0;
//						   }
//						}
//						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//						{
//							return SetErr_ACK(buf, addr ,CMD_ERR);
//							
//						}					
//						for(i=0,temp1=0;i<pntlen;i++)
//						{
//							  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//						}		                   
//						 overflag = temp1;
//						 
////								 Mode=temp1;
////								Change_MODE_Val(Mode);
////						 buf[currCharNum++] = ChrEndS;
//					}
//					break;
//				}break;
//				case 17:
//				{
//					pntlen = 2;
//					if(buf[currCharNum]=='?')
//					{
////						buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
//									sprintf(&buf[currCharNum],"%02d",(int)temperature);
//						currCharNum+=pntlen;
//						buf[currCharNum++ ] = ChrEndR;
//						buf[currCharNum++ ] = ChrEndS;
//					}
//				}break;
			default:    //ERR
           		 return SetErr_ACK(buf, addr ,CMD_ERR);
	 			  
		  }
		  break;
		}
		
	}
	if(j>=CmdNumb)
	{
		return SetErr_ACK(buf, addr ,CMD_ERR);			  
	}
	return currCharNum+1;
}



static void USART1_NVIC_Config(void)//���ڽ����ж�����
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Configuration(void)//���ڳ�ʼ������
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/******ʹ�ܴ��ڽ����ж�******************/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1
	USART_ClearFlag(USART1, USART_FLAG_TC);
  
	USART1_NVIC_Config();//�����ж�����
}			


static void USART2_NVIC_Config(void)//���ڽ����ж�����
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_Configuration(void)//���ڳ�ʼ������
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	/******ʹ�ܴ��ڽ����ж�******************/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_ORE, ENABLE);	
	USART_Cmd(USART2, ENABLE);//ʹ�ܴ���1
  USART_ClearFlag(USART2, USART_FLAG_TC);
  
  USART2_NVIC_Config();//�����ж�����
}

static void USART3_NVIC_Config(void)//????????
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    /* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void USART3_DMA_init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	//����3ʹ��DMA1 ����Ϊ������3  ͨ��4   
    //F4��F1DMA����һ����������˵����ϸ�ο����������ֲ�DMA ��35
    /* 1.ʹ��DMA2ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	//���ڷ���DMA����
    /* 2.����ʹ��DMA�������� */
    DMA_DeInit(DMA1_Stream3); //DMA1 ������ 3
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  /* ����DMA1ͨ��4 */
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(USART3->DR)); /* Ŀ�� */
    DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)MasterSendbuf; /*Դ�ڴ��ַ*/
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_MemoryToPeripheral;  /*������            
                                                                            �浽����*/
    DMA_InitStructure.DMA_BufferSize          = MAXRxTxLen;                    
    /* ���� */                  
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    
    /* �����ַ�Ƿ����� */
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;        
    /* �ڴ��ַ�Ƿ����� */
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;  
    /* Ŀ�����ݴ��� */
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;      
    /* Դ���ݿ�� */
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;             
    /* ���δ���ģʽ/ѭ������ģʽ */
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;             
    /* DMA���ȼ� */
    DMA_InitStructure.DMA_FIFOMode            = DMA_FIFOMode_Disable;          
    /* FIFOģʽ/ֱ��ģʽ */
    DMA_InitStructure.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull; 
    /* FIFO��С */
    DMA_InitStructure.DMA_MemoryBurst         = DMA_MemoryBurst_Single;      
     /* ���δ��� */
    DMA_InitStructure.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;
    /* 3. ����DMA */
    DMA_Init(DMA1_Stream3, &DMA_InitStructure);
    /* 4.ʹ��DMA�ж� */
    DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
    /* 5.ʹ�ܴ��ڵ�DMA���ͽӿ� */
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    /* 6. ����DMA�ж����ȼ� */
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Stream3_IRQn;  
    //  DMA�����ж�         
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;          
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 4; 
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    /* 7.��ʹ��DMA */                  
    DMA_Cmd(DMA1_Stream3, DISABLE);// ������2
	//************************DMA����***********************//
	/* 2.����ʹ��DMA�������� */
    DMA_DeInit(DMA1_Stream1); 
    DMA_InitStructure.DMA_Channel             = DMA_Channel_4;               
    /* ����DMAͨ�� */
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(USART3->DR));   
    /* Դ��ַ */
    DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)MasterRecbuf;    
    /* Ŀ���ַ */
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralToMemory;    
    /* �����������ڴ�  */
    DMA_InitStructure.DMA_BufferSize          = MAXRxTxLen;                    		
    /* ���� */                  
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    
      /* �����ַ�Ƿ����� */
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;         
    /* �ڴ��ַ�Ƿ����� */
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_MemoryDataSize_Byte;     
    /* Ŀ�����ݴ��� */
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;      
    /* Դ���ݿ�� */
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Circular;             
    /*ѭ������ģʽ */
    DMA_InitStructure.DMA_Priority            = DMA_Priority_VeryHigh;        
    /* DMA���ȼ� */
    DMA_InitStructure.DMA_FIFOMode            = DMA_FIFOMode_Disable;         
     /* FIFOģʽ/ֱ��ģʽ */
    DMA_InitStructure.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull; 
    /* FIFO��С */
    DMA_InitStructure.DMA_MemoryBurst         = DMA_MemoryBurst_Single;       
    /* ���δ��� */
    DMA_InitStructure.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;
    /* 3. ����DMA */
    DMA_Init(DMA1_Stream1, &DMA_InitStructure);
    /* 4.���ڽ��ղ���ҪDMA�жϣ��ʲ�����DMA�ж� */
    /* 5.ʹ�ܴ��ڵ�DMA���� */
    USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
    /* 6. ���ڽ��ղ���ҪDMA�жϣ��ʲ�������DMA�ж����ȼ� */
    /* 7.ʹ��DMA */ 
    DMA_Cmd(DMA1_Stream1,ENABLE);
}



void USART3_Configuration()//???????
{  
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //?? GPIOA ??
    
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);
    

    
    
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    

	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
    

	
	/******????????******************/
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART3_NVIC_Config();//??????
	USART3_DMA_init();
	USART_Cmd(USART3, ENABLE);//????1
    USART_ClearFlag(USART3, USART_FLAG_TC);
    
  
    
	
}


void USART3WriteDMA(u16 len)
{
    USART_ClearFlag(USART3, USART_FLAG_TC);     //���ڷ���׼��
	DMA_Cmd(DMA1_Stream3, DISABLE);//�ر�DMA ������3
    DMA_SetCurrDataCounter(DMA1_Stream3,len);//����DMA��������
	DMA_Cmd(DMA1_Stream3, ENABLE);// ��DMA ������3
   
}


void DMA1_Stream3_IRQHandler(void)
{
	//----DMA1 ͨ��3����жϣ���Ӧ���ж�Ҳ��Ҫ���
	if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF3) != RESET)   
    {
		/* �����־λ */
		DMA_ClearFlag(DMA1_Stream3,DMA_IT_TCIF3); 
		/* �ر�DMA */
		DMA_Cmd(DMA1_Stream3,DISABLE);
		
		while (!(USART3->SR & USART_FLAG_TC));  //Ӳ�����ݷ������
		while (!(USART3->SR & USART_FLAG_TXE)); //�ȴ����ݴ����������Ӳ�����з���
	}
}

void USART3_IRQHandler(void)
{
    u8 Clear = Clear;
    u16 num = 0;
         
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
        Clear = USART3->SR;  //��SR�Ĵ������                                                            
       
        Clear = USART3->DR;  //��DR�Ĵ������                                                        
        DMA_Cmd(DMA1_Stream1,DISABLE);   //�ر�DMA
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF2);
        num = DMA_GetCurrDataCounter(DMA1_Stream1); //�õ������������ݸ���
        if(num < MAXRxTxLen)
        {
            USART3_Recive_leng   = MAXRxTxLen - num;
        }else
        {
            USART3_Recive_leng = MAXRxTxLen;
        }
         
        if(USART3_Recive_leng > MAXRxTxLen)
        {
            USART3_Recive_leng = 0;  //����������ݳ���                                               
        }
        DMA_SetCurrDataCounter(DMA1_Stream1,MAXRxTxLen);//����DMA���䳤��                                                        
        DMA_Cmd(DMA1_Stream1,ENABLE);  //����DMA
        USART3_Recive_flg = 1;    //���ݽ��ճɹ�                                                  
        
    } 
}

void USART2_IRQHandler(void)
{
	u8 Res;
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Res=USART_ReceiveData(USART2);

		if((USART_RX_STA&0x8000)==0)//����δ���
		{
		if(USART_RX_STA&0x4000)//���յ���0x0d
		{
			if(Res!=0x0a)
				USART_RX_STA=0;//���մ���,���¿�ʼ
			else 
			{
		//		SerialRemoteHandleL(USART_RX_STA,USART_RX_BUF);
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;			//�����յ����� ����������
				USART_RX_STA++;
				USART_RX_CNT = USART_RX_STA;
				USART_RX_STA|=0x8000;	//��������� 			  //���յ��س��ĺ��ֽ�  ��λ״̬�Ĵ��� 
			}
		}
		else //��û�յ�0X0D
		{	
			if(Res==0x0d)
			{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;			//�����յ����� ����������
				USART_RX_STA++;	
				USART_RX_STA|=0x4000;					 //���յ��س���ǰһ�ֽ�  ��λ״̬�Ĵ���
			}
			else
				{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;			//�����յ����� ����������
					USART_RX_STA++;									//����+1 Ϊ��һ����׼��
				if(USART_RX_STA>(USART_REC_LEN-1))
					USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		} 
	}
}
void UART2_Send(void)
{
	static vu8 UART_Buffer_Send_pointer=0;
		if (UART_Buffer_Send_pointer < Transmit_BUFFERsize)
         {
					 if(UART_Buffer_Send[0]==ADDR)
					 {
						USART_SendData(USART2,UART_Buffer_Send[UART_Buffer_Send_pointer]);
						while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//�ȴ��������
						UART_Buffer_Send_pointer++;
					}
         }
		else 
		{
			UART_Buffer_Send_pointer=0;
			UART_SEND_flag=0;//�������һ֡����
		}
}

void UART1_Send(void)
{
	static vu8 UART_Buffer_Send_pointer=0;
		if (UART_Buffer_Send_pointer < Transmit_BUFFERsize)
         {
				 if(UART_Buffer_Send[0]==ADDR)
				 {
					USART_SendData(USART1,UART_Buffer_Send[UART_Buffer_Send_pointer]);
					while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//�ȴ��������
					UART_Buffer_Send_pointer++;
				 }
         }
		else 
		{
			UART_Buffer_Send_pointer=0;
			UART_SEND_flag=0;//�������һ֡����
		}
}

