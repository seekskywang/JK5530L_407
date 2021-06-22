/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * 文件名  ：FLASH.C
 * 作者    ：李振
 * 描述    ：24C02 FLASH应用函数
 * 内容    ：
 * 硬件连接：
 * 修改日期：2014-12-22
********************************************************************/
#include "my_register.h" //NOP指令在这
#include "AT24C01.h"
//================================================================//
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
} flagA, flagB,flagC,flagD;
//==================================================================//
void Flash_Write_all (void)
{
	vu8 data_8bit;
/***********************测量数据存储****************************/
  data_8bit = REG_Load_CorrectionV_LOW >> 24;
	EEPROM_WriteByte(0x01, data_8bit);
	data_8bit = REG_Load_CorrectionV_LOW >> 16;
	EEPROM_WriteByte(0x02, data_8bit);
	data_8bit = REG_Load_CorrectionV_LOW >> 8;
	EEPROM_WriteByte(0x03, data_8bit);
	EEPROM_WriteByte(0x04, REG_Load_CorrectionV_LOW);//测量电压
	
	data_8bit = REG_Load_CorrectionV_HIG >> 24;
	EEPROM_WriteByte(0x05, data_8bit);
	data_8bit = REG_Load_CorrectionV_HIG >> 16;
	EEPROM_WriteByte(0x06, data_8bit);
	data_8bit = REG_Load_CorrectionV_HIG >> 8;
	EEPROM_WriteByte(0x07, data_8bit);
	EEPROM_WriteByte(0x08, REG_Load_CorrectionV_HIG);//测量电压
	
	
	data_8bit = REG_Load_CorrectionA_LOW >> 24;
	EEPROM_WriteByte(0x09, data_8bit);
	data_8bit = REG_Load_CorrectionA_LOW >> 16;
	EEPROM_WriteByte(0x0A, data_8bit);
	data_8bit = REG_Load_CorrectionA_LOW >> 8;
	EEPROM_WriteByte(0x0B, data_8bit);
	EEPROM_WriteByte(0x0C, REG_Load_CorrectionA_LOW);//测量电压
	
	data_8bit = REG_Load_CorrectionA_HIG >> 24;
	EEPROM_WriteByte(0x0D, data_8bit);
	data_8bit = REG_Load_CorrectionA_HIG >> 16;
	EEPROM_WriteByte(0x0E, data_8bit);
	data_8bit = REG_Load_CorrectionA_HIG >> 8;
	EEPROM_WriteByte(0x0F, data_8bit);
	EEPROM_WriteByte(0x10, REG_Load_CorrectionA_HIG);//测量电压
	
	data_8bit = SET_Load_CorrectionV_LOW >> 24;
	EEPROM_WriteByte(0x11, data_8bit);
	data_8bit = SET_Load_CorrectionV_LOW >> 16;
	EEPROM_WriteByte(0x12, data_8bit);
	data_8bit = SET_Load_CorrectionV_LOW >> 8;
	EEPROM_WriteByte(0x13, data_8bit);
	EEPROM_WriteByte(0x14, SET_Load_CorrectionV_LOW);//测量电压
	
	data_8bit = SET_Load_CorrectionV_HIG >> 24;
	EEPROM_WriteByte(0x15, data_8bit);
	data_8bit = SET_Load_CorrectionV_HIG >> 16;
	EEPROM_WriteByte(0x16, data_8bit);
	data_8bit = SET_Load_CorrectionV_HIG >> 8;
	EEPROM_WriteByte(0x17, data_8bit);
	EEPROM_WriteByte(0x18, SET_Load_CorrectionV_HIG);//测量电压
	
	data_8bit = SET_Load_CorrectionA_LOW >> 24;
	EEPROM_WriteByte(0x19, data_8bit);
	data_8bit = SET_Load_CorrectionA_LOW >> 16;
	EEPROM_WriteByte(0x1A, data_8bit);
	data_8bit = SET_Load_CorrectionA_LOW >> 8;
	EEPROM_WriteByte(0x1B, data_8bit);
	EEPROM_WriteByte(0x1C, SET_Load_CorrectionA_LOW);//负载电流
	
	data_8bit = SET_Load_CorrectionA_HIG >> 24;
	EEPROM_WriteByte(0x1D, data_8bit);
	data_8bit = SET_Load_CorrectionA_HIG >> 16;
	EEPROM_WriteByte(0x1E, data_8bit);
	data_8bit = SET_Load_CorrectionA_HIG >> 8;
	EEPROM_WriteByte(0x1F, data_8bit);
	EEPROM_WriteByte(0x20, SET_Load_CorrectionA_HIG);//负载电流
	
	data_8bit = REG_POWER_CorrectionV >> 24;
	EEPROM_WriteByte(0x21, data_8bit);
	data_8bit = REG_POWER_CorrectionV >> 16;
	EEPROM_WriteByte(0x22, data_8bit);
	data_8bit = REG_POWER_CorrectionV >> 8;
	EEPROM_WriteByte(0x23, data_8bit);
	EEPROM_WriteByte(0x24, REG_POWER_CorrectionV);//负载设定电流
	
	data_8bit = REG_POWER_CorrectionA_CC >> 24;
	EEPROM_WriteByte(0x25, data_8bit);
	data_8bit = REG_POWER_CorrectionA_CC >> 16;
	EEPROM_WriteByte(0x26, data_8bit);
	data_8bit = REG_POWER_CorrectionA_CC >> 8;
	EEPROM_WriteByte(0x27, data_8bit);
	EEPROM_WriteByte(0x28, REG_POWER_CorrectionA_CC);//负载设定电流
	
	data_8bit = REG_POWER_CorrectionA_CV >> 24;
	EEPROM_WriteByte(0x29, data_8bit);
	data_8bit = REG_POWER_CorrectionA_CV >> 16;
	EEPROM_WriteByte(0x2A, data_8bit);
	data_8bit = REG_POWER_CorrectionA_CV >> 8;
	EEPROM_WriteByte(0x2B, data_8bit);
	EEPROM_WriteByte(0x2C, REG_POWER_CorrectionA_CV);//负载测量电压
	
	data_8bit = SET_POWER_CorrectionV >> 24;
	EEPROM_WriteByte(0x2D, data_8bit);
	data_8bit = SET_POWER_CorrectionV >> 16;
	EEPROM_WriteByte(0x2E, data_8bit);
	data_8bit = SET_POWER_CorrectionV >> 8;
	EEPROM_WriteByte(0x2F, data_8bit);
	EEPROM_WriteByte(0x30, SET_POWER_CorrectionV);//负载测量电压
	
	data_8bit = SET_POWER_CorrectionA >> 24;
	EEPROM_WriteByte(0x31, data_8bit);
	data_8bit = SET_POWER_CorrectionA >> 16;
	EEPROM_WriteByte(0x32, data_8bit);
	data_8bit = SET_POWER_CorrectionA >> 8;
	EEPROM_WriteByte(0x33, data_8bit);
	EEPROM_WriteByte(0x34, SET_POWER_CorrectionA);//负载设定电压
	
	data_8bit = REG_CorrectionR_LOW >> 24;
	EEPROM_WriteByte(0x35, data_8bit);
	data_8bit = REG_CorrectionR_LOW >> 16;
	EEPROM_WriteByte(0x36, data_8bit);
	data_8bit = REG_CorrectionR_LOW >> 8;
	EEPROM_WriteByte(0x37, data_8bit);
	EEPROM_WriteByte(0x38, REG_CorrectionR_LOW);//负载设定电压
	
	
	data_8bit = REG_CorrectionR_HIG >> 24;
	EEPROM_WriteByte(0x39, data_8bit);
	data_8bit = REG_CorrectionR_HIG >> 16;
	EEPROM_WriteByte(0x3A, data_8bit);
	data_8bit = REG_CorrectionR_HIG >> 8;
	EEPROM_WriteByte(0x3B, data_8bit);
	EEPROM_WriteByte(0x3C, REG_CorrectionR_HIG);//内阻
	
	data_8bit = REG_CorrectionR1 >> 24;
	EEPROM_WriteByte(0x3D, data_8bit);
	data_8bit = REG_CorrectionR1 >> 16;
	EEPROM_WriteByte(0x3E, data_8bit);
	data_8bit = REG_CorrectionR1 >> 8;
	EEPROM_WriteByte(0x3F, data_8bit);
	EEPROM_WriteByte(0x40, REG_CorrectionR1);//内阻
	
	
	data_8bit = REG_CorrectionR2 >> 24;
	EEPROM_WriteByte(0x41, data_8bit);
	data_8bit = REG_CorrectionR2 >> 16;
	EEPROM_WriteByte(0x42, data_8bit);
	data_8bit = REG_CorrectionR2 >> 8;
	EEPROM_WriteByte(0x43, data_8bit);
	EEPROM_WriteByte(0x44, REG_CorrectionR2);//内阻低档
	
	/************偏差系数*******************/
	data_8bit = REG_LoadV_Offset_LOW >> 24;
	EEPROM_WriteByte(0x45, data_8bit);
	data_8bit = REG_LoadV_Offset_LOW >> 16;
	EEPROM_WriteByte(0x46, data_8bit);
	data_8bit = REG_LoadV_Offset_LOW >> 8;
	EEPROM_WriteByte(0x47, data_8bit);
	EEPROM_WriteByte(0x48, REG_LoadV_Offset_LOW);//内阻低档
	
	data_8bit = REG_LoadV_Offset_HIG >> 24;
	EEPROM_WriteByte(0x49, data_8bit);
	data_8bit = REG_LoadV_Offset_HIG >> 16;
	EEPROM_WriteByte(0x4A, data_8bit);
	data_8bit = REG_LoadV_Offset_HIG >> 8;
	EEPROM_WriteByte(0x4B, data_8bit);
	EEPROM_WriteByte(0x4C, REG_LoadV_Offset_HIG);//电源CC模式电流测量
	
	data_8bit = REG_LoadA_Offset_LOW >> 24;
	EEPROM_WriteByte(0x4D, data_8bit);
	data_8bit = REG_LoadA_Offset_LOW >> 16;
	EEPROM_WriteByte(0x4E, data_8bit);
	data_8bit = REG_LoadA_Offset_LOW >> 8;
	EEPROM_WriteByte(0x4F, data_8bit);
	EEPROM_WriteByte(0x50, REG_LoadA_Offset_LOW);//电源CC模式电流测量
	
	data_8bit = REG_LoadA_Offset_HIG >> 24;
	EEPROM_WriteByte(0x51, data_8bit);
	data_8bit = REG_LoadA_Offset_HIG >> 16;
	EEPROM_WriteByte(0x52, data_8bit);
	data_8bit = REG_LoadA_Offset_HIG >> 8;
	EEPROM_WriteByte(0x53, data_8bit);
	EEPROM_WriteByte(0x54, REG_LoadA_Offset_HIG);//电源CC模式设定电流
	
	
	data_8bit = SET_LoadV_Offset_LOW >> 24;
	EEPROM_WriteByte(0x55, data_8bit);
	data_8bit = SET_LoadV_Offset_LOW >> 16;
	EEPROM_WriteByte(0x56, data_8bit);
	data_8bit = SET_LoadV_Offset_LOW >> 8;
	EEPROM_WriteByte(0x57, data_8bit);
	EEPROM_WriteByte(0x58, SET_LoadV_Offset_LOW);//电源CC模式设定电流
	
	
	data_8bit = SET_LoadV_Offset_HIG >> 24;
	EEPROM_WriteByte(0x59, data_8bit);
	data_8bit = SET_LoadV_Offset_HIG >> 16;
	EEPROM_WriteByte(0x5A, data_8bit);
	data_8bit = SET_LoadV_Offset_HIG >> 8;
	EEPROM_WriteByte(0x5B, data_8bit);
	EEPROM_WriteByte(0x5C, SET_LoadV_Offset_HIG);//电源电压测量
	
	
	data_8bit = SET_LoadA_Offset_LOW >> 24;
	EEPROM_WriteByte(0x5D, data_8bit);
	data_8bit = SET_LoadA_Offset_LOW >> 16;
	EEPROM_WriteByte(0x5E, data_8bit);
	data_8bit = SET_LoadA_Offset_LOW >> 8;
	EEPROM_WriteByte(0x5F, data_8bit);
	EEPROM_WriteByte(0x60, SET_LoadA_Offset_LOW);//电源电压测量
	
	data_8bit = SET_LoadA_Offset_HIG >> 24;
	EEPROM_WriteByte(0x61, data_8bit);
	data_8bit = SET_LoadA_Offset_HIG >> 16;
	EEPROM_WriteByte(0x62, data_8bit);
	data_8bit = SET_LoadA_Offset_HIG >> 8;
	EEPROM_WriteByte(0x63, data_8bit);
	EEPROM_WriteByte(0x64, SET_LoadA_Offset_HIG);//电源电压控制
	
	data_8bit = REG_POWERV_Offset >> 24;
	EEPROM_WriteByte(0x65, data_8bit);
	data_8bit = REG_POWERV_Offset >> 16;
	EEPROM_WriteByte(0x66, data_8bit);
	data_8bit = REG_POWERV_Offset >> 8;
	EEPROM_WriteByte(0x67, data_8bit);
	EEPROM_WriteByte(0x68, REG_POWERV_Offset);//电源电压控制
	
	data_8bit = REG_POWERA_Offset_CC >> 24;
	EEPROM_WriteByte(0x69, data_8bit);
	data_8bit = REG_POWERA_Offset_CC >> 16;
	EEPROM_WriteByte(0x6A, data_8bit);
	data_8bit = REG_POWERA_Offset_CC >> 8;
	EEPROM_WriteByte(0x6B, data_8bit);
	EEPROM_WriteByte(0x6C, REG_POWERA_Offset_CC);//电源CV电流测量
	
	data_8bit = REG_POWERA_Offset_CV >> 24;
	EEPROM_WriteByte(0x6D, data_8bit);
	data_8bit = REG_POWERA_Offset_CV >> 16;
	EEPROM_WriteByte(0x6E, data_8bit);
	data_8bit = REG_POWERA_Offset_CV >> 8;
	EEPROM_WriteByte(0x6F, data_8bit);
	EEPROM_WriteByte(0x70, REG_POWERA_Offset_CV);//电源CV电流测量
	
	
	data_8bit = SET_POWERV_Offset >> 24;
	EEPROM_WriteByte(0x71, data_8bit);
	data_8bit = SET_POWERV_Offset >> 16;
	EEPROM_WriteByte(0x72, data_8bit);
	data_8bit = SET_POWERV_Offset >> 8;
	EEPROM_WriteByte(0x73, data_8bit);
	EEPROM_WriteByte(0x74, SET_POWERV_Offset);//电源CV电流测量
	
	data_8bit = SET_POWERA_Offset >> 24;
	EEPROM_WriteByte(0x75, data_8bit);
	data_8bit = SET_POWERA_Offset >> 16;
	EEPROM_WriteByte(0x76, data_8bit);
	data_8bit = SET_POWERA_Offset >> 8;
	EEPROM_WriteByte(0x77, data_8bit);
	EEPROM_WriteByte(0x78, SET_POWERA_Offset);//电源CV电流测量
	
	
	data_8bit = REG_ReadR_Offset_LOW >> 24;
	EEPROM_WriteByte(0x79, data_8bit);
	data_8bit = REG_ReadR_Offset_LOW >> 16;
	EEPROM_WriteByte(0x7A, data_8bit);
	data_8bit = REG_ReadR_Offset_LOW >> 8;
	EEPROM_WriteByte(0x7B, data_8bit);
	EEPROM_WriteByte(0x7C, REG_ReadR_Offset_LOW);//电源CV电流测量
	
	data_8bit = REG_ReadR_Offset_HIG >> 24;
	EEPROM_WriteByte(0x7D, data_8bit);
	data_8bit = REG_ReadR_Offset_HIG >> 16;
	EEPROM_WriteByte(0x7E, data_8bit);
	data_8bit = REG_ReadR_Offset_HIG >> 8;
	EEPROM_WriteByte(0x7F, data_8bit);
	EEPROM_WriteByte(0x80, REG_ReadR_Offset_HIG);//电源CV电流测量
	
	data_8bit = REG_ReadR1_Offset >> 24;
	EEPROM_WriteByte(0x81, data_8bit);
	data_8bit = REG_ReadR1_Offset >> 16;
	EEPROM_WriteByte(0x82, data_8bit);
	data_8bit = REG_ReadR1_Offset >> 8;
	EEPROM_WriteByte(0x83, data_8bit);
	EEPROM_WriteByte(0x84, REG_ReadR1_Offset);//电源CV电流测量
	
	data_8bit = REG_ReadR2_Offset >> 24;
	EEPROM_WriteByte(0x85, data_8bit);
	data_8bit = REG_ReadR2_Offset >> 16;
	EEPROM_WriteByte(0x86, data_8bit);
	data_8bit = REG_ReadR2_Offset >> 8;
	EEPROM_WriteByte(0x87, data_8bit);
	EEPROM_WriteByte(0x88, REG_ReadR2_Offset);//电源CV电流测量
	
	
	EEPROM_WriteByte(0x89,Polar0);
	EEPROM_WriteByte(0x8A,Polar1);//
	EEPROM_WriteByte(0x8B,Polar2);
	EEPROM_WriteByte(0x8C,Polar3);//
	EEPROM_WriteByte(0x8D,Polar4);
	EEPROM_WriteByte(0x8E,Polar5);
	EEPROM_WriteByte(0x8F,Polar6);
	EEPROM_WriteByte(0x90,Polar7);
}
//===========================================================================//
void EEPROM_READ_Coeff(void)
{
	REG_Load_CorrectionV_LOW=EEPROM_READ_Byte(0x01);
	REG_Load_CorrectionV_LOW=EEPROM_READ_Byte(0x01);
	REG_Load_CorrectionV_LOW=REG_Load_CorrectionV_LOW<<8;
	REG_Load_CorrectionV_LOW=REG_Load_CorrectionV_LOW+EEPROM_READ_Byte(0x02);
	REG_Load_CorrectionV_LOW=REG_Load_CorrectionV_LOW<<8;
	REG_Load_CorrectionV_LOW=REG_Load_CorrectionV_LOW+EEPROM_READ_Byte(0x03);
	REG_Load_CorrectionV_LOW=REG_Load_CorrectionV_LOW<<8;
	REG_Load_CorrectionV_LOW=REG_Load_CorrectionV_LOW+EEPROM_READ_Byte(0x04);
	
	REG_Load_CorrectionV_HIG=EEPROM_READ_Byte(0x05);
	REG_Load_CorrectionV_HIG=REG_Load_CorrectionV_HIG<<8;
	REG_Load_CorrectionV_HIG=REG_Load_CorrectionV_HIG+EEPROM_READ_Byte(0x06);
	REG_Load_CorrectionV_HIG=REG_Load_CorrectionV_HIG<<8;
	REG_Load_CorrectionV_HIG=REG_Load_CorrectionV_HIG+EEPROM_READ_Byte(0x07);
	REG_Load_CorrectionV_HIG=REG_Load_CorrectionV_HIG<<8;
	REG_Load_CorrectionV_HIG=REG_Load_CorrectionV_HIG+EEPROM_READ_Byte(0x08);
	
	REG_Load_CorrectionA_LOW=EEPROM_READ_Byte(0x09);
	REG_Load_CorrectionA_LOW=REG_Load_CorrectionA_LOW<<8;
	REG_Load_CorrectionA_LOW=REG_Load_CorrectionA_LOW+EEPROM_READ_Byte(0x0A);
	REG_Load_CorrectionA_LOW=REG_Load_CorrectionA_LOW<<8;
	REG_Load_CorrectionA_LOW=REG_Load_CorrectionA_LOW+EEPROM_READ_Byte(0x0B);
	REG_Load_CorrectionA_LOW=REG_Load_CorrectionA_LOW<<8;
	REG_Load_CorrectionA_LOW=REG_Load_CorrectionA_LOW+EEPROM_READ_Byte(0x0C);
	
	REG_Load_CorrectionA_HIG=EEPROM_READ_Byte(0x0D);
	REG_Load_CorrectionA_HIG=REG_Load_CorrectionA_HIG<<8;
	REG_Load_CorrectionA_HIG=REG_Load_CorrectionA_HIG+EEPROM_READ_Byte(0x0E);
	REG_Load_CorrectionA_HIG=REG_Load_CorrectionA_HIG<<8;
	REG_Load_CorrectionA_HIG=REG_Load_CorrectionA_HIG+EEPROM_READ_Byte(0x0F);
	REG_Load_CorrectionA_HIG=REG_Load_CorrectionA_HIG<<8;
	REG_Load_CorrectionA_HIG=REG_Load_CorrectionA_HIG+EEPROM_READ_Byte(0x10);
	
	SET_Load_CorrectionV_LOW=EEPROM_READ_Byte(0x11);
	SET_Load_CorrectionV_LOW=SET_Load_CorrectionV_LOW<<8;
	SET_Load_CorrectionV_LOW=SET_Load_CorrectionV_LOW+EEPROM_READ_Byte(0x12);
	SET_Load_CorrectionV_LOW=SET_Load_CorrectionV_LOW<<8;
	SET_Load_CorrectionV_LOW=SET_Load_CorrectionV_LOW+EEPROM_READ_Byte(0x13);
	SET_Load_CorrectionV_LOW=SET_Load_CorrectionV_LOW<<8;
	SET_Load_CorrectionV_LOW=SET_Load_CorrectionV_LOW+EEPROM_READ_Byte(0x14);
	
	SET_Load_CorrectionV_HIG=EEPROM_READ_Byte(0x15);
	SET_Load_CorrectionV_HIG=SET_Load_CorrectionV_HIG<<8;
	SET_Load_CorrectionV_HIG=SET_Load_CorrectionV_HIG+EEPROM_READ_Byte(0x16);
	SET_Load_CorrectionV_HIG=SET_Load_CorrectionV_HIG<<8;
	SET_Load_CorrectionV_HIG=SET_Load_CorrectionV_HIG+EEPROM_READ_Byte(0x17);
	SET_Load_CorrectionV_HIG=SET_Load_CorrectionV_HIG<<8;
	SET_Load_CorrectionV_HIG=SET_Load_CorrectionV_HIG+EEPROM_READ_Byte(0x18);//
	
	
	SET_Load_CorrectionA_LOW=EEPROM_READ_Byte(0x19);
	SET_Load_CorrectionA_LOW=SET_Load_CorrectionA_LOW<<8;
	SET_Load_CorrectionA_LOW=SET_Load_CorrectionA_LOW+EEPROM_READ_Byte(0x1A);
	SET_Load_CorrectionA_LOW=SET_Load_CorrectionA_LOW<<8;
	SET_Load_CorrectionA_LOW=SET_Load_CorrectionA_LOW+EEPROM_READ_Byte(0x1B);
	SET_Load_CorrectionA_LOW=SET_Load_CorrectionA_LOW<<8;
	SET_Load_CorrectionA_LOW=SET_Load_CorrectionA_LOW+EEPROM_READ_Byte(0x1C);
	
	SET_Load_CorrectionA_HIG=EEPROM_READ_Byte(0x1D);
	SET_Load_CorrectionA_HIG=SET_Load_CorrectionA_HIG<<8;
	SET_Load_CorrectionA_HIG=SET_Load_CorrectionA_HIG+EEPROM_READ_Byte(0x1E);
	SET_Load_CorrectionA_HIG=SET_Load_CorrectionA_HIG<<8;
	SET_Load_CorrectionA_HIG=SET_Load_CorrectionA_HIG+EEPROM_READ_Byte(0x1F);
	SET_Load_CorrectionA_HIG=SET_Load_CorrectionA_HIG<<8;
	SET_Load_CorrectionA_HIG=SET_Load_CorrectionA_HIG+EEPROM_READ_Byte(0x20);//
	
	REG_POWER_CorrectionV=EEPROM_READ_Byte(0x21);
	REG_POWER_CorrectionV=REG_POWER_CorrectionV<<8;
	REG_POWER_CorrectionV=REG_POWER_CorrectionV+EEPROM_READ_Byte(0x22);
	REG_POWER_CorrectionV=REG_POWER_CorrectionV<<8;
	REG_POWER_CorrectionV=REG_POWER_CorrectionV+EEPROM_READ_Byte(0x23);
	REG_POWER_CorrectionV=REG_POWER_CorrectionV<<8;
	REG_POWER_CorrectionV=REG_POWER_CorrectionV+EEPROM_READ_Byte(0x24);
	
	REG_POWER_CorrectionA_CC=EEPROM_READ_Byte(0x25);
	REG_POWER_CorrectionA_CC=REG_POWER_CorrectionA_CC<<8;
	REG_POWER_CorrectionA_CC=REG_POWER_CorrectionA_CC+EEPROM_READ_Byte(0x26);
	REG_POWER_CorrectionA_CC=REG_POWER_CorrectionA_CC<<8;
	REG_POWER_CorrectionA_CC=REG_POWER_CorrectionA_CC+EEPROM_READ_Byte(0x27);
	REG_POWER_CorrectionA_CC=REG_POWER_CorrectionA_CC<<8;
	REG_POWER_CorrectionA_CC=REG_POWER_CorrectionA_CC+EEPROM_READ_Byte(0x28);
	/*****************内阻********************************/
	REG_POWER_CorrectionA_CV=EEPROM_READ_Byte(0x29);
	REG_POWER_CorrectionA_CV=REG_POWER_CorrectionA_CV<<8;
	REG_POWER_CorrectionA_CV=REG_POWER_CorrectionA_CV+EEPROM_READ_Byte(0x2A);
	REG_POWER_CorrectionA_CV=REG_POWER_CorrectionA_CV<<8;
	REG_POWER_CorrectionA_CV=REG_POWER_CorrectionA_CV+EEPROM_READ_Byte(0x2B);
	REG_POWER_CorrectionA_CV=REG_POWER_CorrectionA_CV<<8;
	REG_POWER_CorrectionA_CV=REG_POWER_CorrectionA_CV+EEPROM_READ_Byte(0x2C);
	
	SET_POWER_CorrectionV=EEPROM_READ_Byte(0x2D);
	SET_POWER_CorrectionV=SET_POWER_CorrectionV<<8;
	SET_POWER_CorrectionV=SET_POWER_CorrectionV+EEPROM_READ_Byte(0x2E);
	SET_POWER_CorrectionV=SET_POWER_CorrectionV<<8;
	SET_POWER_CorrectionV=SET_POWER_CorrectionV+EEPROM_READ_Byte(0x2F);
	SET_POWER_CorrectionV=SET_POWER_CorrectionV<<8;
	SET_POWER_CorrectionV=SET_POWER_CorrectionV+EEPROM_READ_Byte(0x30);
	
/*******************稳压电源****************************/
	SET_POWER_CorrectionA=EEPROM_READ_Byte(0x31);
	SET_POWER_CorrectionA=SET_POWER_CorrectionA<<8;
	SET_POWER_CorrectionA=SET_POWER_CorrectionA+EEPROM_READ_Byte(0x32);
	SET_POWER_CorrectionA=SET_POWER_CorrectionA<<8;
	SET_POWER_CorrectionA=SET_POWER_CorrectionA+EEPROM_READ_Byte(0x33);
	SET_POWER_CorrectionA=SET_POWER_CorrectionA<<8;
	SET_POWER_CorrectionA=SET_POWER_CorrectionA+EEPROM_READ_Byte(0x34);
	
	REG_CorrectionR_LOW=EEPROM_READ_Byte(0x35);
	REG_CorrectionR_LOW=REG_CorrectionR_LOW<<8;
	REG_CorrectionR_LOW=REG_CorrectionR_LOW+EEPROM_READ_Byte(0x36);
	REG_CorrectionR_LOW=REG_CorrectionR_LOW<<8;
	REG_CorrectionR_LOW=REG_CorrectionR_LOW+EEPROM_READ_Byte(0x37);
	REG_CorrectionR_LOW=REG_CorrectionR_LOW<<8;
	REG_CorrectionR_LOW=REG_CorrectionR_LOW+EEPROM_READ_Byte(0x38);//
	
	REG_CorrectionR_HIG=EEPROM_READ_Byte(0x39);
	REG_CorrectionR_HIG=REG_CorrectionR_HIG<<8;
	REG_CorrectionR_HIG=REG_CorrectionR_HIG+EEPROM_READ_Byte(0x3A);
	REG_CorrectionR_HIG=REG_CorrectionR_HIG<<8;
	REG_CorrectionR_HIG=REG_CorrectionR_HIG+EEPROM_READ_Byte(0x3B);
	REG_CorrectionR_HIG=REG_CorrectionR_HIG<<8;
	REG_CorrectionR_HIG=REG_CorrectionR_HIG+EEPROM_READ_Byte(0x3C);

	REG_CorrectionR1=EEPROM_READ_Byte(0x3D);
	REG_CorrectionR1=REG_CorrectionR1<<8;
	REG_CorrectionR1=REG_CorrectionR1+EEPROM_READ_Byte(0x3E);
	REG_CorrectionR1=REG_CorrectionR1<<8;
	REG_CorrectionR1=REG_CorrectionR1+EEPROM_READ_Byte(0x3F);
	REG_CorrectionR1=REG_CorrectionR1<<8;
	REG_CorrectionR1=REG_CorrectionR1+EEPROM_READ_Byte(0x40);//
	
	
	REG_CorrectionR2=EEPROM_READ_Byte(0x41);
	REG_CorrectionR2=REG_CorrectionR2<<8;
	REG_CorrectionR2=REG_CorrectionR2+EEPROM_READ_Byte(0x42);
	REG_CorrectionR2=REG_CorrectionR2<<8;
	REG_CorrectionR2=REG_CorrectionR2+EEPROM_READ_Byte(0x43);
	REG_CorrectionR2=REG_CorrectionR2<<8;
	REG_CorrectionR2=REG_CorrectionR2+EEPROM_READ_Byte(0x44);

	REG_LoadV_Offset_LOW=EEPROM_READ_Byte(0x45);
	REG_LoadV_Offset_LOW=REG_LoadV_Offset_LOW<<8;
	REG_LoadV_Offset_LOW=REG_LoadV_Offset_LOW+EEPROM_READ_Byte(0x46);
	REG_LoadV_Offset_LOW=REG_LoadV_Offset_LOW<<8;
	REG_LoadV_Offset_LOW=REG_LoadV_Offset_LOW+EEPROM_READ_Byte(0x47);
	REG_LoadV_Offset_LOW=REG_LoadV_Offset_LOW<<8;
	REG_LoadV_Offset_LOW=REG_LoadV_Offset_LOW+EEPROM_READ_Byte(0x48);//
	
	
	REG_LoadV_Offset_HIG=EEPROM_READ_Byte(0x49);
	REG_LoadV_Offset_HIG=REG_LoadV_Offset_HIG<<8;
	REG_LoadV_Offset_HIG=REG_LoadV_Offset_HIG+EEPROM_READ_Byte(0x4A);
	REG_LoadV_Offset_HIG=REG_LoadV_Offset_HIG<<8;
	REG_LoadV_Offset_HIG=REG_LoadV_Offset_HIG+EEPROM_READ_Byte(0x4B);
	REG_LoadV_Offset_HIG=REG_LoadV_Offset_HIG<<8;
	REG_LoadV_Offset_HIG=REG_LoadV_Offset_HIG+EEPROM_READ_Byte(0x4C);

	REG_LoadA_Offset_LOW=EEPROM_READ_Byte(0x4D);
	REG_LoadA_Offset_LOW=SET_POWERV_Offset<<8;
	REG_LoadA_Offset_LOW=SET_POWERV_Offset+EEPROM_READ_Byte(0x4E);
	REG_LoadA_Offset_LOW=SET_POWERV_Offset<<8;
	REG_LoadA_Offset_LOW=SET_POWERV_Offset+EEPROM_READ_Byte(0x4F);
	REG_LoadA_Offset_LOW=SET_POWERV_Offset<<8;
	REG_LoadA_Offset_LOW=SET_POWERV_Offset+EEPROM_READ_Byte(0x50);//
	
	REG_LoadA_Offset_HIG=EEPROM_READ_Byte(0x51);
	REG_LoadA_Offset_HIG=REG_LoadA_Offset_HIG<<8;
	REG_LoadA_Offset_HIG=REG_LoadA_Offset_HIG+EEPROM_READ_Byte(0x52);
	REG_LoadA_Offset_HIG=REG_LoadA_Offset_HIG<<8;
	REG_LoadA_Offset_HIG=REG_LoadA_Offset_HIG+EEPROM_READ_Byte(0x53);
	REG_LoadA_Offset_HIG=REG_LoadA_Offset_HIG<<8;
	REG_LoadA_Offset_HIG=REG_LoadA_Offset_HIG+EEPROM_READ_Byte(0x54);

	SET_LoadV_Offset_LOW=EEPROM_READ_Byte(0x55);
	SET_LoadV_Offset_LOW=SET_LoadV_Offset_LOW<<8;
	SET_LoadV_Offset_LOW=SET_LoadV_Offset_LOW+EEPROM_READ_Byte(0x56);
	SET_LoadV_Offset_LOW=SET_LoadV_Offset_LOW<<8;
	SET_LoadV_Offset_LOW=SET_LoadV_Offset_LOW+EEPROM_READ_Byte(0x57);
	SET_LoadV_Offset_LOW=SET_LoadV_Offset_LOW<<8;
	SET_LoadV_Offset_LOW=SET_LoadV_Offset_LOW+EEPROM_READ_Byte(0x58);
	
	SET_LoadV_Offset_HIG=EEPROM_READ_Byte(0x59);
	SET_LoadV_Offset_HIG=SET_LoadV_Offset_HIG<<8;
	SET_LoadV_Offset_HIG=SET_LoadV_Offset_HIG+EEPROM_READ_Byte(0x5A);
	SET_LoadV_Offset_HIG=SET_LoadV_Offset_HIG<<8;
	SET_LoadV_Offset_HIG=SET_LoadV_Offset_HIG+EEPROM_READ_Byte(0x5B);
	SET_LoadV_Offset_HIG=SET_LoadV_Offset_HIG<<8;
	SET_LoadV_Offset_HIG=SET_LoadV_Offset_HIG+EEPROM_READ_Byte(0x5C);
	
	SET_LoadA_Offset_LOW=EEPROM_READ_Byte(0x5D);
	SET_LoadA_Offset_LOW=SET_LoadA_Offset_LOW<<8;
	SET_LoadA_Offset_LOW=SET_LoadA_Offset_LOW+EEPROM_READ_Byte(0x5E);
	SET_LoadA_Offset_LOW=SET_LoadA_Offset_LOW<<8;
	SET_LoadA_Offset_LOW=SET_LoadA_Offset_LOW+EEPROM_READ_Byte(0x5F);
	SET_LoadA_Offset_LOW=SET_LoadA_Offset_LOW<<8;
	SET_LoadA_Offset_LOW=SET_LoadA_Offset_LOW+EEPROM_READ_Byte(0x60);
	
	SET_LoadA_Offset_HIG=EEPROM_READ_Byte(0x61);
	SET_LoadA_Offset_HIG=SET_LoadA_Offset_HIG<<8;
	SET_LoadA_Offset_HIG=SET_LoadA_Offset_HIG+EEPROM_READ_Byte(0x62);
	SET_LoadA_Offset_HIG=SET_LoadA_Offset_HIG<<8;
	SET_LoadA_Offset_HIG=SET_LoadA_Offset_HIG+EEPROM_READ_Byte(0x63);
	SET_LoadA_Offset_HIG=SET_LoadA_Offset_HIG<<8;
	SET_LoadA_Offset_HIG=SET_LoadA_Offset_HIG+EEPROM_READ_Byte(0x64);
	
	REG_POWERV_Offset=EEPROM_READ_Byte(0x65);
	REG_POWERV_Offset=REG_POWERV_Offset<<8;
	REG_POWERV_Offset=REG_POWERV_Offset+EEPROM_READ_Byte(0x66);
	REG_POWERV_Offset=REG_POWERV_Offset<<8;
	REG_POWERV_Offset=REG_POWERV_Offset+EEPROM_READ_Byte(0x67);
	REG_POWERV_Offset=REG_POWERV_Offset<<8;
	REG_POWERV_Offset=REG_POWERV_Offset+EEPROM_READ_Byte(0x68);
	
	REG_POWERA_Offset_CC=EEPROM_READ_Byte(0x69);
	REG_POWERA_Offset_CC=REG_POWERA_Offset_CC<<8;
	REG_POWERA_Offset_CC=REG_POWERA_Offset_CC+EEPROM_READ_Byte(0x6A);
	REG_POWERA_Offset_CC=REG_POWERA_Offset_CC<<8;
	REG_POWERA_Offset_CC=REG_POWERA_Offset_CC+EEPROM_READ_Byte(0x6B);
	REG_POWERA_Offset_CC=REG_POWERA_Offset_CC<<8;
	REG_POWERA_Offset_CC=REG_POWERA_Offset_CC+EEPROM_READ_Byte(0x6C);
	
	REG_POWERA_Offset_CV=EEPROM_READ_Byte(0x6D);
	REG_POWERA_Offset_CV=REG_POWERA_Offset_CV<<8;
	REG_POWERA_Offset_CV=REG_POWERA_Offset_CV+EEPROM_READ_Byte(0x6E);
	REG_POWERA_Offset_CV=REG_POWERA_Offset_CV<<8;
	REG_POWERA_Offset_CV=REG_POWERA_Offset_CV+EEPROM_READ_Byte(0x6F);
	REG_POWERA_Offset_CV=REG_POWERA_Offset_CV<<8;
	REG_POWERA_Offset_CV=REG_POWERA_Offset_CV+EEPROM_READ_Byte(0x70);
	
	SET_POWERV_Offset=EEPROM_READ_Byte(0x71);
	SET_POWERV_Offset=SET_POWERV_Offset<<8;
	SET_POWERV_Offset=SET_POWERV_Offset+EEPROM_READ_Byte(0x72);
	SET_POWERV_Offset=SET_POWERV_Offset<<8;
	SET_POWERV_Offset=SET_POWERV_Offset+EEPROM_READ_Byte(0x73);
	SET_POWERV_Offset=SET_POWERV_Offset<<8;
	SET_POWERV_Offset=SET_POWERV_Offset+EEPROM_READ_Byte(0x74);
	
	SET_POWERA_Offset=EEPROM_READ_Byte(0x75);
	SET_POWERA_Offset=SET_POWERA_Offset<<8;
	SET_POWERA_Offset=SET_POWERA_Offset+EEPROM_READ_Byte(0x76);
	SET_POWERA_Offset=SET_POWERA_Offset<<8;
	SET_POWERA_Offset=SET_POWERA_Offset+EEPROM_READ_Byte(0x77);
	SET_POWERA_Offset=SET_POWERA_Offset<<8;
	SET_POWERA_Offset=SET_POWERA_Offset+EEPROM_READ_Byte(0x78);
	
	REG_ReadR_Offset_LOW=EEPROM_READ_Byte(0x79);
	REG_ReadR_Offset_LOW=REG_ReadR_Offset_LOW<<8;
	REG_ReadR_Offset_LOW=REG_ReadR_Offset_LOW+EEPROM_READ_Byte(0x7A);
	REG_ReadR_Offset_LOW=REG_ReadR_Offset_LOW<<8;
	REG_ReadR_Offset_LOW=REG_ReadR_Offset_LOW+EEPROM_READ_Byte(0x7B);
	REG_ReadR_Offset_LOW=REG_ReadR_Offset_LOW<<8;
	REG_ReadR_Offset_LOW=REG_ReadR_Offset_LOW+EEPROM_READ_Byte(0x7C);
	
	REG_ReadR_Offset_HIG=EEPROM_READ_Byte(0x7D);
	REG_ReadR_Offset_HIG=REG_ReadR_Offset_HIG<<8;
	REG_ReadR_Offset_HIG=REG_ReadR_Offset_HIG+EEPROM_READ_Byte(0x7E);
	REG_ReadR_Offset_HIG=REG_ReadR_Offset_HIG<<8;
	REG_ReadR_Offset_HIG=REG_ReadR_Offset_HIG+EEPROM_READ_Byte(0x7F);
	REG_ReadR_Offset_HIG=REG_ReadR_Offset_HIG<<8;
	REG_ReadR_Offset_HIG=REG_ReadR_Offset_HIG+EEPROM_READ_Byte(0x80);
	
	REG_ReadR1_Offset=EEPROM_READ_Byte(0x81);
	REG_ReadR1_Offset=REG_ReadR1_Offset<<8;
	REG_ReadR1_Offset=REG_ReadR1_Offset+EEPROM_READ_Byte(0x82);
	REG_ReadR1_Offset=REG_ReadR1_Offset<<8;
	REG_ReadR1_Offset=REG_ReadR1_Offset+EEPROM_READ_Byte(0x83);
	REG_ReadR1_Offset=REG_ReadR1_Offset<<8;
	REG_ReadR1_Offset=REG_ReadR1_Offset+EEPROM_READ_Byte(0x84);
	
	REG_ReadR2_Offset=EEPROM_READ_Byte(0x85);
	REG_ReadR2_Offset=REG_ReadR2_Offset<<8;
	REG_ReadR2_Offset=REG_ReadR2_Offset+EEPROM_READ_Byte(0x86);
	REG_ReadR2_Offset=REG_ReadR2_Offset<<8;
	REG_ReadR2_Offset=REG_ReadR2_Offset+EEPROM_READ_Byte(0x87);
	REG_ReadR2_Offset=REG_ReadR2_Offset<<8;
	REG_ReadR2_Offset=REG_ReadR2_Offset+EEPROM_READ_Byte(0x88);
	
	
	Polar0=EEPROM_READ_Byte(0x89);
	Polar1=EEPROM_READ_Byte(0x8A);
	Polar2=EEPROM_READ_Byte(0x8B);
	Polar3=EEPROM_READ_Byte(0x8C);
	Polar4=EEPROM_READ_Byte(0x8D);
	Polar5=EEPROM_READ_Byte(0x8E);
	Polar6=EEPROM_READ_Byte(0x8F);
	Polar7=EEPROM_READ_Byte(0x90);
}
/******************* (C) COPYRIGHT 2015 AVER *****END OF FILE*************************/