#include "stm32f10x.h"                  // Device header
#include "DHT11.h"
#include "ESP8266.h"
#include "Serial.h"
#include <stdio.h>
#include <string.h>
#include <Delay.h>

u8 upload = 0;
extern u8 inventory[];
/*定时器中断初始化*/
void TIM3IT_Init(void)
{	
	/*配置TIM参数*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);			//开启TIM3时钟信号
	TIM_InternalClockConfig(TIM3);								//选择内部TIM3时钟
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 50000 - 1;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;					//PSC 此时按照公式 一秒记一次
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//关闭重复计数器
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);							//使能TIM3中断,记为更新中断
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);

	/*配置NVIC优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//二级组优先
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			//选择TIM3中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//配置响应优先级
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3,ENABLE);									//使能TIM3外设
	
}



void DataUpload(void)
{
	
	if(upload == 1)
	{
		ESP8266_Send(0,"humi",humi);
		Delay_ms(50);
		
		ESP8266_Send(0,"temp",temp);
		Delay_ms(50);
		
		ESP8266_Send(0,"Apple",inventory[0]);
		Delay_ms(50);
		
		ESP8266_Send(0,"Banana",inventory[2]);
		Delay_ms(50);
		
		ESP8266_Send(0,"Orange",inventory[4]);
		Delay_ms(50);
		
		ESP8266_Send(0,"Mango",inventory[6]);
	}
	upload = 0;
}



void TIM3_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)						//中断标志位判断，确认进入中断
	{	
		upload = 1;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

}




