#include "ov2640.h"
#include "delay.h"


//D0 PC6 D1 PC7 D2 PC8 D3 PC9 D4 PC11 D5 PB6 D6 PE5 D7 PE6
//VSYNC PB7

//DCMI_HREF PA4 
//DCMI_PCLK PA6 
//DCMI_XCLK PA8
//用到的pin组号
//PA PB PC PD PE PG

#define SCCB_SDA_IN {GPIOD->MODER&=~(3<<(7*2));}
#define SCCB_SDA_OUT {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=1<<7*2;}

#define SCCB_ID 				0x60

#define OV2640_MID				0X7FA2
#define OV2640_PID				0X2642

#define SCCB_SDA 			 	PDout(6)
#define SCCB_SCL 		 		PDout(7)

#define SCCB_READ_SDA  			PDin(7)

#define OV2640_PWDN 			PGout(9)
#define OV2640_RST  			PGout(15)

u8 ov2640_mode=0;						//工作模式:0,RGB565模式;1,JPEG模式

//JPEG尺寸支持列表
const u16 jpeg_img_size_tbl[][2]=
{
	176,144,	//QCIF
	160,120,	//QQVGA
	352,288,	//CIF
	320,240,	//QVGA
	640,480,	//VGA
	800,600,	//SVGA
	1024,768,	//XGA
	1280,1024,	//SXGA
	1600,1200,	//UXGA
}; 

const char *EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
const char *JPEG_SIZE_TBL[9]={"QCIF","QQVGA","CIF","QVGA","VGA","SVGA","XGA","SXGA","UXGA"};	//JPEG图片 9种尺寸



DMA_InitTypeDef DMA_InitStructure;

DCMI_InitTypeDef DCMI_InitStructure;

NVIC_InitTypeDef NVIC_InitStructure;


//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
    SCCB_SDA=0;
    delay_us(50);	 
    SCCB_SCL=1;	
    delay_us(50); 
    SCCB_SDA=1;	
    delay_us(50);
}  
//产生NA信号
void SCCB_No_Ack(void)
{
	delay_us(50);
	SCCB_SDA=1;	
	SCCB_SCL=1;	
	delay_us(50);
	SCCB_SCL=0;	
	delay_us(50);
	SCCB_SDA=0;	
	delay_us(50);
}


void dma2_init()
{
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	DMA_Stream_TypeDef DMAy_Streamx;
	
	
	


}

void sccb_init()
{
//DCMI_SCL PD6  DCMI_SDA PD7
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	PDout(6)=1;
	PDout(7)=1;
	SCCB_SDA_OUT;
	
}


//SCCB,写入一个字节
//返回值:0,成功;1,失败. 
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)SCCB_SDA=1;	
		else SCCB_SDA=0;
		dat<<=1;
		delay_us(50);
		SCCB_SCL=1;	
		delay_us(50);
		SCCB_SCL=0;		   
	}			 
	SCCB_SDA_IN;		//设置SDA为输入 
	delay_us(50);
	SCCB_SCL=1;			//接收第九位,以判断是否发送成功
	delay_us(50);
	if(SCCB_READ_SDA)res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	SCCB_SCL=0;		 
	SCCB_SDA_OUT;		//设置SDA为输出    
	return res;  
}

//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN;		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		delay_us(50);
		SCCB_SCL=1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		delay_us(50);
		SCCB_SCL=0;
	}	
	SCCB_SDA_OUT;		//设置SDA为输出    
	return temp;
} 




