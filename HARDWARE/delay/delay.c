#include "delay.h"
//寄存器的地址
#define  DWT_CR      (*(uint32_t *)0xE0001000)		//时间戳寄存器的控制寄存器
#define  DWT_CYCCNT  (*(uint32_t *)0xE0001004)		//时间戳寄存器的计数寄存器
#define  DEM_CR      (*(uint32_t *)0xE000EDFC)		//内核调试寄存器

#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

//1us有168 次脉冲
void delay_us(u32 nus)
{
	uint32_t Timecnt = 168 *nus; //计算nus时间内有多少个时间周期
	uint32_t Getcnt=0;
	OS_ERR err; 
	
	OSSchedLock(&err);//UCOSIII的方式,禁止调度，防止打断us延时---没有禁止所有中断
	
	 //使能DWT外设
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;                
    
	//DWT CYCCNT寄存器计数清0
	DWT_CYCCNT = (uint32_t)0u;
	
	//使能DWT CYCCNT寄存器
  	DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
	
	//每次读取计数器中的数值，直到计数器数值大于目标值
	do{
		Getcnt = OS_TS_GET();
	}while(Getcnt < Timecnt);
	
	//关闭DWT CYCCNT寄存器,关闭计数
	DWT_CR &= ~DWT_CR_CYCCNTENA;
	
	OSSchedUnlock(&err);//UCOSIII的方式,恢复调度
}

//1ms计数 168000次脉冲
void delay_ms(u32 nms)
{
	uint32_t Timecnt = 168000 *nms; //计算nms时间内有多少个时间周期
	uint32_t Getcnt=0;
	OS_ERR err; 
	
	OSSchedLock(&err);//UCOSIII的方式,禁止调度，防止打断us延时---没有禁止所有中断
	
	 //使能DWT外设
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;                
    
	//DWT CYCCNT寄存器计数清0
	DWT_CYCCNT = (uint32_t)0u;
	
	//使能DWT CYCCNT寄存器
  	DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
	
	//每次读取计数器中的数值，直到计数器数值大于目标值
	do{
		Getcnt = OS_TS_GET();
	}while(Getcnt < Timecnt);
	
	//关闭DWT CYCCNT寄存器,关闭计数
	DWT_CR &= ~DWT_CR_CYCCNTENA;
	
	OSSchedUnlock(&err);//UCOSIII的方式,恢复调度
	
}



