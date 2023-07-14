#include "stm32f10x.h"                  // Device header
extern uint16_t ADC_vaule[4];

void DriectMemoryAccess_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	/*����*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				//����1����ַ ADC1�Ĵ�����ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//16λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ر�
	/*�洢��*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_vaule;					//����2����ַ	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//�ֽڷ�ʽ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//��������
	DMA_InitStructure.DMA_BufferSize = 4;										//�������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//���䷽�򣨴洢�����洢����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//Ӳ������or������� (ADC1����)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ѭ��ģʽ���洢�����洢������ʹ���Զ���װ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//���ȼ���
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);									//ADC1 �� DMA1
	
	ADC_DMACmd(ADC1,ENABLE);													//ADC1 DMA����Դ����
	DMA_Cmd(DMA1_Channel1,ENABLE);

}


void DMA_Reset(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);												//�ر�
	DMA_SetCurrDataCounter(DMA1_Channel1,4);									//���ü�����
	DMA_Cmd(DMA1_Channel1,ENABLE);	
}

