#include  <includes.h>
#include "main.h"
/* 
	����UCOS+stm32+������������ƽ̨�ļ�ͥ��ȫ���ؼ��ϵͳ
	����ģ��:esp8266-wifiģ�顢��Ĥ���̡����洫������4��lcd��ʾ����RFIO_RC522��Ƶģ��
	ID���Լ��Ž���
	������ʪ���ϴ���������
	������
	������
	����������
	led��ʾ����������Լ�����
	ˢ��(����Ա����ʹ��)
	�޸�����(����Ա����ʹ�á�����ˢ����Ա��)
	����¿�(����Ա����ʹ��)
*/

//�����ŵ�ַ
#define PASSWD 0xf0
//���Ŵ洢��ַ
#define Card_Addr 0x002f
u8 temp;
u8 humi;
u8 temp_p;
u8 humi_p;
u8 count_input_passwd = 3;
//id������������:����Ա 1  ��ͨ�û� 2  δ֪�� 0 
u8 flag_passwd = 0;
u8 card_num = 0;
u8 card_buf[2];
u8 card_numberbuf[5];
u8 card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_readbuf[18];
//�����������������
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  void  Task_Create(void  *arg);
//����һ������غ�������
static  OS_TCB tcb1;
static  CPU_STK task1_stk[128];
static void dht11_send(void *arg);
//�����������غ�������
static  OS_TCB tcb2;
static  CPU_STK task2_stk[128];
static void keybroad_unlock(void *arg);
static void Modify_Passwd(void);
u8 RC522_Test(void);
static void ID_Card_Enter(void);
u8 input_passwd(void);
//������������غ�������
static  OS_TCB tcb3;
static  CPU_STK task3_stk[128];
static void fire_detect(void *arg);

void Show_Menu(void);
void Show_Admin_Meun(void);

OS_FLAG_GRP pgrp;


//����
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
	//������������ʼ��:led�ơ�������������
	Led_Init();
	Key_Init();
	BEEP_Init();
	//����1��2��3��ʼ��
	USART1_INIT(115200);
	USART2_INIT(115200);
	USART3_INIT(9600);
	//��Ĥ���̳�ʼ��
	Keybroad_Config_Init();
	//wifi��ʼ�������Ӱ�����������ƽ̨
	WIFI_INIT();
	//���洫������ʼ��
	Fire_Init();
	//��ʪ�ȴ�������ʼ��
	dht11_init();
	//��Ƶ��������ʼ��
	MFRC522_Initializtion();
	//oled����ʼ��
	OLED_Init();
	delay_ms(500);
	OLED_Clear(); 
	
	
	OS_ERR err;
	//����ϵͳ��ʼ�������ʼ��λ��
	CPU_Init();
	Mem_Init();
	Math_Init();
	
	OSInit(&err);
	//���񴴽����Ĵ���
    OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,	//����ṹ��
                 (CPU_CHAR     *)"App Task Start",	//��������
                 (OS_TASK_PTR   )Task_Create,		//����ִ�еĺ���
                 (void         *)0u,				//�������Ĳ���
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,//�������ȼ� = 2��
                 (CPU_STK      *)&AppTaskStartStk[0u],//��ջ�ռ��׵�ַ
                 (CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],//����1/10��λ��Ϊ��ջ����
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,//��ջ�ĳߴ��С---��λ�� unsigned int   ��
                 (OS_MSG_QTY    )0u,//��Ϣ���и���
                 (OS_TICK       )0u,//ʱ��Ƭ����
                 (void         *)0u,//��ջ������ָ���С
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//��ջ���Ա���飬�������һ��
                 (OS_ERR       *)&err);//�������ʱ�����Ѵ��󱣴浽����������ṹ����

    OSStart(&err);//��ʼ����UCOSIIIϵͳ��ֻ��ʼ��һ��

	(void)&err;

    return (0u);
}

//�������������
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
	//������������������񴴽���ɾ��
	OSTaskDel(&AppTaskStartTCB,&err);

}


