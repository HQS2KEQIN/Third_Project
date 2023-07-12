#include "exit.h"






void exit0_gpio_init(uint8_t PreemptionPriority,uint8_t SubPriority)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //第0个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉电阻
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Line=EXTI_Line0;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=SubPriority;
	NVIC_Init(&NVIC_InitStruct);

}



void exit2_gpio_init(uint8_t PreemptionPriority,uint8_t SubPriority)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          //第2个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉电阻
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);

	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Line=EXTI_Line2;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=SubPriority;
	NVIC_Init(&NVIC_InitStruct);

}


void exit3_gpio_init(uint8_t PreemptionPriority,uint8_t SubPriority)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;          //第3个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉电阻
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=SubPriority;
	NVIC_Init(&NVIC_InitStruct);
}


void exit4_gpio_init(uint8_t PreemptionPriority,uint8_t SubPriority)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;          //第0个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉电阻
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=SubPriority;
	NVIC_Init(&NVIC_InitStruct);

	
}

//void EXTI0_IRQHandler(void)
//{
//	if(!PAin(0))
//	{
//		delay_ms(50);
//		while(!PAin(0))
//		{
//			if(i<100)
//				i++;
//			TIM_SetCompare1(TIM14,i);
//			delay_ms(10);
//		}
//	}
//	
//	
//	EXTI_ClearITPendingBit(EXTI_Line0);
//}

//void EXTI2_IRQHandler(void)
//{
//	if(!PEin(2))
//	{
//		delay_ms(50);
//		while(!PEin(2))
//		{
//			if(i>0)
//				i--;
//			TIM_SetCompare1(TIM14,i);
//			
//			delay_ms(10);
//		}
//		
//		
//	
//	}

//	EXTI_ClearITPendingBit(EXTI_Line2);
//}

//void EXTI3_IRQHandler(void)
//{
//	if(!PEin(3))
//	{
//		delay_ms(50);
//		while(!PEin(3))
//		{
//			if(led3<1000)
//				led3+=3;
//		
//			TIM_SetCompare4(TIM1,led3);
//			delay_ms(10);
//		}
//	
//	}

//	EXTI_ClearITPendingBit(EXTI_Line3);
//}

//void EXTI4_IRQHandler(void)
//{
//	if(!PEin(4))
//	{
//		delay_ms(50);
//		while(!PEin(4))
//		{
//			if(led3>0)
//				led3--;
//			TIM_SetCompare4(TIM1,led3);
//			delay_ms(10);
//		}
//		
//	}
//	EXTI_ClearITPendingBit(EXTI_Line4);
//}


