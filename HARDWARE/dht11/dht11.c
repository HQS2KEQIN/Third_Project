#include "sys.h"
#include "stm32f4xx.h"
#include "delay.h"
//在main函数的使用




/*//获取温湿度数据
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
	GPIO_InitTypeDef GPIO_InitStructure; //GPIO结构体
	//使能端口G硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	//配置PG9引脚为输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//第9根引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出，增加驱动能力就是增加输出电流，还有吸收灌电流
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//工作速度100MHz，工作速度越高，功耗就会越高
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//不需要上下拉电阻
	GPIO_Init(GPIOG, &GPIO_InitStructure);
		
	
	//模块还没有工作的时候，高电平
	PGout(9)=1;
}



void dht11_pin_mode(u8 i)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	 								//GPIO结构体
							//第9根引脚
	GPIOG->PUPDR|=(1<<2*9);
	GPIOG->PUPDR&=~(1<<(2*9+1));//不需要上下拉电阻	
	
	if(i)
	{
		//配置PG9引脚为输出模式
		GPIOG->MODER|=1<<(2*9);		
		GPIOG->MODER&=~(1<<(2*9+1));								//输出模式
		GPIOG->OTYPER&=~(1<<9);//推挽输出，增加驱动能力就是增加输出电流，还有吸收灌电流
		GPIOG->OSPEEDR|=0x03<<(2*9);//工作速度100MHz，工作速度越高，功耗就会越高			
	}
	else 
	{
		GPIOG->MODER&=1<<9*2;	
		GPIOG->MODER&=1<<(1+9*2);	//输入模式
	}
	
	
	
}


int32_t dht11_read_data(uint8_t *pbuf)
{
	uint32_t t=0;
	uint32_t i,j,m;
	uint8_t d=0;
	
	//配置PG9引脚为输出模式
	dht11_pin_mode(1);
	
	PGout(9)=0;
	
	
	//延时20ms以上
	delay_ms(20);
	
	
	PGout(9)=1;
	
	delay_us(30);
	
	//配置PG9引脚为输入模式
	dht11_pin_mode(0);
	GPIOG->MODER&=1<<9*2;	
	GPIOG->MODER&=1<<(1+9*2);	//输入模式
	
	//等待低电平出现
	while(PGin(9))
	{
		LED0(Bit_RESET);
	}
	
	LED0(Bit_SET);
	//测量低电平的持续时间是否合法
	t=0;
	while(PGin(9)==0)
	{
		
		t++;
		
		delay_us(1);
		
		//发现超时
		if(t >=100)
			return -1;
	}
	
	//测量高电平的持续时间是否合法
	t=0;
	while(PGin(9))
	{
		
		t++;
		
		delay_us(1);
		
		//发现超时
		if(t >=100)
			return -2;
	}	

	//获取5个字节
	for(j=0; j<5; j++)
	{
		d=0;
		
		for(i=0; i<8; i++)
		{
		
			//等待低电平持续完毕
			while(PGin(9)==0);
			
			//延时40us~60us
			delay_us(40);
			
			//判断PG9引脚的电平
			if(PGin(9) == 1)
			{
				d|=1<<(7-i);
				
				//等待高电平持续完毕
				while(PGin(9));
			}
		}	
	
		pbuf[j]=d;
	}
	
	//校验数据
	m = pbuf[0]+pbuf[1]+pbuf[2]+pbuf[3];
	
	if(pbuf[4]!= (m&0xFF))
		return -3;
	
	//结束获取数据，延时100us
	delay_us(100);


	return 0;
}








