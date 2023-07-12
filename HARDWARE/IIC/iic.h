#ifndef _IIC_
#define	_IIC_
#define SCL		PBout(8)	//时钟线
#define SDA_W	PBout(9)	//发送时的数据线
#define SDA_R	PBin(9)		//接收时的数据线
#include "stm32f4xx.h"
#include "sys.h"
#include "newprintf.h"
#include "delay.h"
void IIC_GPIO_INIT(void);
void SDA_Pin_Mode(GPIOMode_TypeDef GPIO_Mode);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_RECV_ACK(void);
void IIC_Send_ACK(uint8_t ACK);
void IIC_Send_Byte(uint8_t Byte);
uint8_t IIC_Recv_Byte(void);
uint8_t AT24C02_Page_Write(uint8_t *data,uint8_t addr,uint8_t size);
u8 AT24C02_Page_Read(u8 *buf,u8 addr);



#endif

