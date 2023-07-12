#include "fire.h"
void Fire_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIOInit;
	GPIOInit.GPIO_Pin	= PIN7;
	GPIOInit.GPIO_Mode	= GPIO_Mode_IN;
	GPIOInit.GPIO_Speed	= GPIO_High_Speed;
	GPIOInit.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIOInit);
}

void Fire_Detect(void)
{
	
	if(PCin(7)==1)
		PFout(8) = 1;

	else
		PFout(8) = 0;
}





