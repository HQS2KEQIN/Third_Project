#ifndef __DHT11_
#define __DHT11_

#include "stm32f4xx.h" //��������ͷ�ļ���Ϣ


void dht11_init(void);
void dht11_pin_mode(u8 i);
int32_t dht11_read_data(uint8_t *pbuf);


#endif
