#ifndef __HEADTYPE_H_
#define __HEADTYPE_H_

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>	


/*************Typedef datatype start*******************/
typedef char int8;
typedef volatile char vint8;
typedef unsigned char uint8;
typedef volatile unsigned char vuint8;
typedef int int16;
typedef unsigned short uint16;
typedef long int32;
typedef unsigned long uint32;
typedef uint8	uchar,u8,BYTE;		/**< 8-bit value */
typedef uint8 	SOCKET;
typedef uint16	ushort,u16,WORD;	/**< 16-bit value */
typedef uint16	uint;		/**< 16-bit value */
typedef uint32	ulong;		/**< 32-bit value */

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */
/*************Typedef datatype end*******************/

/*************define type start*******************/
#define false      (0)
#define true       (!false)

#ifndef NULL
#define NULL  ((void *)(0))
#endif


#define RxBufMax 512
#define TxBufMax 512

/*自动成帧超时定时时间*/
#define AUTO_FRAME_TIMEOUT1  10  //10*2ms
#define AUTO_FRAME_TIMEOUT2  10  //10*2ms
#define AUTO_FRAME_TIMEOUT3  10  //10*2ms
/*自动成帧超时定时时间*/
//响应超时时间，即上位机确认二维码正确后扫描枪一直扫描直到得到下一次数据的最长时间
#define ANSWER_SCANTIME	 	 4000	  //4000*5ms
//无响应超时时间，即当下位机发送数据给上位机，而上位机没有响应的最长时间
#define NANSWER_TIME	 1000	  //1000*5ms
#define NANSWER_NUMOUT	 	 3	  //1000*5ms

#define LOCK_OPEN_TIME	60		//开锁动作执行时间 1000*5ms
#define LOCK_CLOSE_TIME	20		//开锁动作执行时间 1000*5ms


#define  RS485_COM PAout(11)       
#define  RS485_REN()	RS485_COM = 0	
#define  RS485_TEN()	RS485_COM = 1
/*************define type end*******************/

/*************union type start*******************/
typedef union{
	long l;
	unsigned long ul;
	int  i;
	unsigned int ui;
	short si[2];
	char  c[4];
	unsigned char uc[4];	
}UL;

typedef union{
	short i;
	unsigned short ui;
	char c[2];
	unsigned char uc[2];
}UI;

/*************union type end*******************/

/*************enum type start*******************/
enum
{
	False=0,True
};
enum{
	READLOW,READHIGH
};
typedef enum{
	SLAVE,
	HOST
}MCU_State_Type;
typedef enum{
	NO_USART_WORK,
	USART2_WORK,
	USART3_WORK,
}USART_WORK_Type;
typedef enum{
	RESERVE,
	READY,
	WORKING,
	WORKEND,
	END
}CH_Work_Enum_Type;
typedef enum{
	MENU_RESERVE,
	MENU_READPARAM,
	MENU_SETPARAM,
	MENU_SAVEPARAM,
	MENU_END
}Menu_Option;
/*************enum type end*******************/

/*************struct type start*******************/

typedef struct{
    u16 tx_index;        //发送当前数据的索引
    u16 rx_index;        //接收到当前数据的索引
    u16 tx_count;        //发送数据总数
    u16 rx_count;        //接收数据总数
    u8 rx_start;
    u8 rx_aframe;       //已经接收到一帧数据
    u8 txbuf[RxBufMax]; //发送数据缓存区
    u8 rxbuf[RxBufMax]; //接收数据缓存区
}Usart_Type;
//主机发送数据协议字节
typedef struct{
	u8  frame_soh;
	u8  frame_x;
	u16 datasize;
	u8  ch1_state;
	u8  ch1_num;
	u8  ch2_state;
	u8  ch2_num;
	u8  ch3_state;
	u8  ch3_num;
	u8  ch4_state;
	u8  ch4_num;
	u8  belt_state;
	u8  belt_time;
	u16 crc16_ccitt; 
	u8  frame_end1;
	u8  frame_end2;
}Communation_Send_Type;
//主机接收响应协议字节
typedef struct{
	u8  frame_start;
	u8  comm;
	u8  addr;
	u8 lockH;
	u8 lockL;
  u8 lrcH;
  u8 lrcL;
	u8 frame_end1;
	u8 frame_end2;
}Communation_Rec_Type;

