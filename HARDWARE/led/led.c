#include "led.h"


//不是go就是find，不然就在你左右
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  //GPIO结构体
	
	//1、使能硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA, ENABLE);
	
	//2、GPIO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;        //第9个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不用上下拉电阻
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //工作速度在100MHZ
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;        //

	//初始化GPIO设置
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//3、设置引脚高低电平
	GPIO_SetBits(GPIOF, GPIO_Pin_9); //引脚置高电平，熄灯
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13); //引脚置高电平，熄灯
	GPIO_SetBits(GPIOE, GPIO_Pin_14);
	
}






