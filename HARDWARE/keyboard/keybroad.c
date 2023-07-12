#include "keybroad.h"

//初始化薄膜键盘
/*
	PD6、PD7、PC6、PC8、PC11、PE5、PA6、PG9
 	r1   r2    r3   r4   c1   c2   c3    c4
	5、6、7、8、9、11
*/
void Keybroad_Config_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG,ENABLE);
	GPIO_InitTypeDef GPIOInit;
	GPIOInit.GPIO_Mode  = GPIO_Mode_IN;
	GPIOInit.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIOInit.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOInit.GPIO_Pin   = PIN6|PIN8;
	GPIO_Init(GPIOC,&GPIOInit);
	GPIOInit.GPIO_Pin   = PIN6|PIN7;
	GPIO_Init(GPIOD,&GPIOInit);
	
	GPIOInit.GPIO_Mode  = GPIO_Mode_OUT;
	GPIOInit.GPIO_OType	= GPIO_OType_PP;
	GPIOInit.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIOInit.GPIO_Pin = PIN6;
	GPIO_Init(GPIOA,&GPIOInit);
	
	GPIOInit.GPIO_Pin   = PIN5;
	GPIO_Init(GPIOE,&GPIOInit);
	
	GPIOInit.GPIO_Pin   = PIN11;
	GPIO_Init(GPIOC,&GPIOInit);
	
	GPIOInit.GPIO_Pin   = PIN9;
	GPIO_Init(GPIOG,&GPIOInit);

}


//初始化薄膜键盘
/*
	PD6、PD7、PC6、PC8、PC11、PE5、PA6、PG9
 	r1   r2    r3   r4   c1   c2   c3    c4
	5、6、7、8、9、11
*/
u8 Keybroad_Input(u8 mode)
{
	u8 key = 0;
	static u8 key_up=1;  // 按键按松开标志	
	
	if(mode)
		key_up=1;    // 支持连按
//	GPIO_ResetBits(GPIOC,PIN11);
//	GPIO_ResetBits(GPIOE,PIN5);    
//	GPIO_ResetBits(GPIOA,PIN6);
//	GPIO_ResetBits(GPIOG,PIN9);
	PCout(11) = 0;
	PEout(5) = 0;
	PAout(6) = 0;
	PGout(9) = 0;
	
	if(key_up && (PDin(6) == 0 || PDin(7) == 0 || PCin(6) == 0 || PCin(8) == 0))
	{

		delay_ms(10);
		key_up = 0;
		if(PDin(6) == 0 || PDin(7) == 0 || PCin(6) == 0 || PCin(8) == 0)
		{

			if(PDin(6) == 0)
				key = 1;
			else if(PDin(7) == 0)
				key = 4;
			else if(PCin(6) == 0)
				key = 7;
			else if(PCin(8) == 0)
				key = 11;

			PCout(11) = 0;
			PEout(5) = 1;
			PAout(6) = 1;
			PGout(9) = 1;
			if(PDin(6) == 0 || PDin(7) == 0 || PCin(6) == 0 ||PCin(8) == 0)key+=1;
			

			PCout(11) = 1;
			PEout(5) = 0;
			PAout(6) = 1;
			PGout(9) = 1;			
			if(PDin(6) == 0 || PDin(7) == 0 || PCin(6) == 0 ||PCin(8) == 0)key+=2;
			

			PCout(11) = 1;
			PEout(5) = 1;
			PAout(6) = 0;
			PGout(9) = 1;	
			if(PDin(6) == 0 || PDin(7) == 0 || PCin(6) == 0 ||PCin(8) == 0)key+=3;
			

			PCout(11) = 1;
			PEout(5) = 1;
			PAout(6) = 1;
			PGout(9) = 0;	
			if(PDin(6) == 0 || PDin(7) == 0 || PCin(6) == 0 ||PCin(8) == 0)key+=14;
			
			if(key == 13)
				key = 1;
			return key-1;
		}	
		
	}
	else if((PDin(6) == 1 && PDin(7) == 1 && PCin(6) == 1 && PCin(8) == 1))
		key_up=1; 

	return 30;


}









