#include "key.h"

//不是go就是find，不然就在你左右
void Key_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;  //GPIO结构体
	
	//1、使能硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//2、GPIO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10;          //第9个引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //输入模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉电阻
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //工作速度在100MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	//初始化GPIO设置
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //光敏 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//3、获取引脚的高低电平值
	// GPIO_ReadInputDataBit(); //PAin(0);

}


