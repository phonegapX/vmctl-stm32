#ifndef	__LED_H
#define __LED_H

#include "stm32f10x.h"

void LED_Init(void);//≥ı ºªØ

#define LED1(a)	if (a)	                                \
					GPIO_ResetBits(GPIOB, GPIO_Pin_15); \
					else		                        \
					GPIO_SetBits(GPIOB, GPIO_Pin_15)

#define DESKTOP_LED1(a)	if (a)							\
					GPIO_SetBits(GPIOB, GPIO_Pin_1);	\
					else		                        \
					GPIO_ResetBits(GPIOB, GPIO_Pin_1)	

#define DESKTOP_LED2(a)	if (a)							\
					GPIO_SetBits(GPIOB, GPIO_Pin_0);	\
					else		                        \
					GPIO_ResetBits(GPIOB, GPIO_Pin_0)
					
#define DESKTOP_LED3(a)	if (a)							\
					GPIO_SetBits(GPIOB, GPIO_Pin_5);	\
					else		                        \
					GPIO_ResetBits(GPIOB, GPIO_Pin_5)					
	
#define DESKTOP_LED4(a)	if (a)							\
					GPIO_SetBits(GPIOB, GPIO_Pin_6);	\
					else		                        \
					GPIO_ResetBits(GPIOB, GPIO_Pin_6)						

#endif
