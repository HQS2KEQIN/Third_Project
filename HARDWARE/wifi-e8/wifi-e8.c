#include "wifi-e8.h"
#include "usart.h"
//向串口2传送数据

extern  u8 temp;
extern  u8 humi;
extern  u8 temp_p;
extern  u8 humi_p;
char AT[11]= "AT+RST\r\n";
char wifi_mode_buf[15] = "AT+CWMODE=x\r\n";
char wifi_connect_buf[200] = "AT+CWJAP=";
char *user_connect_buf = "AT+MQTTUSERCFG=0,1,\"gfga8FAXckk.stm32|securemode=2\\,signmethod=hmacsha256\\,timestamp=1688888102293|\",\"stm32&gfga8FAXckk\",\"94d118ea3aaccb45cfe2555f27cf835f516dc772f927f7f9ccf6b2bc67876893\",0,0,\"\"\r\n";
char *connect_server_buf = "AT+MQTTCONN=0,\"iot-06z00a0pxwplb1u.mqtt.iothub.aliyuncs.com\",1883,1\r\n";
char client_subscribe_buf[100] = "AT+MQTTSUB=0,\"/sys/gfga8FAXckk/stm32/thing/event/property/post_reply\",1\r\n";
volatile u8 Switch_1;
volatile u8 Switch_2;
char temp_flag[15]="temperature";
char humi_flag[10]="Humidity";	

char *pubtopic = "/sys/gfga8FAXckk/stm32/thing/event/property/post";
//const char* subtopic="/sys/gfga8FAXckk/stm32/thing/event/property/post_reply";

const char power_func1[15]="PowerSwitch_1";
const char power_func2[15]="PowerSwitch_2";

void USART2_sendstr(char *str)
{
	while((*str)!='\0')
	{
		USART_SendData(USART2,*str);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
		str++;
	}


}

//WIFI重启
void WIFI_RESET(void)
{
	printf("WIFI重启中...\n");

	USART2_sendstr(AT);
	delay_ms(6000);
	printf("重启成功\n");

}

//设置网络连接模式
void Setting_Connect_Work(char *type)
{

	wifi_mode_buf[10] = *type;
	USART2_sendstr(wifi_mode_buf);

}

//登陆阿里云


//连接wifi
void WIFI_Connect(void)
{
	
	strcat(wifi_connect_buf,"\"YQJY\"");
	strcat(wifi_connect_buf,",");
	strcat(wifi_connect_buf,"\"yqjyadmin\"");
	strcat(wifi_connect_buf,"\r\n");
	printf("连接wifi...\n");
	
	USART2_sendstr(wifi_connect_buf);
}

/*********************用户设备连接*******************************/
void USER_Connect(void)
{
	printf("用户设备连接...\n");
	USART2_sendstr(user_connect_buf);
}


/*********************连接阿里云服务器*************************/

void Connect_Aliyun_Server(void)
{
	printf("连接阿里云服务器...\n");
	USART2_sendstr(connect_server_buf);
}

/*********************客户端订阅*************************/
void Client_Subscribe(void)
{
	printf("客户端订阅消息...\r\n");
	USART2_sendstr(client_subscribe_buf);
}

void wait_OK(void)
{
	while(!Flag_usart2_receive_OK);//等待接收到OK字符串
	printf("ooo\n");
	Flag_usart2_receive_OK = 0;

}

void WIFI_INIT(void)
{
	WIFI_RESET();
	Setting_Connect_Work("1");
	delay_ms(1000);

	
//	LOGIN_URL();
//	wait_OK();
//	printf("222\n");
	
	WIFI_Connect();
	delay_ms(5000);

	
	USER_Connect();
	delay_ms(5000);

	
	Connect_Aliyun_Server();
	delay_ms(5000);	

	
	Client_Subscribe();
	delay_ms(3000);	

}

void Send_DHT(void)
{
	char buf[256];
	sprintf(buf,"AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%d.%d\\,\\\"%s\\\":%d.%d}}\",0,0\r\n",
	pubtopic,humi_flag,humi,humi_p,temp_flag,temp,temp_p);
	USART2_sendstr(buf);
	
	
}