void dcmi_init()
{

	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_4|GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK  
 	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_DCMI); //PC6,AF13  DCMI_D0  
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_DCMI); //PC7,AF13  DCMI_D1 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_DCMI); //PC8,AF13  DCMI_D2
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_DCMI); //PC9,AF13  DCMI_D3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_DCMI);//PC11,AF13 DCMI_D4 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_DCMI); //PB6,AF13  DCMI_D5 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_DCMI); //PE5,AF13  DCMI_D6
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_DCMI); //PE6,AF13  DCMI_D7

	DCMI_DeInit();//清除原来的设置 
	
	DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//连续模式
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//全帧捕获
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8位数据格式  
	DCMI_InitStructure.DCMI_HSPolarity= DCMI_HSPolarity_Low;//HSYNC 低电平有效
	DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK 上升沿有效
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//硬件同步HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_Low;//VSYNC 低电平有效
	DCMI_Init(&DCMI_InitStructure);
 
	DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//开启帧中断 
	
	DCMI_Cmd(ENABLE);	//DCMI使能
 
	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
	
}



//DCMI,启动传输
void DCMI_Start(void)
{  
	DMA_Cmd(DMA2_Stream1, ENABLE);//开启DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI捕获使能  
}


//DCMI,关闭传输
void DCMI_Stop(void)
{ 
  DCMI_CaptureCmd(DISABLE);//DCMI捕获使关闭	
	
	while(DCMI->CR&0X01);		//等待传输结束 
	 	
	DMA_Cmd(DMA2_Stream1,DISABLE);//关闭DMA2,Stream1
} 


//DCMI设置显示窗口
//sx,sy;LCD的起始坐标
//width,height:LCD显示范围.
void DCMI_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{
	DCMI_Stop(); 
//	LCD_Clear(WHITE);
//	LCD_Set_Window(sx,sy,width,height);
	OV2640_OutSize_Set(width,height);
//  LCD_SetCursor(0,0);  
//	LCD_WriteRAM_Prepare();		//开始写入GRAM
	
	DMA_Cmd(DMA2_Stream1,ENABLE);	//开启DMA2,Stream1 
	
	DCMI_CaptureCmd(ENABLE);//DCMI捕获使能 
	
}

void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync)
{
	DCMI_DeInit();//清除原来的设置 
 
	DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//连续模式
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//全帧捕获
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8位数据格式  
	DCMI_InitStructure.DCMI_HSPolarity= hsync<<6;//HSYNC 低电平有效
	DCMI_InitStructure.DCMI_PCKPolarity= pclk<<5;//PCLK 上升沿有效
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//硬件同步HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity=vsync<<7;//VSYNC 低电平有效
	DCMI_Init(&DCMI_InitStructure);
 
	DCMI_CaptureCmd(ENABLE);//DCMI捕获使能 
	DCMI_Cmd(ENABLE);	//DCMI使能
  
}



void ov2640_Init()
{
//DCMI_PWDN PG9     上电控制
//DCMI_RESET PG15   复位控制
	RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_MODE_OUT;
	GPIO_InitStructure.GPIO_OType= GPIO_OTYPE_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	
	OV2640_PWDN=0;
	delay_ms(10);
	OV2640_RST=0;
	delay_ms(10);
	OV2640_RST=1;
	sccb_init();
	


}

//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
    SCCB_SDA=1;     //数据线高电平	   
    SCCB_SCL=1;	    //在时钟线高的时候数据线由高至低
    delay_us(50);  
    SCCB_SDA=0;
    delay_us(50);	 
    SCCB_SCL=0;	    //数据线恢复低电平，单操作函数必要	  
}



void sccb_wr(u8 data)
{
	



}

// reg 寄存器地址  data 数据
//sccb 写寄存器数据
u8 sccb_wr_reg(u8 reg,u8 data)
{
	u8 res = 0;
	SCCB_Start();
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
  	SCCB_Stop();	  
  	return	res;

}

//读寄存器
//返回值:读到的寄存器值
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID	  
	delay_us(100);	 
  	SCCB_WR_Byte(reg);			//写寄存器地址	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令	  
	delay_us(100);
  	val=SCCB_RD_Byte();		 	//读取数据
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}

