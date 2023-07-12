#include "adc.h"

void ADC_INIT(void)
{
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	//配置GPIO：模拟模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;//模拟模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	//选择ADC模式与输入通道
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonInitStruct.ADC_Mode	= ADC_Mode_Independent;//独立模式
	ADC_CommonInitStruct.ADC_Prescaler	= ADC_Prescaler_Div2;//预分频：84MHz/2=42MHz
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;//不需要数据映射，直接读取寄存器里的值
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;//两次采用之间的时间间隔: (1/42MHz)*5
	ADC_CommonInit(&ADC_CommonInitStruct);
	//初始化ADC
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Resolution	= ADC_Resolution_12b;//12分辨率：0~4095
	ADC_InitStruct.ADC_ScanConvMode	= DISABLE;//单通道，默认的是多通道
	ADC_InitStruct.ADC_ContinuousConvMode	= ENABLE;//连续转换数据
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;//不需要内部时钟脉冲
	//ADC_InitStruct.ADC_ExternalTrigConv	= //上面没有选择内部脉冲就不需要配置
	ADC_InitStruct.ADC_DataAlign	= ADC_DataAlign_Right;//右对齐方式
	ADC_InitStruct.ADC_NbrOfConversion	= 1;//上面选择的是单通道
	ADC_Init(ADC1,&ADC_InitStruct);
	//初始化通道: ADC1的通道5，单通道模式，采样时间周期再次*3
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_3Cycles);
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
}

//光敏电阻
void ADC_LIGHT_INIT(void)
{
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	//配置GPIO：模拟模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;//模拟模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStruct);	
	//选择ADC模式与输入通道
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonInitStruct.ADC_Mode	= ADC_Mode_Independent;//独立模式
	ADC_CommonInitStruct.ADC_Prescaler	= ADC_Prescaler_Div2;//预分频：84MHz/2=42MHz
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;//不需要数据映射，直接读取寄存器里的值
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;//两次采用之间的时间间隔: (1/42MHz)*5
	ADC_CommonInit(&ADC_CommonInitStruct);
	//初始化ADC
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Resolution	= ADC_Resolution_12b;//12分辨率：0~4095
	ADC_InitStruct.ADC_ScanConvMode	= DISABLE;//单通道，默认的是多通道
	ADC_InitStruct.ADC_ContinuousConvMode	= ENABLE;//连续转换数据
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;//不需要内部时钟脉冲
	//ADC_InitStruct.ADC_ExternalTrigConv	= //上面没有选择内部脉冲就不需要配置
	ADC_InitStruct.ADC_DataAlign	= ADC_DataAlign_Right;//右对齐方式
	ADC_InitStruct.ADC_NbrOfConversion	= 5;//上面选择的是单通道
	ADC_Init(ADC3,&ADC_InitStruct);
	//初始化通道: ADC3的通道7，单通道模式，采样时间周期再次*3
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,1,ADC_SampleTime_3Cycles);
	//使能ADC
	ADC_Cmd(ADC3,ENABLE);
}

void ADC1_IN6_Init(void)
{
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	//配置GPIO：模拟模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;//模拟模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	//选择ADC模式与输入通道
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonInitStruct.ADC_Mode	= ADC_Mode_Independent;//独立模式
	ADC_CommonInitStruct.ADC_Prescaler	= ADC_Prescaler_Div2;//预分频：84MHz/2=42MHz
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;//不需要数据映射，直接读取寄存器里的值
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;//两次采用之间的时间间隔: (1/42MHz)*5
	ADC_CommonInit(&ADC_CommonInitStruct);
	//初始化ADC
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Resolution	= ADC_Resolution_12b;//12分辨率：0~4095
	ADC_InitStruct.ADC_ScanConvMode	= DISABLE;//单通道，默认的是多通道
	ADC_InitStruct.ADC_ContinuousConvMode	= ENABLE;//连续转换数据
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;//不需要内部时钟脉冲
	//ADC_InitStruct.ADC_ExternalTrigConv	= //上面没有选择内部脉冲就不需要配置
	ADC_InitStruct.ADC_DataAlign	= ADC_DataAlign_Right;//右对齐方式
	ADC_InitStruct.ADC_NbrOfConversion	= 1;//上面选择的是单通道
	ADC_Init(ADC1,&ADC_InitStruct);
	//初始化通道: ADC1的通道5，单通道模式，采样时间周期再次*3
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_3Cycles);
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
}


