#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����GPIO�ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��PB�˿�ʱ��
	
   	//=============================================================================
	//���ư��ϵ�LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//LED-->PB.15 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIOB.15
	GPIO_SetBits(GPIOB, GPIO_Pin_15);						//PB.15 �����

   	//=============================================================================
	//��һ̨�������Ӧ���濪���ϵ�LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;			//IO����ΪPB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//���ó��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//��ʼ��GPIOB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz

   	//=============================================================================
	//�ڶ�̨�������Ӧ���濪���ϵ�LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;			//IO����ΪPB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//���ó��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//��ʼ��GPIOB0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz

	//=============================================================================
	//����̨�������Ӧ���濪���ϵ�LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;			//IO����ΪPB5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//���ó��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//��ʼ��GPIOB5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz

  	//=============================================================================
	//����̨�������Ӧ���濪���ϵ�LED
	//=============================================================================
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;			//IO����ΪPB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//���ó��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	           	//��ʼ��GPIOB6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
}
