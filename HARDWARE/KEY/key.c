#include "key.h"
#include "delay.h"


void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	 //�ṹ�嶨��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOAʱ��

	//=============================================================================
	//��һ̨�����
	//=============================================================================

	//��ӦRST1��ť
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;			//IO����ΪPA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//��ʼ��GPIOA6

	//��ӦPWR1��ť
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;			//IO����ΪPA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//��ʼ��GPIOA7

	//=============================================================================
	//�ڶ�̨�����
	//=============================================================================

  	//��ӦRST2��ť
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;			//IO����ΪPA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//��ʼ��GPIOA5

	//��ӦPWR2��ť
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;			//IO����ΪPA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//��ʼ��GPIOA4

	//=============================================================================
	//����̨�����
	//=============================================================================

  	//��ӦRST3��ť
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;			//IO����ΪPA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//��ʼ��GPIOA3

	//��ӦPWR3��ť
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;			//IO����ΪPA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//��ʼ��GPIOA2

	//=============================================================================
 	//����̨�����
	//=============================================================================

  	//��ӦRST4��ť
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;			//IO����ΪPA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//��ʼ��GPIOA1

	//��ӦPWR4��ť
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;			//IO����ΪPA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	           	//��ʼ��GPIOA0

   	//=============================================================================
 	//�������忪����ť
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;			//IO����ΪPB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//���ó��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//��ʼ��GPIOB9
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

//�൱�ڰ�����������Եĵ�Դ��
void physical_power_press(void) 
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	delay_ms_with_dog(150);
   	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}
