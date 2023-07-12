#include "uart.h"

//================一、底层操作，不需要理解===================
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
 
int _sys_exit(int x)  //避免半主机模式
{ 
	x = x;
	return 0;
} 

//重定向printf
//即将数据发送到USART1，并显示
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);      
	return ch;
}



//====================二、串口初始化配置=====================
void Init_USART1(uint32_t baud)
{
	//GPIO结构体
	GPIO_InitTypeDef GPIO_InitStructure;

	//中断向量结构体，提供中断控制器
	NVIC_InitTypeDef NVIC_InitSructure;

	//串口结构体
	USART_InitTypeDef USART_InitSructure;
	
	
	//1、使能USART1硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//2、配置GPIO_PinAFConfig()
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;       //第9跟引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;     //复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽模式，增加驱动能力；
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //工作速度在100MHz,性能更强，消耗越大
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不需要上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;       //第10跟引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化
	
	//3、配置USART_Init()
	USART_InitSructure.USART_BaudRate   =  baud;                                  	//波特率
	USART_InitSructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件流控制
	USART_InitSructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                  //数据的收发功能
	USART_InitSructure.USART_Parity = USART_Parity_No ;                             //校验位
	USART_InitSructure.USART_StopBits = USART_StopBits_1;                           //停止位
	USART_InitSructure.USART_WordLength = USART_WordLength_8b;                      //数据位
	USART_Init(USART1, &USART_InitSructure);
	
	//4、NVIC
	NVIC_InitSructure.NVIC_IRQChannel =  USART1_IRQn;
	NVIC_InitSructure.NVIC_IRQChannelPreemptionPriority = 0x01;  //抢占优先级
	NVIC_InitSructure.NVIC_IRQChannelSubPriority = 0x01;         //响应优先级
	NVIC_InitSructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitSructure);

	//5、USART_Cmd(),使串口1工作
	USART_Cmd(USART1, ENABLE);
	
	//6、USART_ITConfig()，使其可接收数据
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
}



//=====================三、中断函数区域======================//
//实现串口中断
void  USART1_IRQHandler(void)
{
	uint8_t d;
	//检查是否有串口中断的请求
	if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//接收数据函数
		d = USART_ReceiveData(USART1);
		
		if(d == 'o')
		{
			printf("d = %c\r\n", d);
		}
		if(d == 'c')
		{
			printf("d = %c\r\n", d);
		}

		//清除中断标志位
		//想一想如果不清除会怎样?
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

}


















