#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdio.h>
#include "Serial.h"
#include "Delay.h"
#include "LED.h"


const char* ProductID="602192";
const char* UserID="334768";
const char* DeviceID="1080235402";
const char* Device_AuthID="123456";



/*����OneNet*/
void ESP8266_Init(void)
{
	Serial_Init();

	while(1)
	{
		if (strstr((const char*)Serial_RxPacket,"WIFI GOT IP"))										//�ȴ�wifi����
		{
				memset(Serial_RxPacket,0,250);														//����������
				break;
		}
	}
	Delay_ms(2000);
	
	memset(Serial_RxPacket,0,250);
	printf("AT+IOTSTATUS\r\n");							//���õ�ַ
	Delay_ms(2000);
	
	memset(Serial_RxPacket,0,250);
	printf("AT+IOTCFG=%s,%s,%s\r\n",DeviceID,ProductID,Device_AuthID);							//���õ�ַ
	Delay_ms(2000);
	

}


/*ESP��������
ָ��AT+IOTSEND=a,b,c,(d)
a��0����������ֵ����
1���������ַ�������
2��������gps
b����������
c������ֵ
d������ֵ (ֻ���ϴ�GPSʱ�Ż��õ�)*/
void ESP8266_Send(uint8_t a,char* b,float c)
{
		printf("AT+IOTSEND=%d,%s,%.1f\r\n",a,b,c);												
}



