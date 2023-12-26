/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "bsp_SysTick.h"
#include "my_register.h"
extern __IO int32_t OS_TimeMS;
u8 g_mods_timeout = 0;
struct MODS_T g_tModS;
float v;
float shortv;
u8 sendmodeflag;
u8 rmtrig[3];
vu8 mode_sw;
extern vu32 ctime;
extern vu32 dctime;
static void MODS_03H(void);
static void MODS_06H(void);
static void MODS_50H(void);
static void MODS_51H(void);
static void MODS_52H(void);
static void MODS_SendAckErr(uint8_t _ucErrCode);
static uint8_t MODS_Load(uint16_t reg_addr, uint16_t reg_value);
static uint8_t MODS_Pow(uint16_t reg_addr, uint16_t reg_value);
static uint8_t MODS_CDC(uint16_t reg_addr, uint16_t reg_value);
static uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value);
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
} flagA,flagB,flagC,flagD,flagE,flagF;
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	OS_TimeMS++;
//	t_KeyScan++;
//	if(t_KeyScan>10)
//	{
//		t_KeyScan=0;
//		Flag_Key_Scan=1;//����������־
//	}
	t_0_5S++;
	if(t_0_5S>20)
	{
		t_0_5S=0;
		Flag_0_5S =1;//��������һ����־
	}
//--------------------------------------------
	if(flag_Tim_USART==1)//�����������
	{
		t_USART++;
	}
	if(t_USART>40)//��Լ2.6ms
	{
		t_USART=0;
		flag_Tim_USART=0;
		UART_Buffer_Size=0;	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_SendAckErr
*	����˵��: ���ʹ���Ӧ��
*	��    ��: _ucErrCode : �������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MODS_SendAckErr(uint8_t _ucErrCode)
{
	uint8_t txbuf[3];

	txbuf[0] = g_tModS.RxBuf[0];					/* 485��ַ */
	txbuf[1] = g_tModS.RxBuf[1] | 0x80;				/* �쳣�Ĺ����� */
	txbuf[2] = _ucErrCode;							/* �������(01,02,03,04) */

	MODS_SendWithCRC(txbuf, 3);
}


/*
*********************************************************************************************************
*	�� �� ��: MODS_SendAckOk
*	����˵��: ������ȷ��Ӧ��.
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MODS_SendAckOk(void)
{
	uint8_t txbuf[6];
	uint8_t i;

	for (i = 0; i < 6; i++)
	{
		txbuf[i] = g_tModS.RxBuf[i];
	}
	MODS_SendWithCRC(txbuf, 6);
}

/*
*********************************************************************************************************
*	�� �� ��: BEBufToUint16
*	����˵��: ��2�ֽ�����(���Big Endian���򣬸��ֽ���ǰ)ת��Ϊ16λ����
*	��    ��: _pBuf : ����
*	�� �� ֵ: 16λ����ֵ
*
*   ���(Big Endian)��С��(Little Endian)
*********************************************************************************************************
*/
uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void uart1SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { 
       USART_SendData(USART1,*(str + k));
       while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
       k++; 
       }   //ѭ������,ֱ���������   
    while (k < strlen); 
} 


 // CRC ��λ�ֽ�ֵ��
static const uint8_t s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
// CRC ��λ�ֽ�ֵ��
const uint8_t s_CRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};



void  DMA2_Stream0_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	Flag_ADC_Full=1;
}

void  TIM6_DAC_IRQHandler (void)
{
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//?????��
}

void USART1_IRQHandler(void)
{
//	flag_Tim_USART=1;
    
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		
        g_mods_timeout = 2;
        if (g_tModS.RxCount < S_RX_BUF_SIZE)
        {
            g_tModS.RxBuf[g_tModS.RxCount++] = USART_ReceiveData(USART1);
        }
		if(g_tModS.RxCount > 7 && g_tModS.RxBuf[1] == 6)
		{
			MODS_SendAckOk();
		}
	}
}



