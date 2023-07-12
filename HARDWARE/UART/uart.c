#include "uart.h"

//================һ���ײ����������Ҫ���===================
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
 
int _sys_exit(int x)  //���������ģʽ
{ 
	x = x;
	return 0;
} 

//�ض���printf
//�������ݷ��͵�USART1������ʾ
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);      
	return ch;
}



//====================�������ڳ�ʼ������=====================
void Init_USART1(uint32_t baud)
{
	//GPIO�ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;

	//�ж������ṹ�壬�ṩ�жϿ�����
	NVIC_InitTypeDef NVIC_InitSructure;

	//���ڽṹ��
	USART_InitTypeDef USART_InitSructure;
	
	
	//1��ʹ��USART1Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//2������GPIO_PinAFConfig()
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;       //��9������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;     //����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����ģʽ����������������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�����ٶ���100MHz,���ܸ�ǿ������Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����Ҫ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //��ʼ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;       //��10������
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //��ʼ��
	
	//3������USART_Init()
	USART_InitSructure.USART_BaudRate   =  baud;                                  	//������
	USART_InitSructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ��������
	USART_InitSructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                  //���ݵ��շ�����
	USART_InitSructure.USART_Parity = USART_Parity_No ;                             //У��λ
	USART_InitSructure.USART_StopBits = USART_StopBits_1;                           //ֹͣλ
	USART_InitSructure.USART_WordLength = USART_WordLength_8b;                      //����λ
	USART_Init(USART1, &USART_InitSructure);
	
	//4��NVIC
	NVIC_InitSructure.NVIC_IRQChannel =  USART1_IRQn;
	NVIC_InitSructure.NVIC_IRQChannelPreemptionPriority = 0x01;  //��ռ���ȼ�
	NVIC_InitSructure.NVIC_IRQChannelSubPriority = 0x01;         //��Ӧ���ȼ�
	NVIC_InitSructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitSructure);

	//5��USART_Cmd(),ʹ����1����
	USART_Cmd(USART1, ENABLE);
	
	//6��USART_ITConfig()��ʹ��ɽ�������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
}



//=====================�����жϺ�������======================//
//ʵ�ִ����ж�
void  USART1_IRQHandler(void)
{
	uint8_t d;
	//����Ƿ��д����жϵ�����
	if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//�������ݺ���
		d = USART_ReceiveData(USART1);
		
		if(d == 'o')
		{
			printf("d = %c\r\n", d);
		}
		if(d == 'c')
		{
			printf("d = %c\r\n", d);
		}

		//����жϱ�־λ
		//��һ����������������?
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

}


















