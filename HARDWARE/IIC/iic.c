#include "iic.h"


void IIC_GPIO_INIT(void)
{
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//����GPIOB8��9Ϊ���
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
	//ȷ����ǰΪ���ģʽ
	SDA_Pin_Mode(GPIO_Mode_OUT);
	//ȷ��ʱ���ߺ�������һ��ʼΪ�ߵ�ƽ
	SCL 	= 1;
	SDA_W 	= 1;
	//�ȳ���һ��ʱ��ߵ�ƽ���޸�������
	delay_us(5);
	SDA_W 	= 0;
	//����ʱ����ҲҪ����
	delay_us(5);
	SCL 	= 0;//һ�����ں��ٽ�ʱ��������
	delay_us(5);//ǯסIIC����
}

void IIC_Stop(void)
{
	SDA_Pin_Mode(GPIO_Mode_OUT);
	SCL 	= 1;//ȷ��ʱ����Ϊ�ߵ�ƽ
	SDA_W 	= 0;//������Ϊ�͵�ƽ
	delay_us(5);
	SDA_W	= 1;//�ٽ�����������
	delay_us(5);


}
//����Ӧ���źŵĺ���
uint8_t IIC_RECV_ACK(void)
{
	//ȷ����ǰΪ����ģʽ
	SDA_Pin_Mode(GPIO_Mode_IN);
	//�Ƚ�ʱ��������
	SCL	= 1;
	delay_us(5);
	uint8_t ACK = 0;
	if(SDA_R == 1)
		ACK = 1;//��Ӧ���ź�
	else
		ACK = 0;//��Ӧ���ź�
	//��ʱ��������
	SCL = 0;
	delay_us(5);
	return ACK;
}

void IIC_Send_ACK(uint8_t ACK)
{
	//ȷ����ǰΪ���ģʽ
	SDA_Pin_Mode(GPIO_Mode_OUT);
	//ʱ��������
	SCL = 0;
	delay_us(5);
	//������������Ϊ��/��Ӧ��
	if(ACK == 1)
		SDA_W = 1;//���û���յ���ȷ����Ϣ��һ��Ҫȷ��Ӧ��λΪ�ߵ�ƽ
	else
		SDA_W = 0;//�͵�ƽ��ʾ��Ӧ��
	//���ʱ�������ߣ��ôӻ�����ȥ��ȡӦ���ź�
	SCL = 1;
	delay_us(5);
}

//�������ݵĺ���
void IIC_Send_Byte(uint8_t Byte)
{
	//ȷ����ǰΪ���ģʽ
	SDA_Pin_Mode(GPIO_Mode_OUT);
	//�����߶��Ǵӵ͵�ƽ��ʼ
	SCL 	= 0;
	SDA_W 	= 0;
	delay_us(3);
	//��1�ֽ����ݰ�λ����,��λ�ȳ�
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if((Byte&0x01<<(7-i)) != 0)
			SDA_W = 1;
		else
			SDA_W = 0;
		delay_us(4);//ȷ���������Ѿ�������������
		SCL = 1;//ʱ�������ߣ��ôӻ�ȥ��ȡ
		delay_us(3);
		SCL = 0;//ʱ�������ͣ���������������һλ����
		delay_us(3);
	}
}

//����1�ֽ�����
uint8_t IIC_Recv_Byte(void)
{
	uint8_t data = 0x00;
	//ȷ����ǰΪ����ģʽ
	SDA_Pin_Mode(GPIO_Mode_IN);
	//ʱ�������ͣ��ôӻ���ʱ����������λ
	SCL = 0;
	//����8λ����
	uint8_t i;
	for(i=0;i<8;i++)
	{
		delay_us(5);
		//ʱ�������ߣ���ʼ��ȡ����
		SCL = 1;
		if(SDA_R == 1)
			data |= 0x01<<(7-i);
		//�ٻص��͵�ƽ�ôӻ�ȥ������һλ����
		delay_us(5);
		SCL = 0;
	}
	return data;
}

uint8_t AT24C02_Page_Write(uint8_t *data,uint8_t addr,uint8_t size)
{
	uint8_t ACK;
	//���Ϳ�ʼ�ź�
	IIC_Start();
	//����Ƭѡ��ַ: 0xA0
	IIC_Send_Byte(0xA0);
	//����һ��Ӧ���ź�
	ACK = IIC_RECV_ACK();
	if(ACK == 1)
	{
		printf("send device address error\n");
		return 1;
	}
	//����Ҫд�����ݵĵ�ַ
	IIC_Send_Byte(addr);
	//����һ��Ӧ���ź�
	ACK = IIC_RECV_ACK();
	if(ACK == 1)
	{
		printf("send wrod address error\n");
		return 2;
	}
	//��ʼ��������
	uint8_t i;
	for(i=0;i<size;i++)
	{
		//����һ���ֽ�����
		IIC_Send_Byte(data[i]);
		//����һ��Ӧ���ź�
		ACK = IIC_RECV_ACK();
		if(ACK == 1)
		{
			printf("recv %d byte error\n",i);
			return 3;
		}
	}
	//����ֹͣ�ź�
	IIC_Stop();
	return 0;
}

u8 AT24C02_Page_Read(u8 *buf,u8 addr)
{
	
	uint8_t ACK;
	SDA_Pin_Mode(GPIO_Mode_OUT);
	//���Ϳ�ʼ�ź�
	IIC_Start();
	//����Ƭѡ��ַ: 0xA0
	IIC_Send_Byte(0xA0);
	//����һ��Ӧ���ź�
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
	//����d��ȡ�ź�
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




