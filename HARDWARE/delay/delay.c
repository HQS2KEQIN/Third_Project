#include "delay.h"
//�Ĵ����ĵ�ַ
#define  DWT_CR      (*(uint32_t *)0xE0001000)		//ʱ����Ĵ����Ŀ��ƼĴ���
#define  DWT_CYCCNT  (*(uint32_t *)0xE0001004)		//ʱ����Ĵ����ļ����Ĵ���
#define  DEM_CR      (*(uint32_t *)0xE000EDFC)		//�ں˵��ԼĴ���

#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

//1us��168 ������
void delay_us(u32 nus)
{
	uint32_t Timecnt = 168 *nus; //����nusʱ�����ж��ٸ�ʱ������
	uint32_t Getcnt=0;
	OS_ERR err; 
	
	OSSchedLock(&err);//UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ---û�н�ֹ�����ж�
	
	 //ʹ��DWT����
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;                
    
	//DWT CYCCNT�Ĵ���������0
	DWT_CYCCNT = (uint32_t)0u;
	
	//ʹ��DWT CYCCNT�Ĵ���
  	DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
	
	//ÿ�ζ�ȡ�������е���ֵ��ֱ����������ֵ����Ŀ��ֵ
	do{
		Getcnt = OS_TS_GET();
	}while(Getcnt < Timecnt);
	
	//�ر�DWT CYCCNT�Ĵ���,�رռ���
	DWT_CR &= ~DWT_CR_CYCCNTENA;
	
	OSSchedUnlock(&err);//UCOSIII�ķ�ʽ,�ָ�����
}

//1ms���� 168000������
void delay_ms(u32 nms)
{
	uint32_t Timecnt = 168000 *nms; //����nmsʱ�����ж��ٸ�ʱ������
	uint32_t Getcnt=0;
	OS_ERR err; 
	
	OSSchedLock(&err);//UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ---û�н�ֹ�����ж�
	
	 //ʹ��DWT����
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;                
    
	//DWT CYCCNT�Ĵ���������0
	DWT_CYCCNT = (uint32_t)0u;
	
	//ʹ��DWT CYCCNT�Ĵ���
  	DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
	
	//ÿ�ζ�ȡ�������е���ֵ��ֱ����������ֵ����Ŀ��ֵ
	do{
		Getcnt = OS_TS_GET();
	}while(Getcnt < Timecnt);
	
	//�ر�DWT CYCCNT�Ĵ���,�رռ���
	DWT_CR &= ~DWT_CR_CYCCNTENA;
	
	OSSchedUnlock(&err);//UCOSIII�ķ�ʽ,�ָ�����
	
}



