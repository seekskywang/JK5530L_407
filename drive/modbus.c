/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * �ļ���  :MODBUS.C
 * ����   :
 * ����    :MODBUS?????
 * ����    :KL220A_Master
 * Ӳ������: 485
 * �޸�����:2014-12-22
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
/*************************У׼����************************************/
vu16 ADJ_Write;//У׼ʱ��д���ʵ��ֵ
vu8 DAC_Flag;//DAC�Ƿ���ر�־
vu32 Modify_A_READ;
vu32 Modify_C_READ;
vu32 Modify_A_ACT;
	
vu32 Modify_B_READ;
vu32 Modify_D_READ;
vu32 Modify_B_ACT;
vu32 Correct_Parametet[17];//У׼����
vu32 Correct_Strong[17];//У׼ϵ��
vu8  correct_por[8];
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count);
/*************************��������***********************************/
vu32 Run_Control[17];
vu8 ADDR;
//============================��������===============================//
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count);
//===========================MODBUSЭ��=============================//
void UART_Action(void)
{//RUT��ʽ��
	//ADDR  ������  ���Ĵ�������ʼ��ַ��   ���Ĵ�������ʼ��ַ��  �������ָ������ֽ�   �����ݸ������ֽ�  CRC�� CRC��
	//���ظ�ʽ��ADDR ������ ���������ֽ���  ���ݸ�  ���ݵ� ..... CRC��  CRC��
	if (UART_Buffer_Rece[0] == ADDR)
	{
		if (UART_Buffer_Rece[1] == (0x03))	//����3 ������   
		{																		 
			vu8 i;
			vu16 crc_result;
			crc_result = (UART_Buffer_Rece[6] << 8) + UART_Buffer_Rece[7];
			if ((crc_result == Hardware_CRC(UART_Buffer_Rece,6)) ||(crc_result == 0) )
			{
				if (UART_Buffer_Rece[3] < 0x07)    								//����Ĵ����ڿɶ���Χ��
				{
					if ((UART_Buffer_Rece[3] + UART_Buffer_Rece[5]) < 0x0F)		//������һ����ȡ�ļĴ�����ַ�ڿɶ���Χ��
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
//===============================д�Ĵ���=================================
	if ((UART_Buffer_Rece[0] == 0) ||  (UART_Buffer_Rece[0] == ADDR) || (UART_Buffer_Rece[0] == ((ADDR-1)/4+100)))	 
	{
		vu8 var8;
		vu8 a=0;
		vu16 var16;
		vu16 crc_result;
//=========================��������6 д�����Ĵ���===========================
		if (UART_Buffer_Rece[1] == 6)                                 //�жϵڶ����ֽ��Ƿ�Ϊ����6
		{
			if (UART_Buffer_Rece[3] < 0x05)							  //�ж���Ҫд�ĵ�ַ�Ƿ��ڿ�д��Χ��
			{
				crc_result = (UART_Buffer_Rece[6] << 8) + UART_Buffer_Rece[7];
				if ((crc_result == Hardware_CRC(UART_Buffer_Rece,6)) ||(crc_result == 0) )		  //���CRC
				{
					var16 = (UART_Buffer_Rece[4] << 8) + UART_Buffer_Rece[5];	//��5 6���ֽ�ΪҪд�������
					var8 = UART_Buffer_Rece[3];	        						//��3 4���ֽ�ΪҪд��ĵ�ַ
					Run_Control[var8] = var16;			    //������д��ָ���ĵ�ַ

					if (UART_Buffer_Rece[0] == ADDR)							//�㲥ģʽ�²���������
					{
						for (a=0;a<8;a++)
						{UART_Buffer_Send[a] = UART_Buffer_Rece[a];}
						Transmit_BUFFERsize = 8;						//ԭ�����ݷ��أ�������CRC
						UART_SEND_flag=1;
					}
				}
			}
		}
//=======================================����������16����д�Ĵ���===========================================
//������16��ʽ:
//     ��ַ ���� д����ʼ��ַ��  д����ʼ��ַ�� д��������  д�������� д���ֽ���  ���ݸ� ���ݵ� ......CRC�� CRC��
//�������ݸ�ʽ:
//     ��ַ ���� д����ʼ��ַ��  д����ʼ��ַ��  д���ֽ����� д���ֽ�����  CRC��  CRC�� 
		if (UART_Buffer_Rece[1] == 16)										  
		{	
			if ((UART_Buffer_Rece[6] == 6) && (UART_Buffer_Rece[3] == 0x00))	//�涨��д�Ĵ����ĸ���
			{
				crc_result = (UART_Buffer_Rece[13] << 8) + UART_Buffer_Rece[14];
				if ((crc_result == Hardware_CRC(UART_Buffer_Rece,13)) ||(crc_result == 0) )	   //���CRC
				{												
					for (var8=0;var8<3;var8++) Run_Control[var8] = (UART_Buffer_Rece[var8*2+7] << 8) + UART_Buffer_Rece[var8*2+8];

					if (UART_Buffer_Rece[0] == ADDR)					  //�㲥ģʽ����������
					{
						UART_Buffer_Send[0] = ADDR;
						UART_Buffer_Send[1] = 16;
						UART_Buffer_Send[2] = UART_Buffer_Rece[2];
						UART_Buffer_Send[3] = UART_Buffer_Rece[3];
						UART_Buffer_Send[4] = UART_Buffer_Rece[4];
						UART_Buffer_Send[5] = UART_Buffer_Rece[5];
						crc_result = Hardware_CRC(UART_Buffer_Send,6);	 //����CRC��
						UART_Buffer_Send[6] = crc_result>>8;
						UART_Buffer_Send[7] = crc_result;				 
						Transmit_BUFFERsize = 8;					     //���÷����ֽ�������
						UART_SEND_flag=1;
					}
				}
			}			 
		}
	}
/*************************************����ΪУ׼����**************************************************************************/
	if (((UART_Buffer_Rece[0] == 0x01)&&(UART_Buffer_Rece[2] == 0xA5)))			   //У׼��ʼ
	{ 
		vu16 crc_result;
		crc_result = (UART_Buffer_Rece[6] << 8) + UART_Buffer_Rece[7];
		if (crc_result == Hardware_CRC(UART_Buffer_Rece,6) ||crc_result==0)
		{
			/**************���ص�ѹ�͵�У׼***************************/
			if(UART_Buffer_Rece[1] == 0x01)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Vmon_Load_value;//������ѹֵ
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�Ͷ�
			}
			if (UART_Buffer_Rece[1] == 0x02)			   //��ѹ����У׼���
			{
				vu32 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu32 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ =Vmon_Load_value;//������ѹֵ
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�߶�
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
				var32b = var32b * CLists.CREG_Load_CorrectionV_LOW;    //�����ѹ���������͵�	
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_LoadV_Offset_LOW = var32b;     //�����ѹ��������  
					CLists.CPolar0 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_LoadV_Offset_LOW = var32a;
					CLists.CPolar0 &= ~0x01;
				}			
					//����д��FLASH
				Flag_DAC_OFF=0;
			}
			
			/**************���ص�ѹ�ߵ�У׼***************************/
			if(UART_Buffer_Rece[1] == 0x03)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Vmon_Load_value;//������ѹֵ
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�Ͷ�
			}
			if (UART_Buffer_Rece[1] == 0x04)			   //��ѹ����У׼���
			{
				vu32 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu32 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ =Vmon_Load_value;//������ѹֵ
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�߶�
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
				var32b = var32b * CLists.CREG_Load_CorrectionV_HIG;      //�����ѹ���������ߵ�
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_LoadV_Offset_HIG = var32b;     //�����ѹ��������
					CLists.CPolar0 |= 0x04;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_LoadV_Offset_HIG = var32a;
					CLists.CPolar0 &= ~0x04;
				}			
					//����д��FLASH
				Flag_DAC_OFF=0;
			}
	     /*****************���ص��������Ϳ���У׼*************************/
			if (UART_Buffer_Rece[1] == 0x05)			   //CCģʽУ׼
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
				var32b = var32b * CLists.CREG_Load_CorrectionA_LOW;    //����CCģʽ�������������͵�
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_LoadA_Offset_LOW = var32b;     //����CCģʽ������������
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
				var32d = CLists.CSET_Load_CorrectionA_LOW;     //����CCģʽ���Ƶ����͵�
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_LoadA_Offset_LOW = var32d;    //����CCģʽ���Ƶ�������  	
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
			
			/*****************���ظߵ����������Ϳ���У׼*************************/
			if (UART_Buffer_Rece[1] == 0x07)			   //CCģʽУ׼
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
				var32b = var32b * CLists.CREG_Load_CorrectionA_LOW;   //����CCģʽ�������������͵�
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_LoadA_Offset_HIG = var32b;     //����CCģʽ������������
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
				var32d = CLists.CSET_Load_CorrectionA_HIG;  //����CCģʽ�������������ߵ�
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_LoadA_Offset_HIG = var32d;   //����CCģʽ���Ƶ�������
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
	/*************************����CVģʽ��ѹ����У׼�͵�*******************************/
			if(UART_Buffer_Rece[1] == 0x09)
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_C_READ = Contr_Laod;//���õ�ѹֵ
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�Ͷ�
			}
			if (UART_Buffer_Rece[1] == 0x0A)			   //��ѹ����У׼���
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
			
				Modify_D_READ =Contr_Laod;//���õ�ѹֵ
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�߶�	
				var32c = Modify_B_ACT; //CVģʽ��ѹ����У׼
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=(var16a*2);
				var32c=var32c/var16a;
				CLists.CSET_Load_CorrectionV_LOW = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_Load_CorrectionV_LOW;   //����CVģʽ���Ƶ�ѹ�ֵ�
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_LoadV_Offset_LOW = var32d;    	//����CVģʽ���Ƶ�ѹ����   
					CLists.CPolar1 |= 0x01;
				}
				else 
				{
					var32c = var32c - var32d;
					CLists.CSET_LoadV_Offset_LOW = var32c;
					CLists.CPolar1 &= ~0x01;
				}
					//����д��FLASH
				Flag_DAC_OFF=0;
			}
			/*************************����CVģʽ��ѹ����У׼�ߵ�*******************************/
			if(UART_Buffer_Rece[1] == 0x0B)
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_C_READ = Contr_Laod;//���õ�ѹֵ
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�Ͷ�
			}
			if (UART_Buffer_Rece[1] == 0x0C)			   //��ѹ����У׼���
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
			
				Modify_D_READ =Contr_Laod;//���õ�ѹֵ
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�߶�	
				var32c = Modify_B_ACT; //CVģʽ��ѹ����У׼
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=(var16a*2);
				var32c=var32c/var16a;
				CLists.CSET_Load_CorrectionV_LOW = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_Load_CorrectionV_HIG;      //����CVģʽ���Ƶ�ѹ�ߵ�
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
					CLists.CSET_LoadV_Offset_HIG = var32c;      //����CVģʽ���Ƶ�ѹ����
					CLists.CPolar1 &= ~0x04;
				}
					//����д��FLASH
				Flag_DAC_OFF=0;
			}
	/************************����У׼�͵�**********************************/
			if(UART_Buffer_Rece[1] == 0x0D)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Rmon_value;//��������ֵ
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�Ͷ�
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
				Modify_B_READ =Rmon_value;//��������ֵ
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�߶�
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
				var32b = var32b * CLists.CREG_CorrectionR_LOW;  //������������͵�
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_ReadR_Offset_LOW = var32b;         //�������ϵ��
					CLists.CPolar4 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_ReadR_Offset_LOW = var32a;
					CLists.CPolar4 &= ~0x01;
				}
					//����д��FLASH
				Flag_DAC_OFF=0;
			}		
			/************************����ߵ�У׼**********************************/
			if(UART_Buffer_Rece[1] == 0x0F)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Rmon_value;//��������ֵ
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�Ͷ�
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
				Modify_B_READ =Rmon_value;//��������ֵ
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�߶�
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
				var32b = var32b * CLists.CREG_CorrectionR_HIG;    //������������ߵ�
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_ReadR_Offset_HIG = var32b;     //�������ϵ��
					CLists.CPolar4 |= 0x04;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_ReadR_Offset_HIG = var32a;
					CLists.CPolar4 &= ~0x04;
				}
					//����д��FLASH
				Flag_DAC_OFF=0;
			}	
			
			/*******************************��Դ��ѹ���ƺͲ���У׼******************************************/	
			if (UART_Buffer_Rece[1] == 0x11)			   //��������У׼
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Vmon_POWE_value;//��������
				Modify_C_READ = Contr_Voltage;//���õ���
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
			}

			if (UART_Buffer_Rece[1] == 0x12)			   //��������У׼���
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
				var32b = var32b * CLists.CREG_POWER_CorrectionV;    //��Դ������ѹ����
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_POWERV_Offset = var32b;     //��ԴCVģʽ��ѹ��������  
					CLists.CPolar5 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_POWERV_Offset = var32a;
					CLists.CPolar5 &= ~0x01;					
				}
				var32c = Modify_B_ACT; //���õ�ѹУ׼
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=var16a*2;
				var32c=var32c/var16a;
				CLists.CSET_POWER_CorrectionV = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_POWER_CorrectionV;    //��Դ��ѹ���Ʋ���
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_POWERV_Offset = var32d;    //��ԴCVģʽ��ѹ���Ʋ���    
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
	/*******************************��ԴCCģʽ���������Ϳ���У׼******************************************/	
			if (UART_Buffer_Rece[1] == 0x13)			   //��������У׼
			{
				Modify_A_READ=0;
				Modify_C_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = Imon_POWE_value;//��������
				Modify_C_READ = Contr_Current;//���õ���
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];
			}
			if (UART_Buffer_Rece[1] == 0x14)			   //��������У׼���
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
				var32b = var32b * CLists.CREG_POWER_CorrectionA_CC;    //��ԴCCģʽ������������
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_POWERA_Offset_CC = var32b;    //��ԴCCģʽ������������ 
					CLists.CPolar6 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_POWERA_Offset_CC = var32a;
					CLists.CPolar6 &= ~0x01;					//��������ϵ�������У׼���
				}
				var32c = Modify_B_ACT; //���õ���У׼
				var32c = var32c - Modify_A_ACT;
				var32c = var32c << 12;
				var16a=Modify_D_READ-Modify_C_READ;
				var16a=var16a*2;
				var32c=var32c/var16a;
				CLists.CSET_POWER_CorrectionA = var32c;
				var32c = Modify_B_ACT;
				var32c = var32c << 12;
				var32d = CLists.CSET_POWER_CorrectionA;    //��Դ�������Ʋ���
				var32d = var32d * (Modify_D_READ*2);
				if (var32c < var32d)
				{
					var32d = var32d - var32c;
					CLists.CSET_POWERA_Offset = var32d;    //��ԴCCģʽ�������Ʋ��� 
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
	
	/****************��ԴCV��������У׼**********************************/
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
				var32b = var32b * CLists.CREG_POWER_CorrectionA_CV;   //��ԴCVģʽ������������
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_POWERA_Offset_CV = var32b;   //��ԴCCģʽ������������
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
	/****************************ʶ�����1У׼*****************************************/
			if(UART_Buffer_Rece[1] == 0x17)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				//Modify_A_READ = R1mon_value;//������ѹֵ
				Modify_A_READ=(R1mon_value*1000)/(65535-R1mon_value);
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�Ͷ�
			}
			if (UART_Buffer_Rece[1] == 0x18)			   //��ѹ����У׼���
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				//Modify_B_READ =R1mon_value;//������ѹֵ
				Modify_B_READ=(R1mon_value*1000)/(65535-R1mon_value);
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�߶�
				var32a = Modify_B_ACT;
				var32a = var32a - Modify_A_ACT;
				var32a = var32a << 12;
				var16 = Modify_B_READ - Modify_A_READ;
				var32a = var32a / var16;
				CLists.CREG_CorrectionR1 = var32a;     //ʶ�����1����
				var32a=0;
				var32a = Modify_B_ACT;
				var32a = var32a << 12;
				var32b = Modify_B_READ;
				var32b = var32b * CLists.CREG_CorrectionR1;
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_ReadR1_Offset = var32b;    //�������ϵ��
					CLists.CPolar7 |= 0x01;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_ReadR1_Offset = var32a;
					CLists.CPolar7 &= ~0x01;
				}
					//����д��FLASH
				Flag_DAC_OFF=0;
			}	
			/***************************ʶ�����2У׼**************************************/
			if(UART_Buffer_Rece[1] == 0x19)
			{
				Modify_A_READ=0;
				Modify_A_ACT=0;
				Modify_A_READ = R2mon_value;//������ѹֵ
				Modify_A_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�Ͷ�
			}
			if (UART_Buffer_Rece[1] == 0x1A)			   //��ѹ����У׼���
			{
				vu16 var16;
				vu32 var32a;
				vu32 var32b;
				
				vu16 var16a;
				vu32 var32c;
				vu32 var32d;
				Modify_B_READ=0;
				Modify_B_ACT=0;
				Modify_B_READ =R2mon_value;//������ѹֵ
				Modify_B_ACT = (UART_Buffer_Rece[3] << 8) + UART_Buffer_Rece[4];//��ȡ�߶�
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
				var32b = var32b * CLists.CREG_CorrectionR2;    	//ʶ�����2����    
				if (var32a < var32b)
				{
					var32b = var32b - var32a;
					CLists.CREG_ReadR2_Offset = var32b;    //�������ϵ��  
					CLists.CPolar7 |= 0x04;
				}
				else 
				{
					var32a = var32a - var32b;
					CLists.CREG_ReadR2_Offset = var32a;
					CLists.CPolar7 &= ~0x04;
				}
					//����д��FLASH
				Flag_DAC_OFF=0;
			}	
	
			Write_Flash_Bytes();
			
	/***********����DAC*******************************************/
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
//===============================ADֵת���ɲ���ֵ============================================//
void Transformation_ADC(void)  
{
	vu32 var32;
	vu32 var32a;
/*****************************���ز�����ѹת��******************************************/
	if(LOAD_V_SW==0)//�͵�
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
		if (var32 < 10) var32 = 0;				  //40mV��������
		Para.CVoltage = var32;
//		DISS_Voltage=Para.CVoltage;
		DISS_Voltage=(float)Para.CVoltage/1000;//������ʾ��ѹ
// 		if( DISS_Voltage >= 10 ){
// 				CLists.CLOAD_V_SW  = 1;
// 		}
// 	//���DISS_Voltage��ֵ  >=10 CLists.CLOAD_V_SW  =1
		var32 = 0;
	}
	else//�ߵ�
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
		if (var32 < 10) var32 = 0;				  //40mV��������
		Para.CVoltage = var32;
