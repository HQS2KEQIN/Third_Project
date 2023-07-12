#include "sys.h"
#include "stm32f4xx.h"
#include "delay.h"
//��main������ʹ��




/*//��ȡ��ʪ������
		rt = dht11_read_data(buf);
		printf("rt=%X \r\n",rt);
		if(rt == 0)
		{
				printf("T:%d.%d H:%d.%d\r\n",buf[2],buf[3],buf[0],buf[1]);
			
		}
		delay_ms(5000);
*/	


void dht11_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //GPIO�ṹ��
	//ʹ�ܶ˿�GӲ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	//����PG9����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//��9������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//���������������������������������������������չ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�����ٶ�100MHz�������ٶ�Խ�ߣ����ľͻ�Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//����Ҫ����������
	GPIO_Init(GPIOG, &GPIO_InitStructure);
		
	
	//ģ�黹û�й�����ʱ�򣬸ߵ�ƽ
	PGout(9)=1;
}



void dht11_pin_mode(u8 i)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	 								//GPIO�ṹ��
							//��9������
	GPIOG->PUPDR|=(1<<2*9);
	GPIOG->PUPDR&=~(1<<(2*9+1));//����Ҫ����������	
	
	if(i)
	{
		//����PG9����Ϊ���ģʽ
		GPIOG->MODER|=1<<(2*9);		
		GPIOG->MODER&=~(1<<(2*9+1));								//���ģʽ
		GPIOG->OTYPER&=~(1<<9);//���������������������������������������������չ����
		GPIOG->OSPEEDR|=0x03<<(2*9);//�����ٶ�100MHz�������ٶ�Խ�ߣ����ľͻ�Խ��			
	}
	else 
	{
		GPIOG->MODER&=1<<9*2;	
		GPIOG->MODER&=1<<(1+9*2);	//����ģʽ
	}
	
	
	
}


int32_t dht11_read_data(uint8_t *pbuf)
{
	uint32_t t=0;
	uint32_t i,j,m;
	uint8_t d=0;
	
	//����PG9����Ϊ���ģʽ
	dht11_pin_mode(1);
	
	PGout(9)=0;
	
	
	//��ʱ20ms����
	delay_ms(20);
	
	
	PGout(9)=1;
	
	delay_us(30);
	
	//����PG9����Ϊ����ģʽ
	dht11_pin_mode(0);
	GPIOG->MODER&=1<<9*2;	
	GPIOG->MODER&=1<<(1+9*2);	//����ģʽ
	
	//�ȴ��͵�ƽ����
	while(PGin(9))
	{
		LED0(Bit_RESET);
	}
	
	LED0(Bit_SET);
	//�����͵�ƽ�ĳ���ʱ���Ƿ�Ϸ�
	t=0;
	while(PGin(9)==0)
	{
		
		t++;
		
		delay_us(1);
		
		//���ֳ�ʱ
		if(t >=100)
			return -1;
	}
	
	//�����ߵ�ƽ�ĳ���ʱ���Ƿ�Ϸ�
	t=0;
	while(PGin(9))
	{
		
		t++;
		
		delay_us(1);
		
		//���ֳ�ʱ
		if(t >=100)
			return -2;
	}	

	//��ȡ5���ֽ�
	for(j=0; j<5; j++)
	{
		d=0;
		
		for(i=0; i<8; i++)
		{
		
			//�ȴ��͵�ƽ�������
			while(PGin(9)==0);
			
			//��ʱ40us~60us
			delay_us(40);
			
			//�ж�PG9���ŵĵ�ƽ
			if(PGin(9) == 1)
			{
				d|=1<<(7-i);
				
				//�ȴ��ߵ�ƽ�������
				while(PGin(9));
			}
		}	
	
		pbuf[j]=d;
	}
	
	//У������
	m = pbuf[0]+pbuf[1]+pbuf[2]+pbuf[3];
	
	if(pbuf[4]!= (m&0xFF))
		return -3;
	
	//������ȡ���ݣ���ʱ100us
	delay_us(100);


	return 0;
}








