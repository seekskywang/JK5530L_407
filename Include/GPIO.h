#ifndef __gpio_H
#define	__gpio_H

void GPIO_Conf(void);
void DS18B20_OUT(void);
void DS18B20_IN(void);
#define DS18B20_DQ_OUT_1	(GPIO_SetBits(GPIOB,GPIO_Pin_10))
#define DS18B20_DQ_OUT_0	(GPIO_ResetBits(GPIOB,GPIO_Pin_10))
#define DS18B20_DQ_IN		(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))
#endif /* __gpio_H */