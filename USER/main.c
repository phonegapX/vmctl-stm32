#include  "delay.h"
#include  "led.h"
#include  "key.h"
#include  "usart.h"
#include "exti.h"
#include "crc.h"
#include "iwdg.h"


int main(void)
{
   	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组为组2  2:2位抢占优先级，2位响应优先级
	                                                //一旦定义 即不要轻易改变
	USART_init(115200);	    //串口初始化为115200  
	LED_Init();		  	    //初始化与LED连接的硬件接口
	KEY_Init();				//按键端口初始化
	EXTIX_Init();		 	//外部中断初始化
 	CRC_Init();				//CRC器件初始化
	IWDG_init();			//初始化独立看门狗

 	while (1)
	{
		delay_one_ms();
		vm_button_click_check();	
	}
}