void RecHandle(void)
{
    u8 crec[6];
    u8 *csend;
    static u8 *sendbuf;
    u8 sendlen;
    static u16 recrc;
    static u16 scrc;
    u8 i;
    lockflag = 1;
    switch(g_tModS.RxBuf[1])
    {
        case 0x03:
        {
            MODS_03H();
        }break;
        case 0x06:
        {
            MODS_06H();
        }break;
		case 0x50:
        {
            MODS_50H();
        }break;
		case 0x51:
        {
            MODS_51H();
        }break;
		case 0x52:
        {
            MODS_52H();
        }break;
        default:break;
    }
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_ReadRegValue
*	����˵��: ��ȡ���ּĴ�����ֵ
*	��    ��: reg_addr �Ĵ�����ַ
*			  reg_value ��żĴ������
*	�� �� ֵ: 1��ʾOK 0��ʾ����
*********************************************************************************************************
*/
static uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value)
{
    uint16_t value;
	switch (reg_addr)									/* �жϼĴ�����ַ */
	{
        case SLAVE_REG_P00:
//			C_SW(1);
			v= DISS_Voltage;
			value =	Para.CR_VLUE;	
			break;
		case SLAVE_REG_P01:
            shortv = DISS_Voltage;
			value =	Para.CVoltage;	
			break;

		case SLAVE_REG_P02:
			value =	(int)(DISS_Current*1000);       /* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P03: 
			value =	(int)(DISS_POW_Voltage*100);
			break;

		case SLAVE_REG_P04:
			value =	(int)(DISS_POW_Current*1000);		/* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P05:
			Off_GPOI_ResetSet();
            Para.CSET_Current_Laod = 0;
			Para.IR_Step_C = 10;
			mainswitch=1;
			MODE = 2;
			MODE_ONOFF(1);
//            while(usartocflag == 1);
            value = (int)(max_discahrgeNum*1000);
			break;
		case SLAVE_REG_P06:
			value = 0;							/* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P07:
			value =	Para.CR1_VLUE;							/* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P08:
			value =	8;	
			break;

		case SLAVE_REG_P09:
			value =	Para.CR2_VLUE;							/* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P10:
			value =	8;	
			break;

		case SLAVE_REG_P11:
			value =	0;							/* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P12:
			value =	0;	
			break;

		case SLAVE_REG_P13:
			value =	0;							/* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P14:
			value =	Para.CLOAD_MODE;	
			break;

		case SLAVE_REG_P15:
			value =	Para.LOAD_V;							/* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P16:
			value =	Para.LOAD_C;
			break;

		case SLAVE_REG_P17:
			value =	Para.POWER_OutPut_V;							/* ���Ĵ���ֵ���� */
			break;
		case SLAVE_REG_P18:
			value =	Para.POWER_Limit_C;							/* ���Ĵ���ֵ���� */
			break;	
		default:
			return 0;
    }
	reg_value[0] = value >> 8;
	reg_value[1] = value;

	return 1;											/* ��ȡ�ɹ� */
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_WriteRegValue
*	����˵��: ��ȡ���ּĴ�����ֵ
*	��    ��: reg_addr �Ĵ�����ַ
*			  reg_value �Ĵ���ֵ
*	�� �� ֵ: 1��ʾOK 0��ʾ����
*********************************************************************************************************
*/
static uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value)
{

	switch (reg_addr)							/* �жϼĴ�����ַ */
	{	
		case SLAVE_REG_P00://负载模式
//			if(reg_value == 0)
//			{
//				LOAD_MODE = 0;
//			}else if(reg_value == 1){
//				LOAD_MODE = 1;
//			}else if(reg_value == 2){
//				LOAD_MODE = 2;
//			}
			Para.CLOAD_MODE=reg_value;
			break;
		case SLAVE_REG_P01://负载电压
			Para.LOAD_V = reg_value;

			break;
		
		case SLAVE_REG_P02://负载电流
			Para.LOAD_C = reg_value*10;
			break;
		case SLAVE_REG_P03://输出电压
			Para.POWER_OutPut_V = reg_value*10;
			break;

		case SLAVE_REG_P04://限制电流
			Para.POWER_Limit_C = reg_value;

			break;
		case SLAVE_REG_P05://充放电输出电�?
			Para.CDC_OutPut_V = reg_value*10;

			break;

		case SLAVE_REG_P06://充放限制电流
			Para.CDC_Limit_C = reg_value;

			break;
		case SLAVE_REG_P07://充电截止电压
			Para.CDC_Ccutoff_V = reg_value*10;
			break;
		case SLAVE_REG_P08://充电截止电流
			Para.CDC_Ccutoff_C = reg_value;

			break;
		case SLAVE_REG_P09:	//充放电负载电�?
			Para.CDC_Load_C = reg_value*10;

			break;
		case SLAVE_REG_P10:	//放电截止电压
			Para.CDC_Dcutoff_V = reg_value;

			break;

		case SLAVE_REG_P11://循环次数
			Para.CDC_Cycle_Time = reg_value;

			break;
		case SLAVE_REG_P12:
//			Para.LOAD_P = reg_value * 10;
			break;

		case SLAVE_REG_P13:
			break;
		case SLAVE_REG_P14://����ʱ��
			Para.CDC_Gap_Time = reg_value;
			break;

		case SLAVE_REG_P15:
			break;
		case SLAVE_REG_P16:
			break;

		case SLAVE_REG_P17:
			break;
		case SLAVE_REG_P18:
			Para.LOAD_P = reg_value * 10;
			break;
		case SLAVE_REG_P19:
			Para.CaPCTIME = reg_value*6;//ת��Ϊs
			break;
		case SLAVE_REG_P20:
			Para.CaPDCTIME = reg_value*6;//ת��Ϊs
			break;
		default:
			return 0;
	}

	return 1;
}