//����1:�ϴ�������ʪ��
void dht11_send(void *arg)
{
	OS_ERR err;
	uint8_t buf[5]={0}; //��ʪ�ȱ�������
	while(1)
	{
		//��ȡ��ʪ��
		dht11_read_data(buf);
		//�¶���������
		temp = buf[2];
		//ʪ����������
		humi = buf[0];
		//�¶�С������
		temp_p = buf[3];
		//ʪ��С������
		humi_p = buf[4];
		printf("�¶�:%d.%d,ʪ��:%d.%d\n",temp,temp_p,humi,humi_p);
		delay_ms(500);
		//�ϴ���ʪ�ȵ�������ƽ̨
		Send_DHT();
		PFout(9) ^= 1;
		//�����������״̬
		OSTimeDlyHMSM(0,0,10,0,OS_OPT_TIME_DLY,&err);
	}
	
}

//����2:�������
void keybroad_unlock(void *arg)
{
	OS_ERR err;
	delay_ms(500);
	Show_Menu();
	//��ʼ��Ϊ�ر�״̬
	USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":0}}\",0,0\r\n"); 
	delay_ms(500);
	//��ʼ����Ա������
	USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"Admin_Status\\\":0}}\",0,0\r\n"); 
	
	while(1)
	{
		//���̽���
		if(!KEY2)
		{
			delay_ms(20);
			u8 ret = input_passwd();
			//��������һ������ATָ��ͳ�ͻ
			OSTaskSuspend(&tcb1,&err);
			if(ret == 1)
			{		
				//�����ѿ�����״ָ̬��
				USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":1}}\",0,0\r\n"); 
				//�ȴ��������Զ�����
				delay_ms(5000);
			}
			USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":0}}\",0,0\r\n"); 
			delay_ms(500);
			//��������һ��ʹ������ϴ���ʪ����Ϣ
			OSTaskResume(&tcb1,&err);
			Show_Menu();
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err);
		}
		//ˢ������
		else if(!KEY3)
		{
			delay_ms(20);
			OLED_Clear();
			OLED_ShowString(0,3,"Please Swipe!",16);
			//����ID�����,����Ѱ����������һֱѭ��
			while(!RC522_Test());
			MFRC522_AT24CXX_Read(Card_Addr, &card_num, 1);
			//Ѱ������Ա�Ŀ�
			if(flag_passwd == 1)
			{
				Show_Admin_Meun();
				while(KEY1)
				{
					//��������
					if(!KEY0)
					{
						//��������һ������ATָ��ͳ�ͻ
						OSTaskSuspend(&tcb1,&err);
						USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":1}}\",0,0\r\n"); 
						delay_ms(5000);
						USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":0}}\",0,0\r\n"); 
						delay_ms(500);
						//��������һ��ʹ������ϴ���ʪ����Ϣ
						OSTaskResume(&tcb1,&err);
						delay_ms(500);
						break;
					}
					
					//�޸�����
					if(!KEY2)
					{
						delay_ms(20);
						Modify_Passwd();
						Show_Admin_Meun();		
					}
					//¼����ͨ�û�����Ϣ
					if(!KEY3)
					{
						ID_Card_Enter();
						MFRC522_Initializtion();
						Show_Admin_Meun();
					}
				}	
				//����key1����˳�����Ա
				OSTaskSuspend(&tcb1,&err);			
				USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"Admin_Status\\\":0}}\",0,0\r\n");
				delay_ms(1000);
				OSTaskResume(&tcb1,&err);
				flag_passwd = 0;
			}
			//Ѱ����ͨ�û��Ŀ�
			else if(flag_passwd == 2)
			{
				//��������һ������ATָ��ͳ�ͻ
				OSTaskSuspend(&tcb1,&err);
				//�����ѿ�����״ָ̬��
				USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":1}}\",0,0\r\n"); 
				//�ȴ��������Զ�����
				delay_ms(5000);
				USART2_sendstr("AT+MQTTPUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"LockState\\\":0}}\",0,0\r\n"); 
				delay_ms(500);
				//��������һ��ʹ������ϴ���ʪ����Ϣ
				OSTaskResume(&tcb1,&err);
				flag_passwd = 0;
			
			}
			delay_ms(1000);
			Show_Menu();
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err);
		}
	}
}

