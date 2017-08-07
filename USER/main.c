#include  "delay.h"
#include  "led.h"
#include  "key.h"
#include  "usart.h"
#include "exti.h"
#include "crc.h"
#include "iwdg.h"


int main(void)
{
   	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���Ϊ��2  2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	                                                //һ������ ����Ҫ���׸ı�
	USART_init(115200);	    //���ڳ�ʼ��Ϊ115200  
	LED_Init();		  	    //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//�����˿ڳ�ʼ��
	EXTIX_Init();		 	//�ⲿ�жϳ�ʼ��
 	CRC_Init();				//CRC������ʼ��
	IWDG_init();			//��ʼ���������Ź�

 	while (1)
	{
		delay_one_ms();
		vm_button_click_check();	
	}
}
