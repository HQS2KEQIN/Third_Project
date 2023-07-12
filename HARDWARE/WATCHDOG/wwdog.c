#include "stm32f4xx.h"
void WWDG_INIT(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	WWDG_SetWindowValue(114);
	
	WWDG_Enable(127);
	
	NVIC_InitTypeDef NVICInit;
	NVICInit.NVIC_IRQChannel					= WWDG_IRQn;
	NVICInit.NVIC_IRQChannelCmd				= ENABLE;
	NVICInit.NVIC_IRQChannelPreemptionPriority	= 0;
	NVICInit.NVIC_IRQChannelSubPriority		= 0;
	NVIC_Init(&NVICInit);
	WWDG_ClearFlag();
	WWDG_EnableIT();
}

