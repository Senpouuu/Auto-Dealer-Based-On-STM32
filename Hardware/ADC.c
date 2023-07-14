#include "stm32f10x.h"                  // Device header
#include "DMA.h"
uint16_t ADC_vaule[4];

void AD_Init(void)
{	
	/*RCC����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 		//72/6 = 12Mhz
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ADCģ������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*ADC����*/
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;							//����ΪADC����ģʽ							
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			//��ʹ���ⲿ����ģʽ
	/*ת��ģʽ���ã�����ת����ɨ�裩*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//����ת�� 	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;								//��ɨ��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 4;										//4��ͨ��	
		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);	//���ù�����ͨ���˵�
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);	//���ù�����ͨ���˵�
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);	//���ù�����ͨ���˵�
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);	//���ù�����ͨ���˵�
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1,ENABLE);														//�ϵ�

	/*ADCУ׼*/
	ADC_ResetCalibration(ADC1);													//��ʼ��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);							//�ȴ���λУ׼���
	ADC_StartCalibration(ADC1);													//��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1) == SET);								//�ȴ�У׼���

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);										//�������ת��
	DriectMemoryAccess_Init();
}


void ADC_GetValue(void)
{
	DMA_Reset();
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);					//�ȴ�������ת�����
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);							//�ȴ�ת�����
	DMA_ClearFlag(DMA1_FLAG_TC1);											//�����־λ
}





