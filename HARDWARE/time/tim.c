#include "tim.h"



void TIM3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler =  8400-1;     		//预分频 8400
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_Period=10000-1;					//10000个计数值	10000	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//分频值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;				//
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel =TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		PFout(9)=!PFout(9);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
	
}




