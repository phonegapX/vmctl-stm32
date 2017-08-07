#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


#define BUTTON_PRESSED 1	//按钮按下
#define BUTTON_RELEASE 0	//按钮松开

s8 power_press_flag[4] = {0};
s8 reset_press_flag[4] = {0};

u32 power_press_ms[4] = {0};
u32 reset_press_ms[4] = {0};

u32 last_power_ms[4] = {0};
u32 last_reset_ms[4] = {0};

//中断
//STM32  19个外部中断
//		 线0-15：对应外部IO口的输入中断。
//		 线16： 连PVD输出
//		 线17： 连RTC闹钟事件
//		 线18： 连USB唤醒事件
//
//每个中断线最多对应7个IO口 比如中断线0  对应GPIOA.0 GPIOB.0 GPIOC.0 GPIOD.0 GPIOE.0 GPIOF.0 GPIOG.0
//并且中断线 一次只能连接到一个IO上
//
//中断线0-4每个中断对应 一个 中断函数
//中断线5-9 共用 中断函数EXTI9_5_IRQHandler
//中断线10-15 共用中断函数EXTI15_10_IRQHandler
//
//         

void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;			//中断配置	  即哪个中断线  EXTI_Line0-15
													//模式 EXTI_Mode_Interrupt中断  EXTI_Mode_Event 事件
													//触发方式  EXTI_Trigger_Falling 下降沿触发
													//			EXTI_Trigger_Rising	 上升沿触发
													//			EXTI_Trigger_Rising_Falling	  任意电平触发

 	NVIC_InitTypeDef NVIC_InitStructure;			//中断参数 中断优先级


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能复用功能时钟

	//=============================================================================
	//第一台虚拟机
	//=============================================================================

    //GPIOA.6 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line6;					//RST1 对应中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//配置下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//GPIOA.7 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line7;					//PWR1 对应中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//配置下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//=============================================================================
	//第二台虚拟机
	//=============================================================================

    //GPIOA.5 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;					//RST2 对应中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//配置下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

 	//中断级别越低 越优先
	//两中断  抢占=响应  执行先发生
	//高优先级的抢占 可断 低优先级
	//优先级相等    高响应不可断低响应
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

   	//GPIOA.4 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line4;					//PWR2 对应中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//配置下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//中断级别越低 越优先
	//两中断  抢占=响应  执行先发生
	//高优先级的抢占 可断 低优先级
	//优先级相等    高响应不可断低响应
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;				//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

	//=============================================================================
	//第三台虚拟机
	//=============================================================================

   	//GPIOA.3 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line3;					//RST3 对应中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//配置下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//中断级别越低 越优先
	//两中断  抢占=响应  执行先发生
	//高优先级的抢占 可断 低优先级
	//优先级相等    高响应不可断低响应
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

  	//GPIOA.2 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line2;					//PWR3 对应中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//配置下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//中断级别越低 越优先
	//两中断  抢占=响应  执行先发生
	//高优先级的抢占 可断 低优先级
	//优先级相等    高响应不可断低响应
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

	//=============================================================================
	//第四台虚拟机
	//=============================================================================

   	//GPIOA.1 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;					//RST4 对应中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//配置下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//中断级别越低 越优先
	//两中断  抢占=响应  执行先发生
	//高优先级的抢占 可断 低优先级
	//优先级相等    高响应不可断低响应
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

   	//GPIOA.0 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;					//PWR4 对应中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//配置下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//中断级别越低 越优先
	//两中断  抢占=响应  执行先发生
	//高优先级的抢占 可断 低优先级
	//优先级相等    高响应不可断低响应
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
}

