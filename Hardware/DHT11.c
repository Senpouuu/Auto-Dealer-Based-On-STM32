#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "DHT11.h"
#include "OLED.h"
#include "ESP8266.h"
#include "Serial.h"
#include "String.h"

u8 count;
u8 humi,temp,dechumi,dectemp;


/*GPIO����ʱ��*/
void DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}




/*����dht11(GPIO)ģʽ��1��� 0����*/
void DHT_Mode(u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStrucuture;
	if(mode)
	{
		GPIO_InitStrucuture.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStrucuture.GPIO_Pin = DHT11;
		GPIO_InitStrucuture.GPIO_Speed = GPIO_Speed_50MHz;
	}
	else
	{
		GPIO_InitStrucuture.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStrucuture.GPIO_Pin = DHT11;
		GPIO_InitStrucuture.GPIO_Speed = GPIO_Speed_50MHz;
	}
	GPIO_Init(DHT11_Port,&GPIO_InitStrucuture);
}



/*DHT11��ʼ�ź�*/
void DHT_Sends_Start(void)
{
	DHT_Mode(1);												//GPIO���
	GPIO_ResetBits(DHT11_Port,DHT11);							//����
	Delay_ms(18);												//�ȴ�18ms
	GPIO_SetBits(DHT11_Port,DHT11);								//����
	Delay_us(40);												//�ȴ�40us
}



/*�ȴ�DHT11��Ӧ���*/
void DHT_Sends_Response(void)
{
	DHT_Mode(0);												//GPIO����

	/*�ȴ��͵�ƽ����*/
	while((GPIO_ReadInputDataBit(DHT11_Port,DHT11) == 0 ) && (count<100))
	{
		count++;
		Delay_us(1);										
	}
	count=0;

	/*�ȴ��ߵ�ƽ����*/
	while((GPIO_ReadInputDataBit(DHT11_Port,DHT11) == 1 ) && (count<100))
	{
		count++;
		Delay_us(1);										
	}
	count=0;
}




/*DHT11������λ*/
u8 DHT11_Read_Byte(void)
{
	u8 data = 0;
	for(u8 i=0;i<8;i++)
	{
		DHT_Mode(0);											//GPIO����

		/*�ȴ��͵�ƽ����*/
		while((GPIO_ReadInputDataBit(DHT11_Port,DHT11) == 0 ) && (count<100))
		{
			count++;
			Delay_us(1);											
		}
		count=0;
			
		data<<=1;													//data��λ��ʼ��
		
		/*д����1*/
		Delay_us(40);												//�ߵ�ƽ40us		
		if(GPIO_ReadInputDataBit(DHT11_Port,DHT11) == 1)
		{
			data |= 0x01;											//��λ�Ƹ�λ
			/*�ȴ��ߵ�ƽ����*/
			while((GPIO_ReadInputDataBit(DHT11_Port,DHT11) == 1 ) && (count<100))
			{
				count++;
				Delay_us(1);											
			}
			count=0;
		}
	}
	return data;
}




/*DHT11����λת��*/
void DHT11_Read_data(u8 *humi,u8 *temp,u8 *dechumi,u8 *dectemp)
{
	u8 DATA[5]={0,0,0,0,0};											//��ʼ��

	DHT_Sends_Start();
	DHT_Sends_Response();

	for(u8 i=0;i<5;i++)
	{
		DATA[i] = DHT11_Read_Byte();
	}
	Delay_ms(1);													//�ȴ�50us

	/*ʪ��+�¶�У��*/
	if((DATA[0]+DATA[1]+DATA[2]+DATA[3]) == DATA[4])
	{
		*humi = DATA[0];
		*dechumi = DATA[1];
		*temp = DATA[2];
		*dectemp = DATA[3];
	}
	else
	{
		for(u8 i=0;i<5;i++)
		DATA[i] = 0;
	}
}






