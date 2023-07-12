#ifndef _WIFI_
#define	_WIFI_
#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
void WIFI_INIT(void);
void Send_DHT(void);
void USART2_sendstr(char *str);

extern  u8 temp;
extern  u8 humi;
extern  u8 temp_p;
extern  u8 humi_p;
extern volatile u8 Switch_1;
extern volatile u8 Switch_2;
#endif

