#include "key.h"

//����go����find����Ȼ����������
void Key_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;  //GPIO�ṹ��
	
	//1��ʹ��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//2��GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10;          //��9������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //����ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //�����ٶ���100MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	//��ʼ��GPIO����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //���� 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//3����ȡ���ŵĸߵ͵�ƽֵ
	// GPIO_ReadInputDataBit(); //PAin(0);

}


