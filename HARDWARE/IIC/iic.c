#include "iic.h"


void IIC_GPIO_INIT(void)
{
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//配置GPIOB8、9为输出
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed	= GPIO_High_Speed;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
}

void SDA_Pin_Mode(GPIOMode_TypeDef GPIO_Mode)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode;
	GPIO_InitStruct.GPIO_Speed	= GPIO_High_Speed;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
}


void IIC_Start(void)
{
	//确保当前为输出模式
	SDA_Pin_Mode(GPIO_Mode_OUT);
	//确保时钟线和数据线一开始为高电平
	SCL 	= 1;
	SDA_W 	= 1;
	//先持续一段时间高电平再修改数据线
	delay_us(5);
	SDA_W 	= 0;
	//接着时钟线也要拉低
	delay_us(5);
	SCL 	= 0;//一个周期后再将时钟线拉低
	delay_us(5);//钳住IIC总线
}

void IIC_Stop(void)
{
	SDA_Pin_Mode(GPIO_Mode_OUT);
	SCL 	= 1;//确保时钟线为高电平
	SDA_W 	= 0;//数据线为低电平
	delay_us(5);
	SDA_W	= 1;//再将数据线拉高
	delay_us(5);


}
//接收应答信号的函数
uint8_t IIC_RECV_ACK(void)
{
	//确保当前为输入模式
	SDA_Pin_Mode(GPIO_Mode_IN);
	//先将时钟线拉高
	SCL	= 1;
	delay_us(5);
	uint8_t ACK = 0;
	if(SDA_R == 1)
		ACK = 1;//无应答信号
	else
		ACK = 0;//有应答信号
	//将时钟线拉低
	SCL = 0;
	delay_us(5);
	return ACK;
}

void IIC_Send_ACK(uint8_t ACK)
{
	//确保当前为输出模式
	SDA_Pin_Mode(GPIO_Mode_OUT);
	//时钟线拉低
	SCL = 0;
	delay_us(5);
	//将数据线设置为有/无应答
	if(ACK == 1)
		SDA_W = 1;//如果没有收到正确的信息，一定要确保应答位为高电平
	else
		SDA_W = 0;//低电平表示有应答
	//最后将时钟线拉高，让从机可以去读取应答信号
	SCL = 1;
	delay_us(5);
}

//发送数据的函数
void IIC_Send_Byte(uint8_t Byte)
{
	//确保当前为输出模式
	SDA_Pin_Mode(GPIO_Mode_OUT);
	//两条线都是从低电平开始
	SCL 	= 0;
	SDA_W 	= 0;
	delay_us(3);
	//将1字节数据按位传输,高位先出
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if((Byte&0x01<<(7-i)) != 0)
			SDA_W = 1;
		else
			SDA_W = 0;
		delay_us(4);//确保数据线已经传输完数据了
		SCL = 1;//时钟线拉高，让从机去读取
		delay_us(3);
		SCL = 0;//时钟线拉低，这样才能设置下一位数据
		delay_us(3);
	}
}

//接收1字节数据
uint8_t IIC_Recv_Byte(void)
{
	uint8_t data = 0x00;
	//确保当前为输入模式
	SDA_Pin_Mode(GPIO_Mode_IN);
	//时钟线拉低，让从机有时间设置数据位
	SCL = 0;
	//接收8位数据
	uint8_t i;
	for(i=0;i<8;i++)
	{
		delay_us(5);
		//时钟线拉高，开始读取数据
		SCL = 1;
		if(SDA_R == 1)
			data |= 0x01<<(7-i);
		//再回到低电平让从机去设置下一位数据
		delay_us(5);
		SCL = 0;
	}
	return data;
}

uint8_t AT24C02_Page_Write(uint8_t *data,uint8_t addr,uint8_t size)
{
	uint8_t ACK;
	//发送开始信号
	IIC_Start();
	//发送片选地址: 0xA0
	IIC_Send_Byte(0xA0);
	//接收一次应答信号
	ACK = IIC_RECV_ACK();
	if(ACK == 1)
	{
		printf("send device address error\n");
		return 1;
	}
	//发送要写入数据的地址
	IIC_Send_Byte(addr);
	//接收一次应答信号
	ACK = IIC_RECV_ACK();
	if(ACK == 1)
	{
		printf("send wrod address error\n");
		return 2;
	}
	//开始发送数据
	uint8_t i;
	for(i=0;i<size;i++)
	{
		//发送一个字节数据
		IIC_Send_Byte(data[i]);
		//接收一个应答信号
		ACK = IIC_RECV_ACK();
		if(ACK == 1)
		{
			printf("recv %d byte error\n",i);
			return 3;
		}
	}
	//发送停止信号
	IIC_Stop();
	return 0;
}

u8 AT24C02_Page_Read(u8 *buf,u8 addr)
{
	
	uint8_t ACK;
	SDA_Pin_Mode(GPIO_Mode_OUT);
	//发送开始信号
	IIC_Start();
	//发送片选地址: 0xA0
	IIC_Send_Byte(0xA0);
	//接收一次应答信号
	ACK = IIC_RECV_ACK();
	if(ACK == 1)
	{
		printf("send device address error\n");
		return 1;
	}
	IIC_Send_Byte(addr);
	ACK = IIC_RECV_ACK();
	if(ACK == 1)
	{
		printf("read byte error\n");
		return 2;
	}
	//发送d读取信号
	IIC_Start();
	IIC_Send_Byte(0xA1);
	ACK = IIC_RECV_ACK();
	if(ACK == 1)
	{
		printf("send read signal error\n");
		return 3;
	}
	*buf = IIC_Recv_Byte();
	IIC_Send_ACK(0);
	IIC_Send_ACK(1);
	IIC_Stop();
	return 0;
}




