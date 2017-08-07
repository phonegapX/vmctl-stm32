#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define KEY_RST1  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)
#define KEY_PWR1  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)

#define KEY_RST2  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)
#define KEY_PWR2  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)

#define KEY_RST3  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)
#define KEY_PWR3  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)

#define KEY_RST4  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)
#define KEY_PWR4  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

void KEY_Init(void);//IO≥ı ºªØ
void physical_power_press(void);

#endif
