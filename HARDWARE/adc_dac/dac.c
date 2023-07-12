#include "stm32f4xx.h"
void DAC1_OUT_Init(void)
{
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	//����GOIO��ģ��ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//��ʼ��DAC
	DAC_InitTypeDef DAC_InitStruct;
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;//����Ҫ��ʱ���ṩ����
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;//����Ҫ���ɲ���
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;//���������ѹ
	DAC_Init(DAC_Channel_1,&DAC_InitStruct);
	//ʹ��DAC
	DAC_Cmd(DAC_Channel_1,ENABLE);
}

