/******************** (C) COPYRIGHT 2015 AVER************************
 * 文件名  ：
 * 作者    ：李振
 * 描述    ：头文件中只能有声明不可定义
 * 内容    ：
 * 修改日期：2015-8-18
*********************************************************************/
#include "my_register.h"
#ifndef _flash_h_
#define _flash_h_
/*********************函数声明*************************************/
void Init_EEPROM(void);
void EEPROM_Write_8(vu8 addr, vu8 data);
vu8 EEPROM_READ_Byte(vu8 addr_eep);
void Flash_Write_all (void);
void EEPROM_READ_Coeff(void);
void Wrtite_A_File(void);
void Wrtite_B_File(void);
void Wrtite_C_File(void);
void Wrtite_D_File(void);
void Wrtite_Beep(void);//保存BEEP状态至FLASH中
void Wrtite_SetVA(void);
void Read_FileA(void);
void Read_FileB(void);
void Read_FileC(void);
void Read_FileD(void);
void Wite_adress(void);
void Wrtite_Lock(void);
void Wrtite_MIN_File(void);
void Wrtite_MAX_File(void);
void Read_Test_File(void);
void Wrtite_Time_File(void);
void Wrtite_Door_File(void);
void Wrtite_Zore(void);
/******************************************************************/
#endif
/******************* (C) COPYRIGHT 2015 AVER *****END OF FILE***************************/