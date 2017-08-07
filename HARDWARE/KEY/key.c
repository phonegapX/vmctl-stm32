#include "key.h"
#include "delay.h"


void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	 //结构体定义

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA时钟

	//=============================================================================
	//第一台虚拟机
	//=============================================================================

	//对应RST1按钮
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;			//IO设置为PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//初始化GPIOA6

	//对应PWR1按钮
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;			//IO设置为PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//初始化GPIOA7

	//=============================================================================
	//第二台虚拟机
	//=============================================================================

  	//对应RST2按钮
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;			//IO设置为PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//初始化GPIOA5

	//对应PWR2按钮
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;			//IO设置为PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//初始化GPIOA4

	//=============================================================================
	//第三台虚拟机
	//=============================================================================

  	//对应RST3按钮
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;			//IO设置为PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//初始化GPIOA3

	//对应PWR3按钮
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;			//IO设置为PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//初始化GPIOA2

	//=============================================================================
 	//第四台虚拟机
	//=============================================================================

  	//对应RST4按钮
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;			//IO设置为PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//初始化GPIOA1

	//对应PWR4按钮
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;			//IO设置为PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//初始化GPIOA0

   	//=============================================================================
 	//物理主板开机按钮
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;			//IO设置为PB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置成推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//初始化GPIOB9
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

//相当于按了下物理电脑的电源键
void physical_power_press(void) 
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	delay_ms_with_dog(150);
   	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}
