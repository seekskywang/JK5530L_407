/******************************************************************/
/* ���ƣHCLOCK                                                   */
/* Ч������ϵͳHSI��Ϊϵͳʱ�Ӳ�����ϵͳʱ��Ϊ48M                                                       */
/* ���ݣ�                                                        */
/* ���ߣ�����                                                    */
/* ��ϵ��ʽ��QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_rcc.h"
/*****************************************************************/
vu8 HSEStartUpStatus;
void RCC_Configuration(void)
{
  RCC_DeInit();
 
  RCC_HSEConfig(RCC_HSE_ON); 
 
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
  if(HSEStartUpStatus == SUCCESS)        
  {
		RCC_PLLCmd(DISABLE);    //配置PLL前应先关闭主PLL
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    //选择PLL时钟为系统时钟
		RCC_HCLKConfig(RCC_SYSCLK_Div1);    //HCLK（AHB）时钟为系统时钟1分频
		RCC_PCLK1Config(RCC_HCLK_Div4);    //PCLK（APB1）时钟为HCLK时钟8分频
		RCC_PCLK2Config(RCC_HCLK_Div2);    //PCLK（APB2）时钟为HCLK时钟2分频
		RCC_PLLConfig(RCC_PLLSource_HSE,4,168,2,4);    //PLL时钟配置，外部晶振为8MHz，系统配置为168MHz
		RCC_PLLCmd(ENABLE);    //PLL时钟开启
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      
		 {
		 }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
    while(RCC_GetSYSCLKSource() != 0x08)
		 { 
		 }
	 }
}
