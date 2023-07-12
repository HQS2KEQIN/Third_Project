#include  <includes.h>
#include "main.h"
/* 
	基于UCOS+stm32+阿里云物联网平台的家庭安全防控检测系统
	所需模块:esp8266-wifi模块、薄膜键盘、火焰传感器、4寸lcd显示屏、RFIO_RC522射频模块
	ID卡以及门禁卡
	室内温湿度上传至阿里云
	火焰检测
	密码监控
	蜂鸣器警报
	led显示输入的密码以及反馈
	刷卡(管理员才能使用)
	修改密码(管理员才能使用、必须刷管理员卡)
	添加新卡(管理员才能使用)
*/

//密码存放地址
#define PASSWD 0xf0
//卡号存储地址
#define Card_Addr 0x002f
u8 temp;
u8 humi;
u8 temp_p;
u8 humi_p;
u8 count_input_passwd = 3;
//id卡所有者类型:管理员 1  普通用户 2  未知卡 0 
u8 flag_passwd = 0;
u8 card_num = 0;
u8 card_buf[2];
u8 card_numberbuf[5];
u8 card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_readbuf[18];
//创建任务的任务声明
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  void  Task_Create(void  *arg);
//任务一及其相关函数声明
static  OS_TCB tcb1;
static  CPU_STK task1_stk[128];
static void dht11_send(void *arg);
//任务二及其相关函数声明
static  OS_TCB tcb2;
static  CPU_STK task2_stk[128];
static void keybroad_unlock(void *arg);
static void Modify_Passwd(void);
u8 RC522_Test(void);
static void ID_Card_Enter(void);
u8 input_passwd(void);
//任务三及其相关函数声明
static  OS_TCB tcb3;
static  CPU_STK task3_stk[128];
static void fire_detect(void *arg);

void Show_Menu(void);
void Show_Admin_Meun(void);

OS_FLAG_GRP pgrp;


//警报
void BEEP_ALARM(u8 n)
{
	u8 i = n;
	while(i)
	{
		PFout(8)^=1;
		delay_ms(350);
		i--;
	}
	PFout(8) = 0;
}

int main(void)
{
	//三大基本外设初始化:led灯、按键、蜂鸣器
	Led_Init();
	Key_Init();
	BEEP_Init();
	//串口1、2、3初始化
	USART1_INIT(115200);
	USART2_INIT(115200);
	USART3_INIT(9600);
	//薄膜键盘初始化
	Keybroad_Config_Init();
	//wifi初始化并连接阿里云物联网平台
	WIFI_INIT();
	//火焰传感器初始化
	Fire_Init();
	//温湿度传感器初始化
	dht11_init();
	//射频传感器初始化
	MFRC522_Initializtion();
	//oled屏初始化
	OLED_Init();
	delay_ms(500);
	OLED_Clear(); 
	
	
	OS_ERR err;
	//操作系统初始化，在最开始的位置
	CPU_Init();
	Mem_Init();
	Math_Init();
	
	OSInit(&err);
	//任务创建器的创建
    OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,	//任务结构体
                 (CPU_CHAR     *)"App Task Start",	//任务名称
                 (OS_TASK_PTR   )Task_Create,		//任务执行的函数
                 (void         *)0u,				//任务函数的参数
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,//任务优先级 = 2级
                 (CPU_STK      *)&AppTaskStartStk[0u],//堆栈空间首地址
                 (CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],//设置1/10的位置为堆栈限深
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,//堆栈的尺寸大小---单位是 unsigned int   字
                 (OS_MSG_QTY    )0u,//消息队列个数
                 (OS_TICK       )0u,//时间片个数
                 (void         *)0u,//堆栈补充区指针大小
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//堆栈可以被检查，并且清空一次
                 (OS_ERR       *)&err);//如果创建时出错，把错误保存到这个错误管理结构体中

    OSStart(&err);//开始运行UCOSIII系统、只初始化一次

	(void)&err;

    return (0u);
}