typedef union{
	Communation_Send_Type control;
	u8	send_buf[28];	
}COMM_Send_Union_Type;

typedef union{
	Communation_Rec_Type control;
	u8	rec_buf[9];	
}COMM_Rec_Union_Type;

typedef struct{
	u8  answer_state;
	u8 	answer_numout;
	u16 answer_timeout;
	u16 Nanswer_timeout;
}Answer_Type;

typedef struct{
	CH_Work_Enum_Type  state;
	u8 	send_num;			//需要发药数，由PC机发送过来
	u8  motor_pulse;	//电机转过的圈数，使用传感器
	u8  send_actual;	//实际发药数，使用传感器计数
	u8  motor_state;	//电机的工作状态，正在工作时不允许执行状态转换
	u8 	timeoutstart; //电机实际发药数超过PC需要发药数时开启超时变量
	u16  timeout;			//电机实际发药数超时时间
	u8  motor_irqstate;//电机中断软件滤波状态
	u8 motor_irqtime;//电机中断软件滤波时间
	u8 send_actual_irqstate;	//实时计数中断软件滤波状态
	u8 send_actual_irqtime;		//实时计数中断软件滤波时间
	u8 motor_start_state;			//电机启动延时状态
	u16 motor_start_time;			//电机延时启动时间
}CH_Work_Type;

typedef struct{
	CH_Work_Enum_Type  state;
	u8 	 send_time;
	u16  actual_time;
	u8	 speed_step;
	u8   start_signal;
	u8   stop_signal;
	u8   start_ok;
	u8   stop_ok;
	u8   stop_delay_state;
}Belt_Work_Type;
typedef struct{
	CH_Work_Type ch1;
	CH_Work_Type ch2;
	CH_Work_Type ch3;
	CH_Work_Type ch4;
}CH_Work;

typedef struct{
	u8 comm_head;
	u8 comm_word;
	u8 data_len;
	u8 data[4];
	u8 xorsum;
}RFID_REC_Type; 


/*************struct type end*******************/

/*************extern variable start*******************/
extern char Auto_Frame_Time1;
extern char Auto_Frame_Time2;
extern char Auto_Frame_Time3;

extern  Usart_Type Usart1_Control_Data;
extern Usart_Type Usart2_Control_Data;
extern Usart_Type Usart3_Control_Data;
extern  COMM_Send_Union_Type PC_Host_Rec;
extern  COMM_Rec_Union_Type  PC_Hosr_Send;

extern  COMM_Send_Union_Type MCU_Host_Send;
extern  COMM_Rec_Union_Type  MCU_Host_Rec;

extern  MCU_State_Type MCU_State;
extern  Answer_Type 	 PC_Answer;

extern int Lock_Excute_Time;
extern u8 open_lock_count;
extern unsigned char send_cark_flag;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

/*************extern variable end*******************/

/*************function start*******************/
unsigned char CharToHex(unsigned char bHex);
unsigned char HexToChar(unsigned char bChar);
u16 CRC_GetModbus16(u8 *pdata, int len);
u16 CRC_GetCCITT(u8 *pdata, int len);
u16 LRC_GetLRC16(u8 *pdata, int len);
u8 Key_Scan(void);
void KEY_GPIO_Config(void);
u16 switch_init_time(void);
void dispose_key(void );
void dispose_menu(void);

void TIM2_Config(void );
void TIM3_Config(void );
void TIM4_Config(void );
/*************function end*******************/
#endif


