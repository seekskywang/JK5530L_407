/******************************************************************/
/* ÂêçÁß∞ÔºöADC0                                                    */
/* ÊïàÊûúÔºö                                                        */
/* ÂÜÖÂÆπÔºö                                                       */
/* ‰ΩúËÄÖÔºözhan                                                  */
/* ËÅîÁ≥ªÊñπÂºèQQ:363116119                                        */
/******************************************************************/
#include "adc.h"
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
/*****************************************************************/
#define ADC1_DR_Address       0x4001204C
vu16 NTC_value;//NTC
vu16 Vmon_POWE_value;//µÁ‘¥µÁ—π
vu16 Imon_POWE_value;//µÁ‘¥µÁ¡˜
vu16 Imon_Load_value;//∏∫‘ÿµÁ¡˜
vu16 Vmon_Load_value;//∏∫‘ÿµÁ—π
vu16 Rmon_value;//ƒ⁄◊Ë
vu16 R1mon_value;// ∂±µÁ◊Ë1
vu16 R2mon_value;// ∂±µÁ◊Ë2
vu16 Contr_Voltage;//Œ»—πµÁ‘¥µÁ—πøÿ÷∆DAC
vu16 Contr_Current;//µÁ‘¥µÁ¡˜øÿ÷∆DAC
vu16 Contr_Laod;//∏∫‘ÿDAC
vu16 ADC1_Buffer[100];
/*****************************************************************/
void ADC1_DMA_Init(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	
		/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* DMA1 clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  /* Configure PA.0  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//
  /* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (vu32)ADC1_DR_Address;
  DMA_InitStructure.DMA_Memory0BaseAddr = (vu32)&ADC1_Buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize =100;//¡¨–¯◊™ªª100¥Œ
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  	
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA2_Stream0, ENABLE);
	/* ÂºÄÂêØDAM1ÈÄöÈÅì1‰∏≠Êñ≠ */
	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
  DAM1_ADC_NVIC();//DMA÷–∂œ≈‰÷√
  ADC_DMACmd(ADC1, ENABLE);  // πƒ‹DMA
  
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;			//
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				//
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConvEdge_None;	//
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//
	ADC_InitStructure.ADC_NbrOfConversion = 3;	 								//
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*≈‰÷√ADC ±÷”*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
  /* ADC Calibration */
  ADC_Cmd(ADC1, ENABLE);// πƒ‹ADC
	ADC_SoftwareStartConv(ADC1);   //ø™ º◊™ªª
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
}
/***************************************************************/
void DAM1_ADC_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the adc1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************/
void ADC_CH_Scan(void)//Êª§Ê≥¢
{
	vu8 count;
	vu32 sum;
	sum=0;
/**********NTC**********************/
	for(count=0;count<100;count++)
	{
		sum +=ADC1_Buffer[count];
	}
	NTC_value=sum/50;
}