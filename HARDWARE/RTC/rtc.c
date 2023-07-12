#include "stm32f4xx.h"
#include "sys.h"
#include "newprintf.h"
#define BAK 300
void RTC_INIT(void)
{
	//使能RTC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	//开启用户访问寄存器权限
	PWR_BackupAccessCmd(ENABLE);
	//配置LSE时钟源:外部低速时钟
//	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	//开启LSE
	RCC_LSEConfig(RCC_LSE_ON);
	//使能时钟
	RCC_RTCCLKCmd(ENABLE);
	//RTC初始化
	RTC_InitTypeDef RTCinit;
	RTCinit.RTC_AsynchPrediv 	= 127;
	RTCinit.RTC_HourFormat		= RTC_HourFormat_24;
	RTCinit.RTC_SynchPrediv 	= 255;
	RTC_Init(&RTCinit);
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=BAK)
	{
		//日历当前日期初始化
		RTC_DateTypeDef Dateinit;
		Dateinit.RTC_Date		= 0x30;
		Dateinit.RTC_Month		= RTC_Month_June;
		Dateinit.RTC_WeekDay	= 0x03;
		Dateinit.RTC_Year		= 0x23;
		RTC_SetDate(RTC_Format_BCD,&Dateinit);
		//日历当前时间初始化
		RTC_TimeTypeDef Timeinit;
		Timeinit.RTC_H12		= RTC_H12_PM;
		Timeinit.RTC_Hours		= 0x19;
		Timeinit.RTC_Minutes	= 0x39;
		Timeinit.RTC_Seconds	= 0x0;
		RTC_SetTime(RTC_Format_BCD,&Timeinit);
		RTC_WriteBackupRegister(RTC_BKP_DR0,BAK);
	}
}

void ALARM_INIT(void)
{
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	RTC_AlarmCmd(RTC_Alarm_B,DISABLE);
	RTC_AlarmTypeDef AlarminitA;
	RTC_TimeTypeDef TimeinitA;
	TimeinitA.RTC_H12		= RTC_H12_AM;
	TimeinitA.RTC_Hours		= 0x9;
	TimeinitA.RTC_Minutes	= 0x30;
	TimeinitA.RTC_Seconds	= 0x0;
	AlarminitA.RTC_AlarmTime				= TimeinitA;
	AlarminitA.RTC_AlarmMask				= RTC_AlarmMask_DateWeekDay;
	AlarminitA.RTC_AlarmDateWeekDay		= 0x04;
	AlarminitA.RTC_AlarmDateWeekDaySel	= RTC_AlarmDateWeekDaySel_WeekDay;
	
	RTC_AlarmTypeDef AlarminitB;
	RTC_TimeTypeDef TimeinitB;
	TimeinitB.RTC_H12		= RTC_H12_PM;
	TimeinitB.RTC_Hours		= 0x9;
	TimeinitB.RTC_Minutes	= 0x30;
	TimeinitB.RTC_Seconds	= 0x05;
	AlarminitA.RTC_AlarmTime				= TimeinitB;
	AlarminitA.RTC_AlarmMask				= RTC_AlarmMask_DateWeekDay;
	AlarminitA.RTC_AlarmDateWeekDay		= 0x04;
	AlarminitA.RTC_AlarmDateWeekDaySel	= RTC_AlarmDateWeekDaySel_WeekDay;
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&AlarminitA);
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_B,&AlarminitB);
	
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_ITConfig(RTC_IT_ALRB,ENABLE);
	EXTI_InitTypeDef EXTIinit;
	EXTIinit.EXTI_Line			= EXTI_Line17;
	EXTIinit.EXTI_LineCmd		= ENABLE;
	EXTIinit.EXTI_Mode			= EXTI_Mode_Interrupt;
	EXTIinit.EXTI_Trigger		= EXTI_Trigger_Rising;
	EXTI_Init(&EXTIinit);
	
	
	NVIC_InitTypeDef NVICinit;
	NVICinit.NVIC_IRQChannel					= RTC_Alarm_IRQn;
	NVICinit.NVIC_IRQChannelCmd					= ENABLE;
	NVICinit.NVIC_IRQChannelPreemptionPriority  = 0;
	NVICinit.NVIC_IRQChannelSubPriority			= 0;
	NVIC_Init(&NVICinit);
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_ITConfig(RTC_IT_ALRB,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_B,ENABLE);
}

void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) == SET)
	{
		printf("闹铃响了\n");
		PFout(8) = 1;
	}
	if(RTC_GetITStatus(RTC_IT_ALRB) == SET)
	{
		printf("闹铃关了\n");
		PFout(8) = 0;
	}
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	EXTI_ClearITPendingBit(EXTI_Line17);
}




