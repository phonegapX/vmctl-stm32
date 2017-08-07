/**********************************************************************************
 * ����1 ����
 * ����1 PA9 PA10���� �� �ж���Ӧ����
 * ��汾  ��ST_v3.5
**********************************************************************************/

#include "usart.h"
#include "delay.h"
#include "crc.h"
#include "key.h"


#define FIRMWARE_VERSION "2017-07-11-1.0.0.a"

void do_recv(u8 *cmdbuf);

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];	//���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA = 0;	//����״̬���	  

//��ʼ��IO ����1 
//bound:������
void USART_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;				   //IO�����ýṹ��
	USART_InitTypeDef USART_InitStructure;				   //�������ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;				   //�ж����ýṹ��
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 		//PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������	 PA.9ѡ����״̬���ܽ��봮��ģʽ
    GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��PA10

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;					//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
#if EN_USART1_RX		  //���ʹ���˽���  
   //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
#endif
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}

//���� ������һ��16λ����USART_RX_STA ����ʾ �ɼ������ݳ���  ����״̬�� �൱��һ���Ĵ���
//USART_RX_STA     15		    14	         13-0
//				 �������	���յ�0x0d	  ���յ����ݳ���  ÿ���ռ�1 ��ʾ����һ���ֽ�
//USART_RX_STA=0 ��Ϊ����������׼��

//���ڽ����жϵ�ǰ���� ���ݵ�����Իس�Ϊ׼  ��  0x0d 0x0a  

void USART1_IRQHandler(void)	//����1�ж���Ӧ���� �����ֲ�����㶨��
{
	u8 Res;													//�����ڽ��յ�����  RXNE������1 
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART1);	//(USART1->DR);	//��ȡ���յ�������
		
		if ((USART_RX_STA&0x8000) == 0)	//����δ���
		{
			if (USART_RX_STA&0x4000)	//���յ���0x0d
			{
				if (Res != 0x0a) 
				{
					USART_RX_STA = 0;	//���մ���,���¿�ʼ
					memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
				}
				else
				{
					USART_RX_STA |= 0x8000;	//��������� //���յ��س��ĺ��ֽ�  ��λ״̬�Ĵ���
					//...
					//printf((const char *)USART_RX_BUF);
					//printf("\r\n");
					do_recv(USART_RX_BUF);
					//==================
					USART_RX_STA = 0;
					memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));	
				}
			}
			else //��û�յ�0X0D
			{	
				if (Res == 0x0d)
				{ 
					USART_RX_STA |= 0x4000;	//���յ��س���ǰһ�ֽ�  ��λ״̬�Ĵ���
				}
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF] = Res;				//�����յ����� ����������
					USART_RX_STA++;											//����+1 Ϊ��һ����׼��
					if (USART_RX_STA > (USART_REC_LEN-1)) 
					{ 
						USART_RX_STA = 0;	//�������ݴ���,���¿�ʼ����
						memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));	
					}  
				}		 
			}
		}   		 
	}
} 
#endif	

//////////////////////////////////////////////////////////////////////////////////////
// 
// "type:handshake"						(PC==>stm32)
// "type:handshake:token:1657897321"	(stm32==>PC)
//
// "type:version:token:1657897321"							(PC==>stm32)
// "type:version:token:1657897321:result:2017-07-11-7685a"	(stm32==>PC)
//
// "type:power:vm:0:token:1657897321"	(stm32==>PC)
// "type:reset:vm:0:token:1657897321"	(stm32==>PC)
//
// "type:suspend:token:1657897321"		(PC==>stm32)
//
// "type:online:token:1657897321"		(stm32==>PC)
// "type:online:token:1657897321"		(PC==>stm32)
//
//////////////////////////////////////////////////////////////////////////////////////

u32 g_token = 1657897321;

int g_suspend = 0;		//PC�����Ƿ��Ѿ�����
int g_has_reply = 0;	//�Ƿ��յ�PC�˵�Ӧ��

