#include "adc.h"

void ADC_INIT(void)
{
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	//����GPIO��ģ��ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	//ѡ��ADCģʽ������ͨ��
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonInitStruct.ADC_Mode	= ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStruct.ADC_Prescaler	= ADC_Prescaler_Div2;//Ԥ��Ƶ��84MHz/2=42MHz
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;//����Ҫ����ӳ�䣬ֱ�Ӷ�ȡ�Ĵ������ֵ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;//���β���֮���ʱ����: (1/42MHz)*5
	ADC_CommonInit(&ADC_CommonInitStruct);
	//��ʼ��ADC
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Resolution	= ADC_Resolution_12b;//12�ֱ��ʣ�0~4095
	ADC_InitStruct.ADC_ScanConvMode	= DISABLE;//��ͨ����Ĭ�ϵ��Ƕ�ͨ��
	ADC_InitStruct.ADC_ContinuousConvMode	= ENABLE;//����ת������
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;//����Ҫ�ڲ�ʱ������
	//ADC_InitStruct.ADC_ExternalTrigConv	= //����û��ѡ���ڲ�����Ͳ���Ҫ����
	ADC_InitStruct.ADC_DataAlign	= ADC_DataAlign_Right;//�Ҷ��뷽ʽ
	ADC_InitStruct.ADC_NbrOfConversion	= 1;//����ѡ����ǵ�ͨ��
	ADC_Init(ADC1,&ADC_InitStruct);
	//��ʼ��ͨ��: ADC1��ͨ��5����ͨ��ģʽ������ʱ�������ٴ�*3
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_3Cycles);
	//ʹ��ADC
	ADC_Cmd(ADC1,ENABLE);
}

//��������
void ADC_LIGHT_INIT(void)
{
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	//����GPIO��ģ��ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStruct);	
	//ѡ��ADCģʽ������ͨ��
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonInitStruct.ADC_Mode	= ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStruct.ADC_Prescaler	= ADC_Prescaler_Div2;//Ԥ��Ƶ��84MHz/2=42MHz
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;//����Ҫ����ӳ�䣬ֱ�Ӷ�ȡ�Ĵ������ֵ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;//���β���֮���ʱ����: (1/42MHz)*5
	ADC_CommonInit(&ADC_CommonInitStruct);
	//��ʼ��ADC
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Resolution	= ADC_Resolution_12b;//12�ֱ��ʣ�0~4095
	ADC_InitStruct.ADC_ScanConvMode	= DISABLE;//��ͨ����Ĭ�ϵ��Ƕ�ͨ��
	ADC_InitStruct.ADC_ContinuousConvMode	= ENABLE;//����ת������
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;//����Ҫ�ڲ�ʱ������
	//ADC_InitStruct.ADC_ExternalTrigConv	= //����û��ѡ���ڲ�����Ͳ���Ҫ����
	ADC_InitStruct.ADC_DataAlign	= ADC_DataAlign_Right;//�Ҷ��뷽ʽ
	ADC_InitStruct.ADC_NbrOfConversion	= 5;//����ѡ����ǵ�ͨ��
	ADC_Init(ADC3,&ADC_InitStruct);
	//��ʼ��ͨ��: ADC3��ͨ��7����ͨ��ģʽ������ʱ�������ٴ�*3
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,1,ADC_SampleTime_3Cycles);
	//ʹ��ADC
	ADC_Cmd(ADC3,ENABLE);
}

void ADC1_IN6_Init(void)
{
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	//����GPIO��ģ��ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;//ģ��ģʽ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	//ѡ��ADCģʽ������ͨ��
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonInitStruct.ADC_Mode	= ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStruct.ADC_Prescaler	= ADC_Prescaler_Div2;//Ԥ��Ƶ��84MHz/2=42MHz
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;//����Ҫ����ӳ�䣬ֱ�Ӷ�ȡ�Ĵ������ֵ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;//���β���֮���ʱ����: (1/42MHz)*5
	ADC_CommonInit(&ADC_CommonInitStruct);
	//��ʼ��ADC
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Resolution	= ADC_Resolution_12b;//12�ֱ��ʣ�0~4095
	ADC_InitStruct.ADC_ScanConvMode	= DISABLE;//��ͨ����Ĭ�ϵ��Ƕ�ͨ��
	ADC_InitStruct.ADC_ContinuousConvMode	= ENABLE;//����ת������
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;//����Ҫ�ڲ�ʱ������
	//ADC_InitStruct.ADC_ExternalTrigConv	= //����û��ѡ���ڲ�����Ͳ���Ҫ����
	ADC_InitStruct.ADC_DataAlign	= ADC_DataAlign_Right;//�Ҷ��뷽ʽ
	ADC_InitStruct.ADC_NbrOfConversion	= 1;//����ѡ����ǵ�ͨ��
	ADC_Init(ADC1,&ADC_InitStruct);
	//��ʼ��ͨ��: ADC1��ͨ��5����ͨ��ģʽ������ʱ�������ٴ�*3
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_3Cycles);
	//ʹ��ADC
	ADC_Cmd(ADC1,ENABLE);
}


