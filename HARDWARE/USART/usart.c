/**********************************************************************************
 * 串口1 配置
 * 串口1 PA9 PA10配置 及 中断相应函数
 * 库版本  ：ST_v3.5
**********************************************************************************/

#include "usart.h"
#include "delay.h"
#include "crc.h"
#include "key.h"


#define FIRMWARE_VERSION "2017-07-11-1.0.0.a"

void do_recv(u8 *cmdbuf);

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif 

/*使用microLib的方法*/
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

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];	//接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0;	//接收状态标记	  

//初始化IO 串口1 
//bound:波特率
void USART_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;				   //IO口配置结构体
	USART_InitTypeDef USART_InitStructure;				   //串口配置结构体
	NVIC_InitTypeDef NVIC_InitStructure;				   //中断配置结构体
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 		//PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出	 PA.9选择复用状态才能进入串口模式
    GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化PA10

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;					//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
#if EN_USART1_RX		  //如果使能了接收  
   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART1, ENABLE);                    //使能串口 
}

//这里 定义了一个16位数据USART_RX_STA 来表示 采集的数据长度  数据状态等 相当于一个寄存器
//USART_RX_STA     15		    14	         13-0
//				 接收完成	接收到0x0d	  接收的数据长度  每接收加1 表示多了一个字节
//USART_RX_STA=0 则为接收数据做准备

//串口进入中断的前提是 数据的最后以回车为准  即  0x0d 0x0a  

void USART1_IRQHandler(void)	//串口1中断响应程序 其名字不能随便定义
{
	u8 Res;													//当串口接收到数据  RXNE将被置1 
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1);	//(USART1->DR);	//读取接收到的数据
		
		if ((USART_RX_STA&0x8000) == 0)	//接收未完成
		{
			if (USART_RX_STA&0x4000)	//接收到了0x0d
			{
				if (Res != 0x0a) 
				{
					USART_RX_STA = 0;	//接收错误,重新开始
					memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
				}
				else
				{
					USART_RX_STA |= 0x8000;	//接收完成了 //接收到回车的后字节  置位状态寄存器
					//...
					//printf((const char *)USART_RX_BUF);
					//printf("\r\n");
					do_recv(USART_RX_BUF);
					//==================
					USART_RX_STA = 0;
					memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));	
				}
			}
			else //还没收到0X0D
			{	
				if (Res == 0x0d)
				{ 
					USART_RX_STA |= 0x4000;	//接收到回车的前一字节  置位状态寄存器
				}
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF] = Res;				//将接收的数据 存入数组中
					USART_RX_STA++;											//长度+1 为下一次做准备
					if (USART_RX_STA > (USART_REC_LEN-1)) 
					{ 
						USART_RX_STA = 0;	//接收数据错误,重新开始接收
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

int g_suspend = 0;		//PC主机是否已经休眠
int g_has_reply = 0;	//是否收到PC端的应答

//给虚拟机发送 <按下电源按钮> 命令
//参数：第几台虚拟机
void vm_power_press(u8 index) 
{
	if (g_suspend) {	//主机休眠了，需要先唤醒
		//...
		g_suspend = 0;	//清标志位
		//...
		{
			int i;
			for (i = 0; i < 5; i++) {	//最多尝试五次
				physical_power_press();	//发电源信号唤醒PC主机
				//...
				delay_ms_with_dog(300);	//等待300毫秒
				//...
				g_has_reply = 0;		//先清标志位
	   			printf("type:online:token:%u\r\n", g_token);	//询问PC端是否已经醒来
				delay_ms_with_dog(200);							//等待200毫秒
				if (g_has_reply) {	//标志位被置位,证明收到PC端的回复,证明PC端已经醒来
					//...
					break;	//醒来就可以直接退出循环了
				}
			}
		}
	}
	//...
	printf("type:power:vm:%u:token:%u\r\n", index, g_token);
}

//给虚拟机发送 <按下重启按钮> 命令
//参数：第几台虚拟机
void vm_reset_press(u8 index) 
{
	printf("type:reset:vm:%u:token:%u\r\n", index, g_token);
}

#define TYPE_HANDSHAKE 1
#define TYPE_VERSION   2
#define TYPE_SUSPEND   3
#define TYPE_ONLINE    4

//处理PC发过来的指令,主要包括两个命令,一个是握手指令,另外一个是获取控制板的版本号
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
		g_suspend = 1;		//收到PC端的通知,PC端休眠了
	} else if (type == TYPE_ONLINE) {
		g_has_reply = 1;	//控制板会向PC端询问是否已经醒来,得到这个应答代表PC端已经醒来了
	}
}