//		DISS_Voltage=Para.CVoltage;
		DISS_Voltage=(float)Para.CVoltage/1000;//������ʾ��ѹ
// 		if( DISS_Voltage < 12 ){
// 				CLists.CLOAD_V_SW  = 0;
// 		}
   //���DISS_Voltage��ֵ  <12  CLists.CLOAD_V_SW  = 0
		var32 = 0;
	}
	
	/*******************���ز�������ת��**************************************/
	if(LOAD_I_SW==0)//�͵�
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
		DISS_Current=(float)Para.CLaod_Current/1000;//������ʾ����
		var32 = 0;	
	}
	else//�ߵ�
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
		DISS_Current=(float)Para.CLaod_Current/1000;//������ʾ����
		var32 = 0;
	}
	
	/*************************���ص�ѹ�͵�������ת��**************************************/
	if(LOAD_MODE==0)//CC
	{
		if(LOAD_I_SW==0)//�͵�
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
		else//�ߵ�
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
		if(LOAD_V_SW==0)//�͵�
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
		else//�ߵ�
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
/*****************************����ֵת��*******************************************/
	if(RCONT_SW==0)//�͵�
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
//			var32 = 0;				  //����
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
	else//�ߵ�
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
//			var32 = 0;				  //����
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
	
	/*****************************��ѹ��Դ������ѹת��*******************************************/
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
//	//	if (var32 < 40) var32 = 0;				  //40mV��������
//	Para.CPOW_Voltage = var32;
//	//	DISS_POW_Voltage=Para.CPOW_Voltage;
	DISS_POW_Voltage=(float)Para.CPOW_Voltage/1000;//������ʾ��ѹ
//	var32 = 0;
/*****************************��ѹ��Դ��������ת��*******************************************/
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
		DISS_POW_Current=(float)Para.CCurrent/100;//������ʾ����
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
//		DISS_POW_Current=(float)Para.CCurrent/1000;//������ʾ����
//	}
/**************************��ѹ��Դ���õ�ѹת��******************************************/
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
/**************************��ѹ��Դ���õ���ת��**************************************/
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
	
/********************ʶ�����1ת��******************************/
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
//		var32 = 0;				  //����
//	}
	Para.CR1_VLUE = var32;
//	if(R1mon_value > 34000)
//	{
//		Para.CR1_VLUE = 100;
//	}
	var32 = 0;	
	
	
/********************ʶ�����2ת��******************************/	
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
//		var32 = 0;				  //����
//	}
	Para.CR2_VLUE = var32;
	var32 = 0;	
}
/********************************************************************************
  * ���� :   MODBUS_Conrtl
  * ���� :  zhan
  * �汾 :  V1.0
  * ���� :  2016-xx-xx
  * ���� :  MODBUS��������
  ******************************************************************************/ 
//-----------------------------CRC���--------------------------------------------//
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