//创建任务的任务
void Task_Create(void *arg)
{
	OS_ERR err;
	//
	BSP_Tick_Init();
	OSStatTaskCPUUsageInit(&err);
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
	OSFlagCreate(&pgrp,"pgrp",0,&err);
	
	OSTaskCreate(&tcb1,
			   "send dht11_data",
				 dht11_send,
					 0,
	                 5,
			 task1_stk,
				128/10,
				   128,
					 0,
					 0,
					 0,
	  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
				  &err
	);
	
	OSTaskCreate(&tcb2,
			   "keybroad unlock",
				 keybroad_unlock,
					 0,
	                 7,
			 task2_stk,
				128/10,
				   128,
					 0,
					 0,
					 0,
	  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
				  &err
	);
	
	OSTaskCreate(&tcb3,
			   "fire detect",
				 fire_detect,
					 0,
	                 7,
			 task3_stk,
				128/10,
				   128,
					 0,
					 0,
					 0,
	  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
				  &err
	);
	//创建完所有任务后将任务创建器删除
	OSTaskDel(&AppTaskStartTCB,&err);

}


//任务1:上传室内温湿度
void dht11_send(void *arg)
{
	OS_ERR err;
	uint8_t buf[5]={0}; //温湿度变量设置
	while(1)
	{
		//读取温湿度
		dht11_read_data(buf);
		//温度整数部分
		temp = buf[2];
		//湿度整数部分
		humi = buf[0];
		//温度小数部分
		temp_p = buf[3];
		//湿度小数部分
		humi_p = buf[4];
		printf("温度:%d.%d,湿度:%d.%d\n",temp,temp_p,humi,humi_p);
		delay_ms(500);
		//上传温湿度到阿里云平台
		Send_DHT();
		PFout(9) ^= 1;
		//任务进入休眠状态
		OSTimeDlyHMSM(0,0,10,0,OS_OPT_TIME_DLY,&err);
	}
	
}

//任务2:门锁监控
void keybroad_unlock(void *arg)
{
	OS_ERR err;
	delay_ms(500);
	Show_Menu();
	//开始门为关闭状态
	USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":0}}\",0,0\r\n"); 
	delay_ms(500);
	//开始管理员不在线
	USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"Admin_Status\\\":0}}\",0,0\r\n"); 
	
	while(1)
	{
		//键盘解锁
		if(!KEY2)
		{
			delay_ms(20);
			u8 ret = input_passwd();
			//挂起任务一，以免AT指令发送冲突
			OSTaskSuspend(&tcb1,&err);
			if(ret == 1)
			{		
				//发送已开锁的状态指令
				USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":1}}\",0,0\r\n"); 
				//等待五秒门自动上锁
				delay_ms(5000);
			}
			USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":0}}\",0,0\r\n"); 
			delay_ms(500);
			//唤醒任务一，使其继续上传温湿度信息
			OSTaskResume(&tcb1,&err);
			Show_Menu();
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err);
		}
		//刷卡解锁
		else if(!KEY3)
		{
			delay_ms(20);
			OLED_Clear();
			OLED_ShowString(0,3,"Please Swipe!",16);
			//进行ID卡监控,除非寻到卡，否则一直循环
			while(!RC522_Test());
			MFRC522_AT24CXX_Read(Card_Addr, &card_num, 1);
			//寻到管理员的卡
			if(flag_passwd == 1)
			{
				Show_Admin_Meun();
				while(KEY1)
				{
					//开锁进门
					if(!KEY0)
					{
						//挂起任务一，以免AT指令发送冲突
						OSTaskSuspend(&tcb1,&err);
						USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":1}}\",0,0\r\n"); 
						delay_ms(5000);
						USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":0}}\",0,0\r\n"); 
						delay_ms(500);
						//唤醒任务一，使其继续上传温湿度信息
						OSTaskResume(&tcb1,&err);
						delay_ms(500);
						break;
					}
					
					//修改密码
					if(!KEY2)
					{
						delay_ms(20);
						Modify_Passwd();
						Show_Admin_Meun();		
					}
					//录入普通用户卡信息
					if(!KEY3)
					{
						ID_Card_Enter();
						MFRC522_Initializtion();
						Show_Admin_Meun();
					}
				}	
				//按下key1后会退出管理员
				OSTaskSuspend(&tcb1,&err);			
				USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"Admin_Status\\\":0}}\",0,0\r\n");
				delay_ms(1000);
				OSTaskResume(&tcb1,&err);
				flag_passwd = 0;
			}
			//寻到普通用户的卡
			else if(flag_passwd == 2)
			{
				//挂起任务一，以免AT指令发送冲突
				OSTaskSuspend(&tcb1,&err);
				//发送已开锁的状态指令
				USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":1}}\",0,0\r\n"); 
				//等待五秒门自动上锁
				delay_ms(5000);
				USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":0}}\",0,0\r\n"); 
				delay_ms(500);
				//唤醒任务一，使其继续上传温湿度信息
				OSTaskResume(&tcb1,&err);
				flag_passwd = 0;
			
			}
			delay_ms(1000);
			Show_Menu();
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err);
		}
	}
}

