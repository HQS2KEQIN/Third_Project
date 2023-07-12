#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#define Trig PBout(6)
#define Echo PEin(6)

//���ų�ʼ��
void TE_PIN_INIT(void)
{
    GPIO_InitTypeDef GPIOinit;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE,ENABLE);
    GPIOinit.GPIO_Pin = GPIO_Pin_6;
    GPIOinit.GPIO_Mode = GPIO_Mode_OUT;
    GPIOinit.GPIO_OType = GPIO_OType_PP;
    GPIOinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIOinit.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(GPIOB,&GPIOinit);
	GPIOinit.GPIO_Mode = GPIO_Mode_IN;
	GPIOinit.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE,&GPIOinit);
	//������������Ϊ�͵�ƽ
	PBout(6) = 0;
}

//���
uint32_t Get_Distance(void)
{
	uint32_t t = 0;
	//����10us���ϵĸߵ�ƽ
	Trig = 1;
	delay_us(20);
	Trig = 0;
	//�ȴ����������������ջ����ź�
	while(Echo == 0)
	{
		t++;
		delay_us(1);
		//��ʱ����
		if(t>100000)
			return 1;
	}
	t = 0;
	while(Echo == 1)
	{
		t++;
		delay_us(1);
		if(t>100000)
			return 2;
	
	}
	return (t*34) / 100 /2;
	
}



