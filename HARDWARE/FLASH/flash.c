#include "stm32f4xx.h"
#include "newprintf.h"

#define FLAG FLASH_FLAG_EOP|FLASH_FLAG_OPERR| \
		FLASH_FLAG_WRPERR|FLASH_FLAG_PGPERR| \
		FLASH_FLAG_PGAERR|FLASH_FLAG_PGSERR \
		|FLASH_FLAG_RDERR
		
u8 FLASH_INIT(void)
{
	//����
	FLASH_Unlock();
	//�������״̬
	FLASH_ClearFlag(FLAG);
	//����
	if(FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3)!=FLASH_COMPLETE)
	{
		printf("������\n");
		return 1;
	}
	return 0;

}


