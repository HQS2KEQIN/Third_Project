

#include "beep.h"




void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  //GPIO�ṹ��
	
	//1��ʹ��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//2��GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;        //��9������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //��������������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�����ٶ���100MHZ
	GPIO_Init(GPIOF, &GPIO_InitStructure);           //��ʼ��GPIO����
	
	//3���������Ÿߵ͵�ƽ
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
	
	
}