//��������ʾ
void Show_Menu(void)
{
	OLED_Clear();
	/*   
		=================
		   �� �� �� ��
		1.�� �� �� ��
		2.ˢ �� �� ��
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

//����Ա������ʾ
void Show_Admin_Meun(void)
{
	OLED_Clear();
	OLED_ShowString(15,1,"Welcome Admin!",16);
	OLED_ShowString(0,3,"0.Enter",8);
	OLED_ShowString(0,4,"1.Exit",8);
	OLED_ShowString(0,5,"2.modify passwd",8);
	OLED_ShowString(0,6,"3.Enter Card",8);
}

//��������
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

//�ж�id���Ƿ�Ϊ����Ա������ͨ�û��ģ��������ᱨ��
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
		status    = MFRC522_Anticoll(card_numberbuf);			//��ײ����			
		card_size = MFRC522_SelectTag(card_numberbuf);	        //ѡ��
		status    = MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
		status   =  MFRC522_Read(4, card_readbuf);					//����	
		printf("�������ͣ�%#x %#x",card_buf[0],card_buf[1]);

		//�����к��ԣ����һ�ֽ�Ϊ����У����
		printf("�������кţ�");
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
			MFRC522_AT24CXX_Read(Card_Addr+1+5*j, card_numberbuf_s,5); // ��
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



//�����޸�
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
	




//¼���¿�
void ID_Card_Enter(void)
{
	u8 i,status,card_size;
	u8  card_numberbuf_s[5];     					 // ����

	OLED_Clear();
	OLED_ShowString(0,0,"Enter User",16);
	OLED_ShowString(0,2,"Please Swipe",16);
	OLED_ShowString(0,4,"Key0:Exit",16);
	card_buf[0] = 0;
	card_buf[1] = 0;

	while(1)
	{	
		status = MFRC522_Request(0x52,card_buf);			//Ѱ��
		if(status == MI_OK)
		{
			status    = MFRC522_Anticoll(card_numberbuf);			            // ��ײ����			
			card_size = MFRC522_SelectTag(card_numberbuf);	                // ѡ��
			status    = MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	// �鿨
			status    =  MFRC522_Read(4, card_readbuf);					    // ����
			OLED_Clear(); // ���(0,12)~(128,64)
			break;
		}
		//�����ˡ�����
		if(!KEY0)
		{
			OLED_Clear();
			return;
		}
	}
	if(status == MI_OK)		//���������
	{


		//��������ʾ����λΪKbits
		OLED_Clear();
		OLED_ShowString(0,0,"Register User",16);
		OLED_ShowString(0,2,"ID:",8);   // ��OLED ��Ļ����ʾ����
		printf("�¿���:");
		for(i = 0; i<5; i++)
		{
			printf("%d ",card_numberbuf[i]);
			OLED_ShowNum(16+i*24, 2,card_numberbuf[i],3,8);    
		}
		printf("\n");
//		card_num = 0;
		delay_ms(1500);
		MFRC522_AT24CXX_Write(Card_Addr+1+5*card_num, card_numberbuf,5);    // д
		delay_ms(1000);
		MFRC522_AT24CXX_Read(Card_Addr+1+5*card_num, card_numberbuf_s,5); // ��
		card_num++;
		MFRC522_AT24CXX_Write(Card_Addr, &card_num,1);    // д
		delay_ms(500);
		u8 card_n;
		MFRC522_AT24CXX_Read(Card_Addr, &card_n,1); // ��
		printf("card_n = %d\n",card_n);
		if(memcmp(card_numberbuf,card_numberbuf_s,5) != 0)
		{
			OLED_ShowString(20, 3, "Register Fail!", 16);
			delay_ms(500);
			memcpy(card_numberbuf, "0", 5);   // ���card_numberbuf�����ֵ
			OLED_Clear();
			return;
		} 
		OLED_ShowString(20, 3, "Register Success!", 16);
		delay_ms(600);
		memset(card_numberbuf, 0, sizeof(card_numberbuf));  // ���card_numberbuf�����ֵ
		OLED_Clear();
	}	
}	

//����3:������
void fire_detect(void *arg)
{
	OS_ERR err;
	while(1)
	{
		Fire_Detect();
		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_DLY,&err);
	
	}

}




