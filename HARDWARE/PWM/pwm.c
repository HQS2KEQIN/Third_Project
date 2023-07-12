#include "pwm.h"

void pwm0_init(void)
{
	//����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	//����GPIOΪ����ģʽ
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;        //��9������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //��������������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�����ٶ���100MHZ
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	//���ö�ʱ��ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler =  8400-1;     		//Ԥ��Ƶ 8400
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period=100-1;					//10000������ֵ	10000	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;				//
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//����PWMģʽ
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OutputState =TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 50-1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);

	//ʹ��
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable);
	TIM_Cmd(TIM14,ENABLE);
	TIM_CtrlPWMOutputs(TIM14,ENABLE);
	
	
}

void pwm1_init(void)
{
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//����GPIOΪ����ģʽ
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;        //��14������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�����ٶ���100MHZ
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	//���ö�ʱ��ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler =  16800-1;     		//Ԥ��Ƶ 16800
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;					//1000������ֵ	1000	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	//
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//����PWMģʽ
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OutputNState =TIM_OutputNState_Disable;
	TIM_OCInitStruct.TIM_Pulse = 50-1;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	
	//ʹ��
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	
}


void pwm2_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//����GPIOΪ����ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_High_Speed;//��Ӧ�ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;//û������������
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	//ѡ���ö���PE13--TIM1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	//���ö�ʱ��ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler 		= 16800-1;//Ԥ��Ƶֵ��0~65535
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;//��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_Period			= 1000-1;//��100������Ϊһ�Σ�1s�ͻ���100��
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;//����ģʽ�����ϼ���
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	//����PWMģʽ��TIM1_CH3(��ʱ��1��ͨ��3)
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	TIM_OCInitStruct.TIM_OCMode			= TIM_OCMode_PWM1;//PWMģʽ��1
//	TIM_OCInitStruct.TIM_OutputNState	= TIM_OutputNState_Enable;//״̬��ʹ��
//	TIM_OCInitStruct.TIM_Pulse			= 50-1;//�Ƚ�ֵ��PWM1����ֵ<�Ƚ�ֵ=��Ч
//	TIM_OCInitStruct.TIM_OCNPolarity	= TIM_OCNPolarity_High;//��Ч��ƽ����
//	TIM_OCInitStruct.TIM_OCIdleState 	= TIM_OCIdleState_Set;
//	TIM_OCInitStruct.TIM_OCNIdleState   = TIM_OCNIdleState_Set;
//	TIM_OC3Init(TIM1,&TIM_OCInitStruct);



	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	
	TIM_OCInitStruct.TIM_OutputNState =TIM_OutputNState_Disable;
	
	TIM_OCInitStruct.TIM_Pulse = 50-1;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	//TIM_OCInitStruct.TIM_OCIdleState 	= TIM_OCIdleState_Set;
	//TIM_OCInitStruct.TIM_OCNIdleState   = TIM_OCNIdleState_Set;
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
	//ʹ�ܣ���ʱ��ͨ��3����ʱ��
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);

}


//void pwm2_init(void)
//{
//	//����ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
//	//����GPIOΪ����ģʽ
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;        //��14������  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //����ģʽ
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����ģʽ
//	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�����ٶ���100MHZ
//	GPIO_Init(GPIOE, &GPIO_InitStructure); 
//	
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
//	//���ö�ʱ��ʱ����Ԫ
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_TimeBaseInitStruct.TIM_Prescaler =  16800-1;     		//Ԥ��Ƶ 16800
//	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up; //���ϼ���
//	TIM_TimeBaseInitStruct.TIM_Period=1000-1;					//1000������ֵ	1000	
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//��Ƶֵ
//	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
//	//
//	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
//	
//	//����PWMģʽ
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCInitStruct.TIM_OutputNState =TIM_OutputNState_Disable;
//	TIM_OCInitStruct.TIM_Pulse = 50-1;
//	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
//	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
//	
//	//ʹ��
//	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
//	
//	TIM_Cmd(TIM1,ENABLE);
//	
//	TIM_CtrlPWMOutputs(TIM1,ENABLE);


//}