//主界面显示
void Show_Menu(void)
{
	OLED_Clear();
	/*   
		=================
		   智 能 门 锁
		1.键 盘 解 锁
		2.刷 卡 解 锁
	*/ 
	OLED_ShowString(0,0,"================",16);
	OLED_ShowCHinese(23,2,1);
	OLED_ShowCHinese(48,2,2);
	OLED_ShowCHinese(73,2,3);
	OLED_ShowCHinese(98,2,4);
	OLED_ShowString(0,4,"1.",16);
	OLED_ShowCHinese(18,4,5);
	OLED_ShowCHinese(43,4,6);
	OLED_ShowCHinese(68,4,7);
	OLED_ShowCHinese(93,4,8);
	OLED_ShowString(0,6,"2.",16);
	OLED_ShowCHinese(18,6,9);
	OLED_ShowCHinese(43,6,10);
	OLED_ShowCHinese(68,6,11);
	OLED_ShowCHinese(93,6,12);
	
}

//管理员界面显示
void Show_Admin_Meun(void)
{
	OLED_Clear();
	OLED_ShowString(15,1,"Welcome Admin!",16);
	OLED_ShowString(0,3,"0.Enter",8);
	OLED_ShowString(0,4,"1.Exit",8);
	OLED_ShowString(0,5,"2.modify passwd",8);
	OLED_ShowString(0,6,"3.Enter Card",8);
}

//密码输入
u8 input_passwd(void)
{
	
	u8 passwd_door[6] = {0};
	u8 key_in;
	short i = 0;
	char ch = 0;
	OLED_Clear();
	OLED_ShowString(0,0,"================",16);
	OLED_ShowCHinese(23,2,1);
	OLED_ShowCHinese(48,2,2);
	OLED_ShowCHinese(73,2,3);
	OLED_ShowCHinese(98,2,4);
	
	OLED_ShowCHinese(0,5,37);
	OLED_ShowCHinese(13,5,33);
	OLED_ShowCHinese(26,5,34);
	OLED_ShowCHinese(39,5,17);	
	OLED_ShowCHinese(52,5,18);
	OLED_ShowString(65,5,":",16);
	for(;i<6;i++)
	{
		
		key_in = Keybroad_Input(0);
		
		if(key_in!=30)
			printf("key_in = %d\n",key_in);
			
		if(key_in<10)	
		{
			ch = key_in+48;
			passwd_door[i] = key_in;
			OLED_ShowChar(75+i*8,5,ch,16);
		}
		else
			i--;
	}
	u8 passwd[6];
	for(i = 0;i<6;i++)
		AT24C02_Page_Read(&passwd[i],PASSWD+i);

	OLED_Clear();
	OLED_ShowString(12,3,"Unlocking....!",16);
	delay_ms(1500);
	OLED_Clear();
	if(passwd_door[0]==passwd[0]&&passwd_door[1]==passwd[1]&&
	   passwd_door[2]==passwd[2]&&passwd_door[3]==passwd[3]&&
	   passwd_door[4]==passwd[4]&&passwd_door[5]==passwd[5])
	{
		OLED_ShowCHinese(26,3,17);
		OLED_ShowCHinese(51,3,18);
		OLED_ShowCHinese(76,3,19);	
		OLED_ShowCHinese(101,3,20);
		OLED_ShowString(114,3,"!",16);
		count_input_passwd = 3;
		delay_ms(2000);
		return 1;
	}
	else
	{
		count_input_passwd--;
		if(!count_input_passwd)
		{
			OLED_ShowCHinese(0,0,40);
			OLED_ShowCHinese(15,0,41);
			OLED_ShowString(30,0,"!",16);	
			OLED_ShowCHinese(45,0,40);
			OLED_ShowCHinese(60,0,41);
			OLED_ShowString(75,0,"!",16);	
			OLED_ShowCHinese(90,0,40);
			OLED_ShowCHinese(105,0,41);
			OLED_ShowString(120,0,"!",16);			
			OLED_ShowCHinese(0,2,43);
			OLED_ShowCHinese(20,2,44);
			OLED_ShowCHinese(40,2,45);
			OLED_ShowCHinese(60,2,46);
			OLED_ShowCHinese(80,2,47);
			OLED_ShowCHinese(100,2,48);
			OLED_ShowCHinese(0,4,43);
			OLED_ShowCHinese(20,4,44);
			OLED_ShowCHinese(40,4,45);
			OLED_ShowCHinese(60,4,46);
			OLED_ShowCHinese(80,4,47);
			OLED_ShowCHinese(100,4,48);
			OLED_ShowCHinese(0,6,43);
			OLED_ShowCHinese(20,6,44);
			OLED_ShowCHinese(40,6,45);
			OLED_ShowCHinese(60,6,46);
			OLED_ShowCHinese(80,6,47);
			OLED_ShowCHinese(100,6,48);
			count_input_passwd = 3;	
			BEEP_ALARM(10);
			return 0;
		
		}
		OLED_ShowCHinese(26,3,17);
		OLED_ShowCHinese(51,3,18);
		OLED_ShowCHinese(76,3,38);	
		OLED_ShowCHinese(101,3,39);
		OLED_ShowString(114,3,"!",16);
		
		OLED_ShowCHinese(0,5,30);
		OLED_ShowCHinese(15,5,31);
		ch = count_input_passwd+48;
		OLED_ShowChar(30,5,ch,16);
		OLED_ShowCHinese(45,5,32);
		OLED_ShowCHinese(60,5,33);
		OLED_ShowCHinese(75,5,34);
		OLED_ShowCHinese(90,5,35);
		OLED_ShowCHinese(105,5,36);
		
		BEEP_ALARM(6);
		return 0;
	}
}

