#ifndef __DELAY_H
#define __DELAY_H 

#include "stm32f10x.h"

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_one_ms(void);
void delay_ms_with_dog(u16 nms);

extern u32 g_systickmsec;

#endif
