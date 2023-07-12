#include "led.h"


//����go����find����Ȼ����������
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  //GPIO�ṹ��
	
	//1��ʹ��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA, ENABLE);
	
	//2��GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;        //��9������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //��������������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�����ٶ���100MHZ
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;        //

	//��ʼ��GPIO����
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//3���������Ÿߵ͵�ƽ
	GPIO_SetBits(GPIOF, GPIO_Pin_9); //�����øߵ�ƽ��Ϩ��
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13); //�����øߵ�ƽ��Ϩ��
	GPIO_SetBits(GPIOE, GPIO_Pin_14);
	
}