static uint8_t MODS_Load(uint16_t reg_addr, uint16_t reg_value)
{
	switch (reg_addr)							
	{	
		case SLAVE_REG_P00://开始停止负�?
			if(reg_value == 1)//开始负�?
			{
				MODE = 0;
				MODE_PARASET(MODE);
				MODE_ONOFF(1);
				mainswitch = 1;
				rmtrig[0] = 1;
			}else if(reg_value == 4){//停止负载
				MODE = 0;
				MODE_ONOFF(0);
				mainswitch = 0;
				rmtrig[0] = 0;
			}
			break;
		case SLAVE_REG_P01://

			break;
		
		case SLAVE_REG_P02://

			break;
		case SLAVE_REG_P03://

			break;

		case SLAVE_REG_P04://

			break;
		case SLAVE_REG_P05://

			break;

		case SLAVE_REG_P06://

			break;
		case SLAVE_REG_P07://

			break;
		case SLAVE_REG_P08://

			break;
		case SLAVE_REG_P09:	//

			break;
		case SLAVE_REG_P10:	//

			break;

		case SLAVE_REG_P11://

			break;
		case SLAVE_REG_P12:
			break;

		case SLAVE_REG_P13:
			break;
		case SLAVE_REG_P14:
			break;

		case SLAVE_REG_P15:
			break;
		case SLAVE_REG_P16:
			break;

		case SLAVE_REG_P17:
			break;
		default:
			return 0;
	}

	return 1;
}

static uint8_t MODS_Pow(uint16_t reg_addr, uint16_t reg_value)
{

	switch (reg_addr)							
	{	
		case SLAVE_REG_P00://开始停止电�?
			if(reg_value == 1)//开始电�?
			{
				MODE = 1;
				Para.CSET_Voltage = Para.POWER_OutPut_V;
				Para.CSET_Current = Para.POWER_Limit_C;
				mainswitch = 1;
				MODE_ONOFF(1);
				rmtrig[1] = 1;
					
			}else if(reg_value == 4){//停止电源
				MODE = 1;
				mainswitch = 0;
				MODE_ONOFF(1);
				rmtrig[1] = 0;
			}
			break;
		case SLAVE_REG_P01://

			break;
		
		case SLAVE_REG_P02://

			break;
		case SLAVE_REG_P03://

			break;

		case SLAVE_REG_P04://

			break;
		case SLAVE_REG_P05://

			break;

		case SLAVE_REG_P06://

			break;
		case SLAVE_REG_P07://

			break;
		case SLAVE_REG_P08://

			break;
		case SLAVE_REG_P09:	//

			break;
		case SLAVE_REG_P10:	//

			break;

		case SLAVE_REG_P11://

			break;
		case SLAVE_REG_P12:
			break;

		case SLAVE_REG_P13:
			break;
		case SLAVE_REG_P14:
			break;

		case SLAVE_REG_P15:
			break;
		case SLAVE_REG_P16:
			break;

		case SLAVE_REG_P17:
			break;
		default:
			return 0;
	}

	return 1;
}

