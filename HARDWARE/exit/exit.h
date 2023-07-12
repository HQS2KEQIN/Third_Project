#ifndef EXIT_H
#define EXIT_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "oled.h"

void exit0_gpio_init(uint8_t PreemptionPriority,uint8_t SubPriority);
void exit2_gpio_init(uint8_t PreemptionPriority,uint8_t SubPriority);
void exit3_gpio_init(uint8_t PreemptionPriority,uint8_t SubPriority);
void exit4_gpio_init(uint8_t PreemptionPriority,uint8_t SubPriority);



#endif


