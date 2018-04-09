#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "stm32f10x.h"

/*************************************************
			             预处理
*************************************************/

#define COMM_SET_MCU_IDLE       0x01 //设置模块空闲睡眠模式
#define EER_SET_MCU_IDLE        0xE0 

#define COMM_CONTROL_ANTENNA    0x02 //模块天线控制、自动寻卡设置
#define EER_CONTROL_ANTENNA     0xE1 

#define COMM_MIFARE_SEARCH_CARD	0x03 //TYPE-A寻卡
#define EER_MIFARE_SEARCH_CARD	0xE2 

#define COMM_READ_BLOCK			0x04 //读块
#define EER_READ_BLOCK			0xE3 

#define COMM_WRITE_BLOCK		0x05 //写块
#define EER_WRITE_BLOCK		    0xE4 

#define COMM_INIT_PURSE			0x06 //初始化钱包
#define EER_INIT_PURSE		    0xE5 

#define COMM_READ_PURSE		    0x07 //读取钱包
#define EER_READ_PURSE		    0xE6 

#define COMM_INC_VALUE			0x08 //充值
#define EER_INC_VALUE			0xE7
	
#define COMM_DEC_VALUE			0x09 //扣款
#define EER_DEC_VALUE			0xE8 
	
#define COMM_BAK_PURSE			0x0A //备份钱包值
#define EER_BAK_PURSE			0xE9 

#define COMM_CARD_HALT			0x0B //卡休眠
#define EER_CARD_HALT			0xEA //卡休眠

#define COMM_SELF_SEARCH_CARD	0x0C //主动读ID号
#define EER_SELF_SEARCH_CARD	0xEB 

//================================================


extern u8 slaveaddr;




void Do_Usrat2_RFIDCmd(u8 cmd );
void Do_Usrat3_RFIDCmd(u8 cmd );
void PC_Communication_Time_ISR(void );
u8 Execute_Host_Comm(void);


void Communication_Process(void );
#endif



