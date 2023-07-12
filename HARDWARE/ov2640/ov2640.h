#ifndef OV2640_H
#define	OV2640_H

#include "stm32f4xx.h"
#include "sys.h"



u8 OV2640_OutSize_Set(u16 width,u16 height);


//0xFF = 0时寄存器列表

#define R_BYPASS  	 0x05

#define QS  		 0x44

#define CTRLI 		 0x50
#define HSIZE 		 0x51
#define VSIZE 		 0x52
#define XOFFL 		 0x53
#define YOFFL 		 0x54
#define VHYX 		 0x55
#define DPRP 		 0x56
#define TEST 		 0x57
#define ZMOW 		 0x5A
#define ZMOH 		 0x5B
#define ZMHH 		 0x5C
#define BPADDR 		 0x7C
#define BPDATA 		 0x7D

#define CTRL2 		 0x86
#define CTRL3 		 0x87
#define SIZEL 		 0x8C
#define HSIZE8 		 0xC0
#define VSIZE8 		 0xC1
#define CTRL0 		 0xC2
#define CTRL1 		 0xC3
#define R_DVP_SP 	 0xD4
#define IMAGE_MODE 	 0xDA
#define RESET 	 	 0xE0
#define MS_SP 	 	 0xF0
#define SS_ID 	 	 0xF7   //设备id
#define SS_CTRL 	 0xF8
#define MC_BIST 	 0xF9
#define MC_AL 	 	 0xFA
#define MC_AH 	 	 0xFB
#define MC_D 	 	 0xFC
#define P_CMD 	 	 0xFD
#define P_STATUS 	 0xFE
#define RA_DLMT 	 0xFF

//当选择传感器地址(0XFF=0X01)时,OV2640的DSP寄存器地址映射表
#define GAIN       0x00
#define COM1       0x03
#define REG04      0x04
#define REG08      0x08
#define COM2       0x09
#define PIDH       0x0A
#define PIDL       0x0B
#define COM3       0x0C
#define COM4       0x0D
#define AEC        0x10
#define CLKRC      0x11
#define COM7       0x12
#define COM8       0x13
#define COM9       0x14
#define COM10      0x15
#define HREFST     0x17
#define HREFEND    0x18
#define VSTART     0x19
#define VEND       0x1A
#define MIDH       0x1C
#define MIDL       0x1D
#define AEW        0x24
#define AEB        0x25
#define W          0x26
#define REG2A      0x2A
#define FRARL      0x2B
#define ADDVSL     0x2D
#define ADDVHS     0x2E
#define YAVG       0x2F
#define REG32      0x32
#define ARCOM2     0x34
#define REG45      0x45
#define FLL        0x46
#define FLH        0x47
#define COM19      0x48
#define ZOOMS      0x49
#define COM22      0x4B
#define COM25      0x4E
#define BD50       0x4F
#define BD60       0x50
#define REG5D      0x5D
#define REG5E      0x5E
#define REG5F      0x5F
#define REG60      0x60
#define HISTO_LOW  0x61
#define HISTO_HIGH 0x62



#endif