u8 OV2640_OutSize_Set(u16 width,u16 height)
{
	u16 outh;
	u16 outw;
	u8 temp; 
	if(width%4)
		return 1;
	if(height%4)
		return 2;
	outw=width/4;
	outh=height/4; 
	sccb_wr_reg(0XFF,0X00);	
	sccb_wr_reg(0XE0,0X04);			
	sccb_wr_reg(0X5A,outw&0XFF);		//设置OUTW的低八位
	sccb_wr_reg(0X5B,outh&0XFF);		//设置OUTH的低八位
	temp=(outw>>8)&0X03;
	temp|=(outh>>6)&0X04;
	sccb_wr_reg(0X5C,temp);				//设置OUTH/OUTW的高位 
	sccb_wr_reg(0XE0,0X00);
	



}

u8 OV2640_ImageWin_Set(u16 offx,u16 offy,u16 width,u16 height)
{
	u16 hsize;
	u16 vsize;
	u8 temp; 
	if(width%4)return 1;
	if(height%4)return 2;
	hsize=width/4;
	vsize=height/4;
	sccb_wr_reg(0XFF,0X00);	
	sccb_wr_reg(0XE0,0X04);					
	sccb_wr_reg(0X51,hsize&0XFF);		//设置H_SIZE的低八位
	sccb_wr_reg(0X52,vsize&0XFF);		//设置V_SIZE的低八位
	sccb_wr_reg(0X53,offx&0XFF);		//设置offx的低八位
	sccb_wr_reg(0X54,offy&0XFF);		//设置offy的低八位
	temp=(vsize>>1)&0X80;
	temp|=(offy>>4)&0X70;
	temp|=(hsize>>5)&0X08;
	temp|=(offx>>8)&0X07; 
	sccb_wr_reg(0X55,temp);				//设置H_SIZE/V_SIZE/OFFX,OFFY的高位
	sccb_wr_reg(0X57,(hsize>>2)&0X80);	//设置H_SIZE/V_SIZE/OFFX,OFFY的高位
	sccb_wr_reg(0XE0,0X00);	
	return 0;
} 

//该函数设置图像尺寸大小,也就是所选格式的输出分辨率
//UXGA:1600*1200,SVGA:800*600,CIF:352*288
//width,height:图像宽度和图像高度
//返回值:0,设置成功
//    其他,设置失败
u8 OV2640_ImageSize_Set(u16 width,u16 height)
{ 
	u8 temp; 
	sccb_wr_reg(0XFF,0X00);			
	sccb_wr_reg(0XE0,0X04);			
	sccb_wr_reg(0XC0,(width)>>3&0XFF);		//设置HSIZE的10:3位
	sccb_wr_reg(0XC1,(height)>>3&0XFF);		//设置VSIZE的10:3位
	temp=(width&0X07)<<3;
	temp|=height&0X07;
	temp|=(width>>4)&0X80; 
	sccb_wr_reg(0X8C,temp);	
	sccb_wr_reg(0XE0,0X00);				 
	return 0;
}




//处理JPEG数据
//当采集完一帧JPEG数据后,调用此函数,切换JPEG BUF.开始下一帧采集.
//void jpeg_data_process(void)
//{
//	if(ov2640_mode)//只有在JPEG格式下,才需要做处理.
//	{
//		if(jpeg_data_ok==0)	//jpeg数据还未采集完?
//		{	
//			DMA_Cmd(DMA2_Stream1, DISABLE);//停止当前传输 
//			while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//等待DMA2_Stream1可配置  
//			jpeg_data_len=jpeg_buf_size-DMA_GetCurrDataCounter(DMA2_Stream1);//得到此次数据传输的长度
//				
//			jpeg_data_ok=1; 				//标记JPEG数据采集完按成,等待其他函数处理
//		}
//		if(jpeg_data_ok==2)	//上一次的jpeg数据已经被处理了
//		{
//			DMA2_Stream1->NDTR=jpeg_buf_size;	
//			DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//传输长度为jpeg_buf_size*4字节
//			DMA_Cmd(DMA2_Stream1, ENABLE);			//重新传输
//			jpeg_data_ok=0;						//标记数据未采集
//		}
//	}
//}








