

#include "beep.h"




void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  //GPIO结构体
	
	//1、使能硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//2、GPIO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;        //第9个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //不用上下拉电阻
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //工作速度在100MHZ
	GPIO_Init(GPIOF, &GPIO_InitStructure);           //初始化GPIO设置
	
	//3、设置引脚高低电平
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
	
	
}












