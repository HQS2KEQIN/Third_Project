#include "stm32f4xx.h"

void IWDG_INIT(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	IWDG_SetReload(500);
		
	IWDG_ReloadCounter();
	
	IWDG_Enable();
	


}



