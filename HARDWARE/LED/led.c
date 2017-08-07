#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义GPIO结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能PB端口时钟
	
   	//=============================================================================
	//控制板上的LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//LED-->PB.15 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIOB.15
	GPIO_SetBits(GPIOB, GPIO_Pin_15);						//PB.15 输出高

   	//=============================================================================
	//第一台虚拟机对应桌面开关上的LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;			//IO设置为PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置成推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//初始化GPIOB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz

   	//=============================================================================
	//第二台虚拟机对应桌面开关上的LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;			//IO设置为PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置成推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//初始化GPIOB0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz

	//=============================================================================
	//第三台虚拟机对应桌面开关上的LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;			//IO设置为PB5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置成推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//初始化GPIOB5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz

  	//=============================================================================
	//第四台虚拟机对应桌面开关上的LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;			//IO设置为PB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置成推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//初始化GPIOB6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
}
