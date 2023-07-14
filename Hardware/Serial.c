#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

//uint8_t Serial_RxFlag;
char Serial_RxPacket[250];
int pRxPacket;

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure); 

	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure); 
		
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;											//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;						//串口模式（发送）+ （接收）
	USART_InitStructure.USART_Parity = USART_Parity_No;									//校验模式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//字长
	USART_Init(USART1,&USART_InitStructure);

	/*串口中断配置*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);										//开启RXNE标志位中断
	/*配置NVIC中断优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//二级组优先
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//选择USART1中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//配置响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);														//发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);							//等待数据进入寄存器
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}


void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}


/*printf重定向*/
int fputc(int ch,FILE *f )   
{
	USART_SendData(USART1,(uint8_t)ch);
	while(USART_GetFlagStatus (USART1,USART_FLAG_TC) == RESET);
	return ch;
}


/*带指令判断的中断函数*/
//void USART1_IRQHandler (void)
//{								
//	static uint8_t RxState = 0;								//接收数据状态
//	static uint8_t pRxPacket = 0;							//指定接收第几个数据;
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData = USART_ReceiveData(USART1);
//		
//		if (RxState == 0)									//0 进入等待包头程序
//		{
//			if (RxData == '#' && Serial_RxFlag == 0)		//扫描到包头
//			{
//				RxState = 1;								//跳转到状态1
//				pRxPacket = 0;	
//			}
//		}
//		else if (RxState == 1)								//1 进入接收数据程序
//		{
//			if (RxData == '\r')								//扫描到包尾
//				RxState = 2;								//跳转到状态2
//			else											//赋值数据包
//			{
//				Serial_RxPacket[pRxPacket] = RxData;		
//				pRxPacket ++ ;
//			}
//		}
//		else if (RxState == 2)								//2 进入等待包尾程序
//		{
//			if (RxData == '\n')								//包尾执行结束程序
//			{
//				RxState = 0;
//				Serial_RxPacket[pRxPacket] = '\0';
//				Serial_RxFlag = 1;
//			}
//		}
//		
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位
//	}
//} 
//



/*不带指令判断的中断函数*/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
//		Serial_RxFlag = 1;
		Serial_RxPacket[pRxPacket ++]=USART_ReceiveData(USART1);
		if((Serial_RxPacket[pRxPacket - 2] == '\r')|(Serial_RxPacket[pRxPacket - 1] == '\n'))  
		{
			Serial_RxPacket[pRxPacket - 2] = '\0';
			pRxPacket = 0;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位	
	}
}


