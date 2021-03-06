/******************************************************************/
/* 名称CLOCK                                                   */
/* 效果：将系统HSI作为系统时钟并设置系统时钟为48M                                                       */
/* 内容：                                                        */
/* 作者：李振                                                    */
/* 联系方式：QQ:363116119                                        */
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
		RCC_PLLCmd(DISABLE);    //閰嶇疆PLL鍓嶅簲鍏堝叧闂富PLL
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    //閫夋嫨PLL鏃堕挓涓虹郴缁熸椂閽?
		RCC_HCLKConfig(RCC_SYSCLK_Div1);    //HCLK锛圓HB锛夋椂閽熶负绯荤粺鏃堕挓1鍒嗛
		RCC_PCLK1Config(RCC_HCLK_Div4);    //PCLK锛圓PB1锛夋椂閽熶负HCLK鏃堕挓8鍒嗛
		RCC_PCLK2Config(RCC_HCLK_Div2);    //PCLK锛圓PB2锛夋椂閽熶负HCLK鏃堕挓2鍒嗛
		RCC_PLLConfig(RCC_PLLSource_HSE,4,168,2,4);    //PLL鏃堕挓閰嶇疆锛屽閮ㄦ櫠鎸负8MHz锛岀郴缁熼厤缃负168MHz
		RCC_PLLCmd(ENABLE);    //PLL鏃堕挓寮?鍚?
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      
		 {
		 }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
    while(RCC_GetSYSCLKSource() != 0x08)
		 { 
		 }
	 }
}