//判断id卡是否为管理员或者普通用户的，其他卡会报警
u8 RC522_Test(void)
{
	u8 i,j,status,card_size;
	
	OS_ERR err;
	card_buf[0]=0;
	card_buf[1]=0;
	status = MFRC522_Request(0x52,card_buf);
	u8 card_numberbuf_s[5];
	if(status==MI_OK)
	{
		status    = MFRC522_Anticoll(card_numberbuf);			//防撞处理			
		card_size = MFRC522_SelectTag(card_numberbuf);	        //选卡
		status    = MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		status   =  MFRC522_Read(4, card_readbuf);					//读卡	
		printf("卡的类型：%#x %#x",card_buf[0],card_buf[1]);

		//卡序列号显，最后一字节为卡的校验码
		printf("卡的序列号：");
		for(i=0;i<5;i++)
		{
			printf("%d ",card_numberbuf[i]);
		}
		printf("\n");
		OLED_Clear();
		OLED_ShowString(0,3,"Identifying....!",16);
		delay_ms(1500);
		if(card_numberbuf[0]==19&&card_numberbuf[1]==121&&card_numberbuf[2]==101&&
			card_numberbuf[3]==146&&card_numberbuf[4]==157)
		{
			OLED_Clear();
			OLED_ShowString(0,3,"Unlock Success!",16);
			PFout(8) = 1;
			delay_ms(100);
			PFout(8) = 0;
			OSTaskSuspend(&tcb1,&err);			
			USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"Admin_Status\\\":1}}\",0,0\r\n");
			delay_ms(500);
			OSTaskResume(&tcb1,&err);
			flag_passwd = 1;
//			MFRC522_Reset();
			MFRC522_Initializtion();
			memset(card_numberbuf, 0, sizeof(card_numberbuf));
			return 1;
		}
		delay_ms(20);
		u8 card_n;
		MFRC522_AT24CXX_Read(Card_Addr, &card_n,1);
		printf("card_n = %d\n",card_n);
		for(j=0;j<card_n;j++)
		{
			MFRC522_AT24CXX_Read(Card_Addr+1+5*j, card_numberbuf_s,5); // 读
			printf("card_numberbuf_s:%d %d %d %d %d\n",card_numberbuf_s[0],
					card_numberbuf_s[1],card_numberbuf_s[2],
					card_numberbuf_s[3],card_numberbuf_s[4]);
			if(card_numberbuf[0]==card_numberbuf_s[0]&&
			   card_numberbuf[1]==card_numberbuf_s[1]&&
			   card_numberbuf[2]==card_numberbuf_s[2]&&
			   card_numberbuf[3]==card_numberbuf_s[3]&&
			   card_numberbuf[4]==card_numberbuf_s[4])
			{
				OLED_Clear();
				OLED_ShowString(0,3,"Unlock Success!",16);
				PFout(8) = 1;
				delay_ms(100);
				PFout(8) = 0;

				MFRC522_Initializtion();
				memset(card_numberbuf, 0, sizeof(card_numberbuf));
				flag_passwd = 2;
				return 1;
			}
			delay_ms(100);
		}
		OLED_Clear();
		OLED_ShowString(0,3,"Unknown Card!",16);
//		BEEP_ALARM(4);
		PEout(13) = 0;
		delay_ms(1500);
		PEout(13) = 1;

		MFRC522_Initializtion();
		memset(card_numberbuf, 0, sizeof(card_numberbuf));
		return 2;
	}
	return 0;

}