static uint8_t MODS_CDC(uint16_t reg_addr, uint16_t reg_value)
{

	switch (reg_addr)							
	{	
		case SLAVE_REG_P00://开始停止电�?
			if(reg_value == 1)//开始电�?
			{
				MODE = 5;
//				GPIO_SetBits(GPIOE,GPIO_Pin_2);
				Para.CSET_Voltage = Para.CDC_OutPut_V;
				Para.CSET_Current = Para.CDC_Limit_C;
				Para.CSET_Current_Laod = Para.CDC_Load_C;
				mainswitch = 1;
				listsend = 1;
//			    GPIO_SetBits(GPIOC,GPIO_Pin_1);
				rmtrig[2] = 1;
				mode_sw = 0;
				sendmodeflag = 1;
				charge_step = 1;
				loop = 1;
				ctime = 0;
				dctime = 0;
				cdcswdelay = 5000;
			}else if(reg_value == 4){//停止电源
				Off_GPOI_ResetSet();
				mainswitch = 0;
				listsend = 1;
			    rmtrig[2] = 0;
				CDC_CsumMah = 0;
				CDC_DCsumMah = 0;
				ctime = 0;
				dctime = 0;
			}
			break;
		case SLAVE_REG_P01://

			break;
		
		case SLAVE_REG_P02://

			break;
		case SLAVE_REG_P03://

			break;

		case SLAVE_REG_P04://

			break;
		case SLAVE_REG_P05://

			break;

		case SLAVE_REG_P06://

			break;
		case SLAVE_REG_P07://

			break;
		case SLAVE_REG_P08://

			break;
		case SLAVE_REG_P09:	//

			break;
		case SLAVE_REG_P10:	//

			break;

		case SLAVE_REG_P11://

			break;
		case SLAVE_REG_P12:
			break;

		case SLAVE_REG_P13:
			break;
		case SLAVE_REG_P14:
			break;

		case SLAVE_REG_P15:
			break;
		case SLAVE_REG_P16:
			break;

		case SLAVE_REG_P17:
			break;
		default:
			return 0;
	}

	return 1;
}

static void MODS_03H(void)
{
    uint16_t reg;
	uint16_t num;
	uint16_t i;
	uint8_t reg_value[64];

    
    g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)								/* 03H���������8���ֽ� */
	{
		g_tModS.RspCode = RSP_ERR_VALUE;					/* ����ֵ����� */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 				/* �Ĵ����� */
	num = BEBufToUint16(&g_tModS.RxBuf[4]);					/* �Ĵ������� */
	if (num > sizeof(reg_value) / 2)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;					/* ����ֵ����� */
		goto err_ret;
	}

	for (i = 0; i < num; i++)
	{
		if (MODS_ReadRegValue(reg, &reg_value[2 * i]) == 0)	/* �����Ĵ���ֵ����reg_value */
		{
			g_tModS.RspCode = RSP_ERR_REG_ADDR;				/* �Ĵ�����ַ���� */
			break;
		}
		reg++;
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)							/* ��ȷӦ�� */
	{
		g_tModS.TxCount = 0;
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[0];
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[1];
		g_tModS.TxBuf[g_tModS.TxCount++] = num * 2;			/* �����ֽ��� */

		for (i = 0; i < num; i++)
		{
			g_tModS.TxBuf[g_tModS.TxCount++] = reg_value[2*i];
			g_tModS.TxBuf[g_tModS.TxCount++] = reg_value[2*i+1];
		}
//         if(usartocflag == 0)
//         {
//             MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);	/* ������ȷӦ�� */
//         }
	}
	else
	{
//		MODS_SendAckErr(g_tModS.RspCode);					/* ���ʹ���Ӧ�� */
	}
    
}


