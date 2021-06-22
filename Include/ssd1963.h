#include "my_register.h"
#include "stm32f4xx.h"
#ifndef  SSD1963_H
#define  SSD1963_H

#define Bank1_LCD_C      ((u32) 0x60000000) //disp Reg ADDR
#define Bank1_LCD_D      ((u32) 0x60020000) //disp Data ADDR

void sLCD_GPIO_Config(void);
void sLCD_FSMC_Config(void);
void sLCD_WR_REG(unsigned int index);
void sLCD_WR_Data(unsigned int val);
vu16 sLCD_Read_Data(void);

void LCD_Initializtion(void);
void LCD_SetPoint(vu16 x,vu16 y,vu16 c);
vu16 LCD_GetPoint(vu16 x, vu16 y);

#endif

