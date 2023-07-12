#include "stm32f4xx.h"
void DAC1_OUT_Init(void)
{
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	//配置GOIO：模拟模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;//模拟模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//初始化DAC
	DAC_InitTypeDef DAC_InitStruct;
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;//不需要定时器提供脉冲
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;//不需要生成波形
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;//允许输出电压
	DAC_Init(DAC_Channel_1,&DAC_InitStruct);
	//使能DAC
	DAC_Cmd(DAC_Channel_1,ENABLE);
}

