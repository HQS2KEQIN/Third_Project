#include "pwm.h"

void pwm0_init(void)
{
	//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	//配置GPIO为复用模式
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;        //第9个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //不用上下拉电阻
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //工作速度在100MHZ
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	//配置定时器时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler =  8400-1;     		//预分频 8400
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_Period=100-1;					//10000个计数值	10000	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//分频值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;				//
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//配置PWM模式
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OutputState =TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 50-1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);

	//使能
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable);
	TIM_Cmd(TIM14,ENABLE);
	TIM_CtrlPWMOutputs(TIM14,ENABLE);
	
	
}

void pwm1_init(void)
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//配置GPIO为复用模式
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;        //第14个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //工作速度在100MHZ
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	//配置定时器时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler =  16800-1;     		//预分频 16800
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;					//1000个计数值	1000	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//分频值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	//
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//配置PWM模式
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OutputNState =TIM_OutputNState_Disable;
	TIM_OCInitStruct.TIM_Pulse = 50-1;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	
	//使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	
}


void pwm2_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//配置GPIO为复用模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_High_Speed;//响应速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;//没有上下拉电阻
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	//选择复用对象：PE13--TIM1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	//配置定时器时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler 		= 16800-1;//预分频值：0~65535
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;//分频值
	TIM_TimeBaseInitStruct.TIM_Period			= 1000-1;//以100个计数为一段，1s就会有100段
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;//计数模式：向上计数
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	//配置PWM模式：TIM1_CH3(定时器1的通道3)
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	TIM_OCInitStruct.TIM_OCMode			= TIM_OCMode_PWM1;//PWM模式：1
//	TIM_OCInitStruct.TIM_OutputNState	= TIM_OutputNState_Enable;//状态：使能
//	TIM_OCInitStruct.TIM_Pulse			= 50-1;//比较值：PWM1计数值<比较值=有效
//	TIM_OCInitStruct.TIM_OCNPolarity	= TIM_OCNPolarity_High;//有效电平：高
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
	//使能：定时器通道3、定时器
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);

}


//void pwm2_init(void)
//{
//	//开启时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
//	//配置GPIO为复用模式
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;        //第14个引脚  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //复用模式
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽模式
//	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //工作速度在100MHZ
//	GPIO_Init(GPIOE, &GPIO_InitStructure); 
//	
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
//	//配置定时器时基单元
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_TimeBaseInitStruct.TIM_Prescaler =  16800-1;     		//预分频 16800
//	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up; //向上计数
//	TIM_TimeBaseInitStruct.TIM_Period=1000-1;					//1000个计数值	1000	
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//分频值
//	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
//	//
//	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
//	
//	//配置PWM模式
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCInitStruct.TIM_OutputNState =TIM_OutputNState_Disable;
//	TIM_OCInitStruct.TIM_Pulse = 50-1;
//	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
//	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
//	
//	//使能
//	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
//	
//	TIM_Cmd(TIM1,ENABLE);
//	
//	TIM_CtrlPWMOutputs(TIM1,ENABLE);


//}