//密码修改
void Modify_Passwd(void)
{
	u8 passwd_door[6] = {0};
	u8 key_in;
	short i = 0;
	char ch = 0;
	OLED_Clear();
	OLED_ShowString(0,0,"================",16);
	OLED_ShowCHinese(23,2,1);
	OLED_ShowCHinese(48,2,2);
	OLED_ShowCHinese(73,2,3);
	OLED_ShowCHinese(98,2,4);
	
	OLED_ShowCHinese(0,5,33);
	OLED_ShowCHinese(13,5,34);
	OLED_ShowCHinese(26,5,49);
	OLED_ShowCHinese(39,5,17);	
	OLED_ShowCHinese(52,5,18);
	OLED_ShowString(65,5,":",16);
	for(;i<6;i++)
	{
		key_in = Keybroad_Input(0);
		
		if(key_in!=30)
			printf("key_in = %d\n",key_in);
			
		if(key_in<10)	
		{
			ch = key_in+48;
			passwd_door[i] = key_in;
			
			OLED_ShowChar(75+i*8,5,ch,16);
		}
		else
			i--;
	}
	AT24C02_Page_Write(passwd_door,PASSWD,6);
	OLED_ShowString(30,6,"Modify OK!",16);
	delay_ms(1500);

}
	




//录入新卡
void ID_Card_Enter(void)
{
	u8 i,status,card_size;
	u8  card_numberbuf_s[5];     					 // 卡号

	OLED_Clear();
	OLED_ShowString(0,0,"Enter User",16);
	OLED_ShowString(0,2,"Please Swipe",16);
	OLED_ShowString(0,4,"Key0:Exit",16);
	card_buf[0] = 0;
	card_buf[1] = 0;

	while(1)
	{	
		status = MFRC522_Request(0x52,card_buf);			//寻卡
		if(status == MI_OK)
		{
			status    = MFRC522_Anticoll(card_numberbuf);			            // 防撞处理			
			card_size = MFRC522_SelectTag(card_numberbuf);	                // 选卡
			status    = MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	// 验卡
			status    =  MFRC522_Read(4, card_readbuf);					    // 读卡
			OLED_Clear(); // 清除(0,12)~(128,64)
			break;
		}
		//不读了、走人
		if(!KEY0)
		{
			OLED_Clear();
			return;
		}
	}
	if(status == MI_OK)		//如果读到卡
	{


		//卡容量显示，单位为Kbits
		OLED_Clear();
		OLED_ShowString(0,0,"Register User",16);
		OLED_ShowString(0,2,"ID:",8);   // 在OLED 屏幕上显示卡号
		printf("新卡号:");
		for(i = 0; i<5; i++)
		{
			printf("%d ",card_numberbuf[i]);
			OLED_ShowNum(16+i*24, 2,card_numberbuf[i],3,8);    
		}
		printf("\n");
//		card_num = 0;
		delay_ms(1500);
		MFRC522_AT24CXX_Write(Card_Addr+1+5*card_num, card_numberbuf,5);    // 写
		delay_ms(1000);
		MFRC522_AT24CXX_Read(Card_Addr+1+5*card_num, card_numberbuf_s,5); // 读
		card_num++;
		MFRC522_AT24CXX_Write(Card_Addr, &card_num,1);    // 写
		delay_ms(500);
		u8 card_n;
		MFRC522_AT24CXX_Read(Card_Addr, &card_n,1); // 读
		printf("card_n = %d\n",card_n);
		if(memcmp(card_numberbuf,card_numberbuf_s,5) != 0)
		{
			OLED_ShowString(20, 3, "Register Fail!", 16);
			delay_ms(500);
			memcpy(card_numberbuf, "0", 5);   // 清除card_numberbuf数组的值
			OLED_Clear();
			return;
		} 
		OLED_ShowString(20, 3, "Register Success!", 16);
		delay_ms(600);
		memset(card_numberbuf, 0, sizeof(card_numberbuf));  // 清除card_numberbuf数组的值
		OLED_Clear();
	}	
}	

//任务3:火焰检测
void fire_detect(void *arg)
{
	OS_ERR err;
	while(1)
	{
		Fire_Detect();
		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_DLY,&err);
	
	}

}




