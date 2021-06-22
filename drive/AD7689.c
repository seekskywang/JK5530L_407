#include "AD7689.h"
#include "GPIO.h"
#include "my_register.h" 
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"

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
#define AD7689_CNV_0			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
#define AD7689_CNV_1			GPIO_SetBits(GPIOD,GPIO_Pin_2);
vu16 AD7689_Buffer[8];
vu16 Ad7689_Fit_Imon[100];
vu16 Ad7689_Fit_Vmon[10];
u16 Ad7689_Fit_Rmon[100];
void Bubble_sort(vu16 *D_temp,vu8 num);
void AD7689_InitializeSPI1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  /*!< SD_SPI_CS_GPIO, SD_SPI_MOSI_GPIO, SD_SPI_MISO_GPIO, SD_SPI_DETECT_GPIO 
       and SD_SPI_SCK_GPIO Periph clock enable 
	// 	 RCC_AHBPeriphClockCmd(FLASH_CS_PIN_SCK|FLASH_SCK_PIN_SCK|FLASH_MISO_PIN_SCK | FLASH_MOSI_PIN_SCK, ENABLE);
  !< SD_SPI Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//inv
	GPIO_Init(GPIOD, &GPIO_InitStructure);//
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PC2复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PBC3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB13复用为 SPI1
	
	AD7689_CNV_1;
	AD7689_Delay();
	AD7689_CNV_0;
	//这里只针对SPI口初始化
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//设置SPI的数据大小:SPI发送接收16位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
}

vu16 SPI_AD7689_Read(vu16 AD7689_CFG, vu16 AD7689_INx)
{
	vu16 AD7689_Config=0;
	vu16 AD7689_Data;
	vu8 i;
	AD7689_Config = AD7689_CFG<<13|AD7689_INCC<<10|AD7689_INx<<7|AD7689_BW<<6|AD7689_REF<<3|AD7689_SEQ<<1|AD7689_RB;//写入配置参数
	AD7689_Config <<=2;
	AD7689_CNV_0;
  AD7689_Data=AD7689_TransferByte(AD7689_Config);
	AD7689_Delay(); 
	AD7689_CNV_1;
	AD7689_Delay(); 
	return AD7689_Data;
}

/************************************************************************/
/* Function Name:	TM7707_TransferByte.								*/
/* Purpose:			Send of recive a byte.								*/
/* Resources:		SPI controler.										*/
/* Params:																*/
/*	@uiTxData:			The byte data will be sent.						*/
/* Return:			The byte data recived.						+		*/
/* Limitation:		None.												*/
/************************************************************************/
vu16 AD7689_TransferByte(vu16 uiTxData1)
{
	vu16 uiRxData;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1,uiTxData1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	uiRxData = SPI_I2S_ReceiveData(SPI1);
	return uiRxData;
}

void Swap(uint16_t A[], uint16_t i, uint16_t j)
{
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

//冒泡排序
void BubbleSort(uint16_t A[], uint16_t n)
{
	int i,j;
    for (j = 0; j < n - 1; j++)         // 每次最大元素就像气泡一样"浮"到数组的最后
    {
        for (i = 0; i < n - 1 - j; i++) // 依次比较相邻的两个元素,使较大的那个向后移
        {
            if (A[i] > A[i + 1])            // 如果条件改成A[i] >= A[i + 1],则变为不稳定的排序算法
            {
                Swap(A, i, i + 1);
            }
        }
    }
}
/************************************************************************/
/* Function Name:	AD7689_Delay.									*/
/* Purpose:			Wait for device DRDY signal.						*/
/* Resources:		DRDY GPIO pin.										*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void AD7689_Delay(void)
{
	vu16 count ;
  for(count = 1000; count != 0; count--);
}
void AD7689_Delay1(void)
{
	vu16 count ;
  for(count = 1000; count != 0; count--);
}
void AD7689_Scan_CH(void)
{
// 	vu8 i,f,d;
// 	vu32 sum1;
// 	vu16 var_chI,var_chV;
// 	static vu8 I_cont,V_cont;
// 	for(i=0;i<7;i++)
// 	{
// 		AD7689_Buffer[i]=SPI_AD7689_Read(1, i);
// 		R1mon_value=AD7689_Buffer[0];
// 		R2mon_value=AD7689_Buffer[1];
// 		Vmon_Load_value=AD7689_Buffer[2];
// 		Imon_Load_value=AD7689_Buffer[3];
// 		Rmon_value=AD	7689_Buffer[4];
// 		Vmon_POWE_value=AD7689_Buffer[5];
// 		Imon_POWE_value=AD7689_Buffer[6];

	vu8 i,f,d;
	vu32 sum1;
	vu16 var_chI,var_chI1,var_chV,var_chV1,var_chR;
	static vu8 I_cont,I1_cont,V_cont,R_cont,V1_cont;
	for(i=0;i<7;i++)
	{
		AD7689_Buffer[i]=SPI_AD7689_Read(1, i);
		R1mon_value=AD7689_Buffer[0];
		R2mon_value=AD7689_Buffer[1];
		Vmon_Load_value=AD7689_Buffer[2];
		var_chI=AD7689_Buffer[3];
//		Imon_Load_value = AD7689_Buffer[3];
		var_chR=AD7689_Buffer[4];
		Vmon_POWE_value=AD7689_Buffer[5];
		Imon_POWE_value=AD7689_Buffer[6];
	}
	
	Ad7689_Fit_Rmon[R_cont++]=var_chR;
	if(R_cont==100)
	{
		R_cont=0;
		sum1=0;
		BubbleSort(Ad7689_Fit_Rmon, 100);
		for(f=40;f<50;f++)
		{
			sum1 +=Ad7689_Fit_Rmon[f];
		}
		Rmon_value=sum1/10;//鍐呴樆
	}

	Ad7689_Fit_Imon[I_cont++] = var_chI;
	if(I_cont==30)
	{
		I_cont=0;
		sum1=0;
		for(f=0;f<30;f++)
		{
			sum1 +=Ad7689_Fit_Imon[f];
		}
		Imon_Load_value=sum1/30;//鍐呴樆
	}
//	if(I_cont==10)
//	{
//		I_cont=0;
//		sum1=0;
//		for(f=0;f<10;f++)
//		{
//			sum1 +=Ad7689_Fit_Imon[f];
//		}
//		Imon_Load_value=sum1/10;//鍐呴樆
//	}
}