void MODS_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen)
{
	uint16_t crc;
	uint8_t i;
	uint8_t buf[S_TX_BUF_SIZE];
	memcpy(buf, _pBuf, _ucLen);
	crc = CRC16(_pBuf, _ucLen);
	buf[_ucLen++] = crc >> 8;
	buf[_ucLen++] = crc;
//ͨ��485������
//	RS485_SendBuf(buf, _ucLen);
	for(i=0;i<_ucLen;i++)
	{
		Usart_SendByte(USART1,buf[i]);
	}
//	uart1SendChars(buf, _ucLen);
	
// #if 1									/* �˲���Ϊ�˴��ڴ�ӡ���,ʵ�������пɲ�Ҫ */
// 	g_tPrint.Txlen = _ucLen;
// 	memcpy(g_tPrint.TxBuf, buf, _ucLen);
// #endif
}

static void MODS_06H(void)
{
    uint16_t reg;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		/* ����ֵ����� */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* �Ĵ����� */
	value = BEBufToUint16(&g_tModS.RxBuf[4]);	/* �Ĵ���ֵ */
    
//    if(reg == 0x0E)
//    {
//        if(value == 00)
//        {
//            GPIO_SetBits(GPIOC,GPIO_Pin_10);//CC
//            flag_Load_CC = 1;
//        }else if(value == 01){
//            GPIO_ResetBits(GPIOC,GPIO_Pin_10);//CV
//            flag_Load_CC = 0;
//        }
//    }
 	if (MODS_WriteRegValue(reg, value) == 1)	/* �ú������д���ֵ����Ĵ��� */
 	{
 		;
 	}
 	else
 	{
 		g_tModS.RspCode = RSP_ERR_REG_ADDR;		/* �Ĵ�����ַ���� */
 	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)				/* ��ȷӦ�� */
	{
		
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);		/* ��������������� */
	}
}

static void MODS_50H(void)
{
    uint16_t reg;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]);
	value = BEBufToUint16(&g_tModS.RxBuf[4]);
    

 	if (MODS_Load(reg, value) == 1)
 	{
 		;
 	}
 	else
 	{
 		g_tModS.RspCode = RSP_ERR_REG_ADDR;
 	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)
	{
		MODS_SendAckOk();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);
	}
}

static void MODS_51H(void)
{
    uint16_t reg;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]);
	value = BEBufToUint16(&g_tModS.RxBuf[4]);
    

 	if (MODS_Pow(reg, value) == 1)
 	{
 		;
 	}
 	else
 	{
 		g_tModS.RspCode = RSP_ERR_REG_ADDR;
 	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)
	{
		MODS_SendAckOk();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);
	}
}

static void MODS_52H(void)
{
    uint16_t reg;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]);
	value = BEBufToUint16(&g_tModS.RxBuf[4]);
    

 	if (MODS_CDC(reg, value) == 1)
 	{
		
 	}
 	else
 	{
 		g_tModS.RspCode = RSP_ERR_REG_ADDR;
 	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)
	{
		MODS_SendAckOk();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);
	}
}

uint16_t CRC16(uint8_t *_pBuf, uint16_t _usLen)
{    
    uint8_t ucCRCHi = 0xFF; /* ��CRC�ֽڳ�ʼ�� */
	uint8_t ucCRCLo = 0xFF; /* ��CRC �ֽڳ�ʼ�� */
	uint16_t usIndex;  /* CRCѭ���е����� */

    while (_usLen--)
    {
		usIndex = ucCRCHi ^ *_pBuf++; /* ����CRC */
		ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
		ucCRCLo = s_CRCLo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
