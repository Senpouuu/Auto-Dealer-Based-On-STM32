#include "stm32f10x.h"                  // Device header
#include "DHT11.h"
#include "ESP8266.h"
#include "Serial.h"
#include <stdio.h>
#include <string.h>
#include <Delay.h>

u8 upload = 0;
extern u8 inventory[];
/*��ʱ���жϳ�ʼ��*/
void TIM3IT_Init(void)
{	
	/*����TIM����*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);			//����TIM3ʱ���ź�
	TIM_InternalClockConfig(TIM3);								//ѡ���ڲ�TIM3ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//ѡ���Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//ѡ�����ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 50000 - 1;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;					//PSC ��ʱ���չ�ʽ һ���һ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//�ر��ظ�������
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);							//ʹ��TIM3�ж�,��Ϊ�����ж�
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);

	/*����NVIC���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			//ѡ��TIM3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3,ENABLE);									//ʹ��TIM3����
	
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
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)						//�жϱ�־λ�жϣ�ȷ�Ͻ����ж�
	{	
		upload = 1;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

}




