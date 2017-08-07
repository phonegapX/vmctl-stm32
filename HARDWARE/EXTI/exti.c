#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


#define BUTTON_PRESSED 1	//��ť����
#define BUTTON_RELEASE 0	//��ť�ɿ�

s8 power_press_flag[4] = {0};
s8 reset_press_flag[4] = {0};

u32 power_press_ms[4] = {0};
u32 reset_press_ms[4] = {0};

u32 last_power_ms[4] = {0};
u32 last_reset_ms[4] = {0};

//�ж�
//STM32  19���ⲿ�ж�
//		 ��0-15����Ӧ�ⲿIO�ڵ������жϡ�
//		 ��16�� ��PVD���
//		 ��17�� ��RTC�����¼�
//		 ��18�� ��USB�����¼�
//
//ÿ���ж�������Ӧ7��IO�� �����ж���0  ��ӦGPIOA.0 GPIOB.0 GPIOC.0 GPIOD.0 GPIOE.0 GPIOF.0 GPIOG.0
//�����ж��� һ��ֻ�����ӵ�һ��IO��
//
//�ж���0-4ÿ���ж϶�Ӧ һ�� �жϺ���
//�ж���5-9 ���� �жϺ���EXTI9_5_IRQHandler
//�ж���10-15 �����жϺ���EXTI15_10_IRQHandler
//
//         

void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;			//�ж�����	  ���ĸ��ж���  EXTI_Line0-15
													//ģʽ EXTI_Mode_Interrupt�ж�  EXTI_Mode_Event �¼�
													//������ʽ  EXTI_Trigger_Falling �½��ش���
													//			EXTI_Trigger_Rising	 �����ش���
													//			EXTI_Trigger_Rising_Falling	  �����ƽ����

 	NVIC_InitTypeDef NVIC_InitStructure;			//�жϲ��� �ж����ȼ�


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ�ܸ��ù���ʱ��

	//=============================================================================
	//��һ̨�����
	//=============================================================================

    //GPIOA.6 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line6;					//RST1 ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�����½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//GPIOA.7 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line7;					//PWR1 ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�����½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//=============================================================================
	//�ڶ�̨�����
	//=============================================================================

    //GPIOA.5 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;					//RST2 ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�����½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

 	//�жϼ���Խ�� Խ����
	//���ж�  ��ռ=��Ӧ  ִ���ȷ���
	//�����ȼ�����ռ �ɶ� �����ȼ�
	//���ȼ����    ����Ӧ���ɶϵ���Ӧ
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);

   	//GPIOA.4 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line4;					//PWR2 ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�����½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//�жϼ���Խ�� Խ����
	//���ж�  ��ռ=��Ӧ  ִ���ȷ���
	//�����ȼ�����ռ �ɶ� �����ȼ�
	//���ȼ����    ����Ӧ���ɶϵ���Ӧ
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);

	//=============================================================================
	//����̨�����
	//=============================================================================

   	//GPIOA.3 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line3;					//RST3 ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�����½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//�жϼ���Խ�� Խ����
	//���ж�  ��ռ=��Ӧ  ִ���ȷ���
	//�����ȼ�����ռ �ɶ� �����ȼ�
	//���ȼ����    ����Ӧ���ɶϵ���Ӧ
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);

  	//GPIOA.2 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line2;					//PWR3 ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�����½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//�жϼ���Խ�� Խ����
	//���ж�  ��ռ=��Ӧ  ִ���ȷ���
	//�����ȼ�����ռ �ɶ� �����ȼ�
	//���ȼ����    ����Ӧ���ɶϵ���Ӧ
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);

	//=============================================================================
	//����̨�����
	//=============================================================================

   	//GPIOA.1 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;					//RST4 ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�����½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//�жϼ���Խ�� Խ����
	//���ж�  ��ռ=��Ӧ  ִ���ȷ���
	//�����ȼ�����ռ �ɶ� �����ȼ�
	//���ȼ����    ����Ӧ���ɶϵ���Ӧ
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);

   	//GPIOA.0 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	//...
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;					//PWR4 ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�����½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//�жϼ���Խ�� Խ����
	//���ж�  ��ռ=��Ӧ  ִ���ȷ���
	//�����ȼ�����ռ �ɶ� �����ȼ�
	//���ȼ����    ����Ӧ���ɶϵ���Ӧ
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);
}