//外部中断服务程序 
void EXTI9_5_IRQHandler(void)
{
	//第一台虚拟机重启
	if (EXTI_GetITStatus(EXTI_Line6) != RESET)  
	{
		if (KEY_RST1 == 0)
		{
			delay_ms(10);	//消抖
		  	if (KEY_RST1 == 0)
			{
				reset_press_flag[0] = BUTTON_PRESSED;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line6);    // Clear the EXTI line 6 pending bit   
	}

	//第一台虚拟机关机
	if (EXTI_GetITStatus(EXTI_Line7) != RESET)  
	{   
		if (KEY_PWR1 == 0)
		{
			delay_ms(10);	//消抖
		  	if (KEY_PWR1 == 0)
			{
				power_press_flag[0] = BUTTON_PRESSED;
			}
		}      
		EXTI_ClearITPendingBit(EXTI_Line7);    // Clear the EXTI line 7 pending bit   
	} 

	//第二台虚拟机重启
	if (EXTI_GetITStatus(EXTI_Line5) != RESET)  
	{   
		if (KEY_RST2 == 0)
		{
			delay_ms(10);	//消抖
		  	if (KEY_RST2 == 0)
			{
				reset_press_flag[1] = BUTTON_PRESSED;
			}
		}        
		EXTI_ClearITPendingBit(EXTI_Line5);    // Clear the EXTI line 5 pending bit   
	}
}							 

//外部中断服务程序 
void EXTI4_IRQHandler(void)
{
	//第二台虚拟机关机
	if (KEY_PWR2 == 0)
	{
		delay_ms(10);	//消抖
	  	if (KEY_PWR2 == 0)
		{
			power_press_flag[1] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);    // Clear the EXTI line 4 pending bit	
}

//外部中断服务程序 
void EXTI3_IRQHandler(void)
{
	//第三台虚拟机重启
	if (KEY_RST3 == 0)
	{
		delay_ms(10);	//消抖
	  	if (KEY_RST3 == 0)
		{
			reset_press_flag[2] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);    // Clear the EXTI line 3 pending bit	
}

//外部中断服务程序 
void EXTI2_IRQHandler(void)
{
	//第三台虚拟机关机
	if (KEY_PWR3 == 0)
	{
		delay_ms(10);	//消抖
	  	if (KEY_PWR3 == 0)
		{
			power_press_flag[2] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);    // Clear the EXTI line 2 pending bit	
}

//外部中断服务程序 
void EXTI1_IRQHandler(void)
{
	//第四台虚拟机重启
	if (KEY_RST4 == 0)
	{
		delay_ms(10);	//消抖
	  	if (KEY_RST4 == 0)
		{
			reset_press_flag[3] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line1);    // Clear the EXTI line 1 pending bit	
}

//外部中断服务程序 
void EXTI0_IRQHandler(void)
{
	//第四台虚拟机关机
	if (KEY_PWR4 == 0)
	{
		delay_ms(10);	//消抖
	  	if (KEY_PWR4 == 0)
		{
			power_press_flag[3] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);    // Clear the EXTI line 0 pending bit	
}

#define PRESS_INTERVAL_MS   1500
#define LONG_PRESS_MS       3000
#define RELEASE_INTERVAL_MS 500

//...
void vm_button_click_check(void)
{
	//=============================================================================
	//第一台虚拟机按钮操作(电源,重启)
	//=============================================================================
	if (power_press_flag[0] == BUTTON_PRESSED)	//电源键已经按下 
	{
		if (KEY_PWR1 == 0)	//电源键仍是按下状态
		{
			power_press_ms[0]++;	//计数加一
			//...
			if (power_press_ms[0] == 20) 
			{

			 	if (last_power_ms[0] == 0 || (g_systickmsec > last_power_ms[0] && (g_systickmsec - last_power_ms[0] > PRESS_INTERVAL_MS))) {
					last_power_ms[0] = g_systickmsec;
				   	//...
					LED1(1);
					//DESKTOP_LED1(1);
					vm_power_press(0);	//发送 按电源 命令
			 	}
			}
			if (power_press_ms[0] == LONG_PRESS_MS) 
			{
				LED1(1);
				physical_power_press();
			}
		} 
		else	//电源键松开 
		{
			if (power_press_ms[0] < RELEASE_INTERVAL_MS) 
			{
				LED1(1);
			}
			//...
			power_press_flag[0] = BUTTON_RELEASE;	//按钮标志清除
			power_press_ms[0] = 0;	//计数清0
		}
	}
	//...
	if (reset_press_flag[0] == BUTTON_PRESSED)	//重启键已经按下 
	{
		if (KEY_RST1 == 0)	//重启键仍是按下状态
		{
			reset_press_ms[0]++;	//计数加一
			//...
			if (reset_press_ms[0] == 20) 
			{
				if (last_reset_ms[0] == 0 || (g_systickmsec > last_reset_ms[0] && (g_systickmsec - last_reset_ms[0] > PRESS_INTERVAL_MS))) {
					last_reset_ms[0] = g_systickmsec;
					//...
				 	LED1(0);
					vm_reset_press(0);	//发送 按重启 命令
				}
			}
			if (reset_press_ms[0] == LONG_PRESS_MS) 
			{
				LED1(0);
			}
		} 
		else	//重启键松开 
		{	
			if (reset_press_ms[0] < RELEASE_INTERVAL_MS) 
			{
				LED1(0);
			}
			//...
			reset_press_flag[0] = BUTTON_RELEASE;	//按钮标志清除
			reset_press_ms[0] = 0;	//计数清0
		}	
	}
   	//=============================================================================
	//第二台虚拟机按钮操作(电源,重启)
	//=============================================================================
	if (power_press_flag[1] == BUTTON_PRESSED)	//电源键已经按下 
	{
		if (KEY_PWR2 == 0)	//电源键仍是按下状态
		{
			power_press_ms[1]++;	//计数加一
			//...
			if (power_press_ms[1] == 20) 
			{
				if (last_power_ms[1] == 0 || (g_systickmsec > last_power_ms[1] && (g_systickmsec - last_power_ms[1] > PRESS_INTERVAL_MS))) {
					last_power_ms[1] = g_systickmsec;
					//...	
					LED1(1);
					//DESKTOP_LED2(1);
					vm_power_press(1);	//发送 按电源 命令
				}
			}
			if (power_press_ms[1] == LONG_PRESS_MS) 
			{
				LED1(1);
				physical_power_press();
			}
		} 
		else	//电源键松开 
		{
			if (power_press_ms[1] < RELEASE_INTERVAL_MS) 
			{
				LED1(1);
			}
			//...
			power_press_flag[1] = BUTTON_RELEASE;	//按钮标志清除
			power_press_ms[1] = 0;	//计数清0
		}	
	}
	//...
	if (reset_press_flag[1] == BUTTON_PRESSED)	//重启键已经按下 
	{
		if (KEY_RST2 == 0)	//重启键仍是按下状态
		{
			reset_press_ms[1]++;	//计数加一
			//...
			if (reset_press_ms[1] == 20) 
			{
				if (last_reset_ms[1] == 0 || (g_systickmsec > last_reset_ms[1] && (g_systickmsec - last_reset_ms[1] > PRESS_INTERVAL_MS))) {
					last_reset_ms[1] = g_systickmsec;
					//...
					LED1(0);
					vm_reset_press(1);	//发送 按重启 命令
				}
			}
			if (reset_press_ms[1] == LONG_PRESS_MS) 
			{
				LED1(0);
			}
		} 
		else	//重启键松开 
		{		
			if (reset_press_ms[1] < RELEASE_INTERVAL_MS) 
			{
				LED1(0);
			}
			//...
			reset_press_flag[1] = BUTTON_RELEASE;	//按钮标志清除
			reset_press_ms[1] = 0;	//计数清0
		}	
	}
	//=============================================================================
	//第三台虚拟机按钮操作(电源,重启)
	//=============================================================================
	if (power_press_flag[2] == BUTTON_PRESSED)	//电源键已经按下 
	{
		if (KEY_PWR3 == 0)	//电源键仍是按下状态
		{
			power_press_ms[2]++;	//计数加一

			//...
			if (power_press_ms[2] == 20) 
			{
				if (last_power_ms[2] == 0 || (g_systickmsec > last_power_ms[2] && (g_systickmsec - last_power_ms[2] > PRESS_INTERVAL_MS))) {
					last_power_ms[2] = g_systickmsec;
					//...
					LED1(1);
					//DESKTOP_LED3(1);
					vm_power_press(2);	//发送 按电源 命令
				}			
			}
			if (power_press_ms[2] == LONG_PRESS_MS) 
			{
				LED1(1);
				physical_power_press();
			}
		} 
		else	//电源键松开 
		{
			if (power_press_ms[2] < RELEASE_INTERVAL_MS) 
			{
				LED1(1);
			}
			//...
			power_press_flag[2] = BUTTON_RELEASE;	//按钮标志清除
			power_press_ms[2] = 0;	//计数清0
		}	
	}
	//...
	if (reset_press_flag[2] == BUTTON_PRESSED)	//重启键已经按下 
	{
		if (KEY_RST3 == 0)	//重启键仍是按下状态
		{
			reset_press_ms[2]++;	//计数加一

			//...
			if (reset_press_ms[2] == 20) 
			{
				if (last_reset_ms[2] == 0 || (g_systickmsec > last_reset_ms[2] && (g_systickmsec - last_reset_ms[2] > PRESS_INTERVAL_MS))) {
					last_reset_ms[2] = g_systickmsec;
					//...
					LED1(0);
					vm_reset_press(2);	//发送 按重启 命令	
				}
			}
			if (reset_press_ms[2] == LONG_PRESS_MS) 
			{
				LED1(0);
			}
		} 
		else	//重启键松开 
		{

			if (reset_press_ms[2] < RELEASE_INTERVAL_MS) 
			{
				LED1(0);
			}
			//...
			reset_press_flag[2] = BUTTON_RELEASE;	//按钮标志清除
			reset_press_ms[2] = 0;	//计数清0
		}	
	}
	//=============================================================================
	//第四台虚拟机按钮操作(电源,重启)
	//=============================================================================
	if (power_press_flag[3] == BUTTON_PRESSED)	//电源键已经按下 
	{
		if (KEY_PWR4 == 0)	//电源键仍是按下状态
		{
			power_press_ms[3]++;	//计数加一

			//...
			if (power_press_ms[3] == 20) 
			{
				if (last_power_ms[3] == 0 || (g_systickmsec > last_power_ms[3] && (g_systickmsec - last_power_ms[3] > PRESS_INTERVAL_MS))) {
					last_power_ms[3] = g_systickmsec;
					//...
					LED1(1);
					//DESKTOP_LED4(1);
					vm_power_press(3);	//发送 按电源 命令		
				}
			}
			if (power_press_ms[3] == LONG_PRESS_MS) 
			{
				LED1(1);
				physical_power_press();
			}
		} 
		else	//电源键松开 
		{
			if (power_press_ms[3] < RELEASE_INTERVAL_MS) 
			{
				LED1(1);
			}
			//...
			power_press_flag[3] = BUTTON_RELEASE;	//按钮标志清除
			power_press_ms[3] = 0;	//计数清0
		}	
	}
	//...
	if (reset_press_flag[3] == BUTTON_PRESSED)	//重启键已经按下 
	{
		if (KEY_RST4 == 0)	//重启键仍是按下状态
		{
			reset_press_ms[3]++;	//计数加一

			//...
			if (reset_press_ms[3] == 20) 
			{
				if (last_reset_ms[3] == 0 || (g_systickmsec > last_reset_ms[3] && (g_systickmsec - last_reset_ms[3] > PRESS_INTERVAL_MS))) {
					last_reset_ms[3] = g_systickmsec;
					//...
					LED1(0);
					vm_reset_press(3);	//发送 按重启 命令	
				}
			}
			if (reset_press_ms[3] == LONG_PRESS_MS) 
			{
				LED1(0);
			}
		} 
		else	//重启键松开 
		{

			if (reset_press_ms[3] < RELEASE_INTERVAL_MS) 
			{
				LED1(0);
			}
			//...
			reset_press_flag[3] = BUTTON_RELEASE;	//按钮标志清除
			reset_press_ms[3] = 0;	//计数清0
		}	
	}
}