//����������� <���µ�Դ��ť> ����
//�������ڼ�̨�����
void vm_power_press(u8 index) 
{
	if (g_suspend) {	//���������ˣ���Ҫ�Ȼ���
		//...
		g_suspend = 0;	//���־λ
		//...
		{
			int i;
			for (i = 0; i < 5; i++) {	//��ೢ�����
				physical_power_press();	//����Դ�źŻ���PC����
				//...
				delay_ms_with_dog(300);	//�ȴ�300����
				//...
				g_has_reply = 0;		//�����־λ
	   			printf("type:online:token:%u\r\n", g_token);	//ѯ��PC���Ƿ��Ѿ�����
				delay_ms_with_dog(200);							//�ȴ�200����
				if (g_has_reply) {	//��־λ����λ,֤���յ�PC�˵Ļظ�,֤��PC���Ѿ�����
					//...
					break;	//�����Ϳ���ֱ���˳�ѭ����
				}
			}
		}
	}
	//...
	printf("type:power:vm:%u:token:%u\r\n", index, g_token);
}

//����������� <����������ť> ����
//�������ڼ�̨�����
void vm_reset_press(u8 index) 
{
	printf("type:reset:vm:%u:token:%u\r\n", index, g_token);
}

#define TYPE_HANDSHAKE 1
#define TYPE_VERSION   2
#define TYPE_SUSPEND   3
#define TYPE_ONLINE    4

//����PC��������ָ��,��Ҫ������������,һ��������ָ��,����һ���ǻ�ȡ���ư�İ汾��
void do_recv(u8 *cmdbuf) 
{
	u8 i = 0;
	u8 err = 2;
	u8 type = 0;
	char *ptr;   
	ptr = strtok((char*)cmdbuf, ":");  
	while (ptr != NULL) {
		//...
		if (i == 0) {
			if (strcmp(ptr, "type") != 0) {
				err = 1;
				break;		
			}
		} else if (i == 1) {
			if (strcmp(ptr, "handshake") == 0) {
				type = TYPE_HANDSHAKE;
				err = 0;
			} else if (strcmp(ptr, "version") == 0) {
				type = TYPE_VERSION;
			} else if (strcmp(ptr, "suspend") == 0) {
				type = TYPE_SUSPEND;
	   		} else if (strcmp(ptr, "online") == 0) {
				type = TYPE_ONLINE;
			} else {
				err = 1;
				break;
			} 
		} else if (i == 2) {
			if ((type == TYPE_VERSION || type == TYPE_SUSPEND || type == TYPE_ONLINE) && strcmp(ptr, "token") == 0) {} else {
				err = 1;
				break;			
			}
		} else if (i == 3) {
			if (type == TYPE_VERSION || type == TYPE_SUSPEND || type == TYPE_ONLINE) {
				char token[16] = {0};
				sprintf(token, "%u", g_token);
				if (strcmp(ptr, token) == 0) {
					err = 0;
				} else {
					err = 1;
					break;
				}			
			} else {
				err = 1;
				break;			
			}
		} else if (i == 4) {
			err = 1;
			break;		
		}
	  	//...
	    ptr = strtok(NULL, ":");
		i++;  
	}
	//...
	if (err) {
		return;
	}  
	//...
	if (type == TYPE_HANDSHAKE) {
		srand(g_systickmsec);
		g_token = rand();
		g_token = CRC_CalcBlockCRC((uint32_t *)&g_token, 1);
		printf("type:handshake:token:%u\r\n", g_token);
	} else if (type == TYPE_VERSION) {
		printf("type:version:token:%u:result:"FIRMWARE_VERSION"\r\n", g_token);
	} else if (type == TYPE_SUSPEND) {
		g_suspend = 1;		//�յ�PC�˵�֪ͨ,PC��������
	} else if (type == TYPE_ONLINE) {
		g_has_reply = 1;	//���ư����PC��ѯ���Ƿ��Ѿ�����,�õ����Ӧ�����PC���Ѿ�������
	}
}