//�ⲿ�жϷ������ 
void EXTI9_5_IRQHandler(void)
{
	//��һ̨���������
	if (EXTI_GetITStatus(EXTI_Line6) != RESET)  
	{
		if (KEY_RST1 == 0)
		{
			delay_ms(10);	//����
		  	if (KEY_RST1 == 0)
			{
				reset_press_flag[0] = BUTTON_PRESSED;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line6);    // Clear the EXTI line 6 pending bit   
	}

	//��һ̨������ػ�
	if (EXTI_GetITStatus(EXTI_Line7) != RESET)  
	{   
		if (KEY_PWR1 == 0)
		{
			delay_ms(10);	//����
		  	if (KEY_PWR1 == 0)
			{
				power_press_flag[0] = BUTTON_PRESSED;
			}
		}      
		EXTI_ClearITPendingBit(EXTI_Line7);    // Clear the EXTI line 7 pending bit   
	} 

	//�ڶ�̨���������
	if (EXTI_GetITStatus(EXTI_Line5) != RESET)  
	{   
		if (KEY_RST2 == 0)
		{
			delay_ms(10);	//����
		  	if (KEY_RST2 == 0)
			{
				reset_press_flag[1] = BUTTON_PRESSED;
			}
		}        
		EXTI_ClearITPendingBit(EXTI_Line5);    // Clear the EXTI line 5 pending bit   
	}
}							 

//�ⲿ�жϷ������ 
void EXTI4_IRQHandler(void)
{
	//�ڶ�̨������ػ�
	if (KEY_PWR2 == 0)
	{
		delay_ms(10);	//����
	  	if (KEY_PWR2 == 0)
		{
			power_press_flag[1] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);    // Clear the EXTI line 4 pending bit	
}

//�ⲿ�жϷ������ 
void EXTI3_IRQHandler(void)
{
	//����̨���������
	if (KEY_RST3 == 0)
	{
		delay_ms(10);	//����
	  	if (KEY_RST3 == 0)
		{
			reset_press_flag[2] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);    // Clear the EXTI line 3 pending bit	
}

//�ⲿ�жϷ������ 
void EXTI2_IRQHandler(void)
{
	//����̨������ػ�
	if (KEY_PWR3 == 0)
	{
		delay_ms(10);	//����
	  	if (KEY_PWR3 == 0)
		{
			power_press_flag[2] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);    // Clear the EXTI line 2 pending bit	
}

//�ⲿ�жϷ������ 
void EXTI1_IRQHandler(void)
{
	//����̨���������
	if (KEY_RST4 == 0)
	{
		delay_ms(10);	//����
	  	if (KEY_RST4 == 0)
		{
			reset_press_flag[3] = BUTTON_PRESSED;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line1);    // Clear the EXTI line 1 pending bit	
}

//�ⲿ�жϷ������ 
void EXTI0_IRQHandler(void)
{
	//����̨������ػ�
	if (KEY_PWR4 == 0)
	{
		delay_ms(10);	//����
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
	//��һ̨�������ť����(��Դ,����)
	//=============================================================================
	if (power_press_flag[0] == BUTTON_PRESSED)	//��Դ���Ѿ����� 
	{
		if (KEY_PWR1 == 0)	//��Դ�����ǰ���״̬
		{
			power_press_ms[0]++;	//������һ
			//...
			if (power_press_ms[0] == 20) 
			{

			 	if (last_power_ms[0] == 0 || (g_systickmsec > last_power_ms[0] && (g_systickmsec - last_power_ms[0] > PRESS_INTERVAL_MS))) {
					last_power_ms[0] = g_systickmsec;
				   	//...
					LED1(1);
					//DESKTOP_LED1(1);
					vm_power_press(0);	//���� ����Դ ����
			 	}
			}
			if (power_press_ms[0] == LONG_PRESS_MS) 
			{
				LED1(1);
				physical_power_press();
			}
		} 
		else	//��Դ���ɿ� 
		{
			if (power_press_ms[0] < RELEASE_INTERVAL_MS) 
			{
				LED1(1);
			}
			//...
			power_press_flag[0] = BUTTON_RELEASE;	//��ť��־���
			power_press_ms[0] = 0;	//������0
		}
	}
	//...
	if (reset_press_flag[0] == BUTTON_PRESSED)	//�������Ѿ����� 
	{
		if (KEY_RST1 == 0)	//���������ǰ���״̬
		{
			reset_press_ms[0]++;	//������һ
			//...
			if (reset_press_ms[0] == 20) 
			{
				if (last_reset_ms[0] == 0 || (g_systickmsec > last_reset_ms[0] && (g_systickmsec - last_reset_ms[0] > PRESS_INTERVAL_MS))) {
					last_reset_ms[0] = g_systickmsec;
					//...
				 	LED1(0);
					vm_reset_press(0);	//���� ������ ����
				}
			}
			if (reset_press_ms[0] == LONG_PRESS_MS) 
			{
				LED1(0);
			}
		} 
		else	//�������ɿ� 
		{	
			if (reset_press_ms[0] < RELEASE_INTERVAL_MS) 
			{
				LED1(0);
			}
			//...
			reset_press_flag[0] = BUTTON_RELEASE;	//��ť��־���
			reset_press_ms[0] = 0;	//������0
		}	
	}
   	//=============================================================================
	//�ڶ�̨�������ť����(��Դ,����)
	//=============================================================================
	if (power_press_flag[1] == BUTTON_PRESSED)	//��Դ���Ѿ����� 
	{
		if (KEY_PWR2 == 0)	//��Դ�����ǰ���״̬
		{
			power_press_ms[1]++;	//������һ
			//...
			if (power_press_ms[1] == 20) 
			{
				if (last_power_ms[1] == 0 || (g_systickmsec > last_power_ms[1] && (g_systickmsec - last_power_ms[1] > PRESS_INTERVAL_MS))) {
					last_power_ms[1] = g_systickmsec;
					//...	
					LED1(1);
					//DESKTOP_LED2(1);
					vm_power_press(1);	//���� ����Դ ����
				}
			}
			if (power_press_ms[1] == LONG_PRESS_MS) 
			{
				LED1(1);
				physical_power_press();
			}
		} 
		else	//��Դ���ɿ� 
		{
			if (power_press_ms[1] < RELEASE_INTERVAL_MS) 
			{
				LED1(1);
			}
			//...
			power_press_flag[1] = BUTTON_RELEASE;	//��ť��־���
			power_press_ms[1] = 0;	//������0
		}	
	}
	//...
	if (reset_press_flag[1] == BUTTON_PRESSED)	//�������Ѿ����� 
	{
		if (KEY_RST2 == 0)	//���������ǰ���״̬
		{
			reset_press_ms[1]++;	//������һ
			//...
			if (reset_press_ms[1] == 20) 
			{
				if (last_reset_ms[1] == 0 || (g_systickmsec > last_reset_ms[1] && (g_systickmsec - last_reset_ms[1] > PRESS_INTERVAL_MS))) {
					last_reset_ms[1] = g_systickmsec;
					//...
					LED1(0);
					vm_reset_press(1);	//���� ������ ����
				}
			}
			if (reset_press_ms[1] == LONG_PRESS_MS) 
			{
				LED1(0);
			}
		} 
		else	//�������ɿ� 
		{		
			if (reset_press_ms[1] < RELEASE_INTERVAL_MS) 
			{
				LED1(0);
			}
			//...
			reset_press_flag[1] = BUTTON_RELEASE;	//��ť��־���
			reset_press_ms[1] = 0;	//������0
		}	
	}
	//=============================================================================
	//����̨�������ť����(��Դ,����)
	//=============================================================================
	if (power_press_flag[2] == BUTTON_PRESSED)	//��Դ���Ѿ����� 
	{
		if (KEY_PWR3 == 0)	//��Դ�����ǰ���״̬
		{
			power_press_ms[2]++;	//������һ

			//...
			if (power_press_ms[2] == 20) 
			{
				if (last_power_ms[2] == 0 || (g_systickmsec > last_power_ms[2] && (g_systickmsec - last_power_ms[2] > PRESS_INTERVAL_MS))) {
					last_power_ms[2] = g_systickmsec;
					//...
					LED1(1);
					//DESKTOP_LED3(1);
					vm_power_press(2);	//���� ����Դ ����
				}			
			}
			if (power_press_ms[2] == LONG_PRESS_MS) 
			{
				LED1(1);
				physical_power_press();
			}
		} 
		else	//��Դ���ɿ� 
		{
			if (power_press_ms[2] < RELEASE_INTERVAL_MS) 
			{
				LED1(1);
			}
			//...
			power_press_flag[2] = BUTTON_RELEASE;	//��ť��־���
			power_press_ms[2] = 0;	//������0
		}	
	}
	//...
	if (reset_press_flag[2] == BUTTON_PRESSED)	//�������Ѿ����� 
	{
		if (KEY_RST3 == 0)	//���������ǰ���״̬
		{
			reset_press_ms[2]++;	//������һ

			//...
			if (reset_press_ms[2] == 20) 
			{
				if (last_reset_ms[2] == 0 || (g_systickmsec > last_reset_ms[2] && (g_systickmsec - last_reset_ms[2] > PRESS_INTERVAL_MS))) {
					last_reset_ms[2] = g_systickmsec;
					//...
					LED1(0);
					vm_reset_press(2);	//���� ������ ����	
				}
			}
			if (reset_press_ms[2] == LONG_PRESS_MS) 
			{
				LED1(0);
			}
		} 
		else	//�������ɿ� 
		{

			if (reset_press_ms[2] < RELEASE_INTERVAL_MS) 
			{
				LED1(0);
			}
			//...
			reset_press_flag[2] = BUTTON_RELEASE;	//��ť��־���
			reset_press_ms[2] = 0;	//������0
		}	
	}
	//=============================================================================
	//����̨�������ť����(��Դ,����)
	//=============================================================================
	if (power_press_flag[3] == BUTTON_PRESSED)	//��Դ���Ѿ����� 
	{
		if (KEY_PWR4 == 0)	//��Դ�����ǰ���״̬
		{
			power_press_ms[3]++;	//������һ

			//...
			if (power_press_ms[3] == 20) 
			{
				if (last_power_ms[3] == 0 || (g_systickmsec > last_power_ms[3] && (g_systickmsec - last_power_ms[3] > PRESS_INTERVAL_MS))) {
					last_power_ms[3] = g_systickmsec;
					//...
					LED1(1);
					//DESKTOP_LED4(1);
					vm_power_press(3);	//���� ����Դ ����		
				}
			}
			if (power_press_ms[3] == LONG_PRESS_MS) 
			{
				LED1(1);
				physical_power_press();
			}
		} 
		else	//��Դ���ɿ� 
		{
			if (power_press_ms[3] < RELEASE_INTERVAL_MS) 
			{
				LED1(1);
			}
			//...
			power_press_flag[3] = BUTTON_RELEASE;	//��ť��־���
			power_press_ms[3] = 0;	//������0
		}	
	}
	//...
	if (reset_press_flag[3] == BUTTON_PRESSED)	//�������Ѿ����� 
	{
		if (KEY_RST4 == 0)	//���������ǰ���״̬
		{
			reset_press_ms[3]++;	//������һ

			//...
			if (reset_press_ms[3] == 20) 
			{
				if (last_reset_ms[3] == 0 || (g_systickmsec > last_reset_ms[3] && (g_systickmsec - last_reset_ms[3] > PRESS_INTERVAL_MS))) {
					last_reset_ms[3] = g_systickmsec;
					//...
					LED1(0);
					vm_reset_press(3);	//���� ������ ����	
				}
			}
			if (reset_press_ms[3] == LONG_PRESS_MS) 
			{
				LED1(0);
			}
		} 
		else	//�������ɿ� 
		{

			if (reset_press_ms[3] < RELEASE_INTERVAL_MS) 
			{
				LED1(0);
			}
			//...
			reset_press_flag[3] = BUTTON_RELEASE;	//��ť��־���
			reset_press_ms[3] = 0;	//������0
		}	
	}
}
