#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void Beep_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
}

void BEEP_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


void BEEP_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

void BEEP_Alert(void)
{
	BEEP_ON();
	Delay_ms(50);
	BEEP_OFF();
}

 void BEEP_Turn(void)
{
	/*����ǵ͵�ƽ���͸���һ���෴���źţ��ߵ�ƽͬ��*/
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0)
		GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
	else
		GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
}
