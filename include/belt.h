#ifndef __BELT_H
#define __BELT_H

#include "stm32f10x.h"
#include "delay.h"
/**********************皮带1 IO配置****************************/
//#define	BELT_SPEED11_IO					GPIO_Pin_4
//#define	BELT_SPEED11_PORT				GPIOA
//#define	BELT_SPEED12_IO					GPIO_Pin_2
//#define	BELT_SPEED12_PORT				GPIOA
//#define	BELT_SPEED13_IO					GPIO_Pin_3
//#define	BELT_SPEED13_PORT				GPIOA
//#define	BELT_DIR11_IO						GPIO_Pin_1
//#define	BELT_DIR11_PORT					GPIOA
//#define	BELT_ERR11_IO						GPIO_Pin_0
//#define	BELT_ERR11_PORT					GPIOB
#define	BELT11_SPEED1_IO				GPIO_Pin_5
#define	BELT11_SPEED1_PORT			GPIOB
#define	BELT11_SPEED2_IO				GPIO_Pin_12
#define	BELT11_SPEED2_PORT			GPIOC
#define	BELT11_SPEED3_IO				GPIO_Pin_2
#define	BELT11_SPEED3_PORT			GPIOD
#define	BELT11_DIR_IO						GPIO_Pin_11
#define	BELT11_DIR_PORT					GPIOC
#define	BELT11_ERR_IO						GPIO_Pin_10
#define	BELT11_ERR_PORT					GPIOC

#define	BELT11_SENSOR1_IO				GPIO_Pin_1
#define	BELT11_SENSOR1_PORT			GPIOB
#define	BELT11_SENSOR2_IO				GPIO_Pin_9
#define	BELT11_SENSOR2_PORT			GPIOB

#define BELT11_SPEED1_SET  			GPIO_SetBits(BELT11_SPEED1_PORT, BELT11_SPEED1_IO)
#define BELT11_SPEED1_RESET 			GPIO_ResetBits(BELT11_SPEED1_PORT, BELT11_SPEED1_IO)
#define BELT11_SPEED2_SET  			GPIO_SetBits(BELT11_SPEED2_PORT, BELT_SPEED2_IO)
#define BELT11_SPEED2_RESET 			GPIO_ResetBits(BELT11_SPEED2_PORT, BELT11_SPEED2_IO)
#define BELT11_SPEED3_SET  			GPIO_SetBits(BELT11_SPEED3_PORT, BELT_SPEED3_IO)
#define BELT11_SPEED3_RESET 			GPIO_ResetBits(BELT11_SPEED13_PORT, BELT11_SPEED3_IO)
#define BELT11_DIR_SET  				GPIO_SetBits(BELT11_DIR_PORT, BELT11_DIR_IO)
#define BELT11_DIR_RESET 				GPIO_ResetBits(BELT11_DIR_PORT, BELT11_DIR_IO)

#define READ_BELT11_ERR    			GPIO_ReadInputDataBit(BELT11_ERR_PORT,BELT_ERR_IO) 
#define READ_BELT11_SENSOR1    	GPIO_ReadInputDataBit(BELT11_SENSOR1_PORT,BELT11_SENSOR1_IO)
#define READ_BELT11_SENSOR2    	GPIO_ReadInputDataBit(BELT11_SENSOR2_PORT,BELT11_SENSOR2_IO)

//#define BELT11_SPEED1				 		PAout(4)// PB5
//#define BELT11_SPEED2 						PAout(2)// PB5
//#define BELT11_SPEED3						PAout(3)// PB5
//#define BELT11_DIR 							PAout(1)// PB5
// #define BELT11_ERR 							PCout(10)// PB5

#define BELT11_SPEED1				 		PBout(5)// PB5
#define BELT11_SPEED2 					PCout(12)// PB5
#define BELT11_SPEED3						PDout(2)// PB5
#define BELT11_DIR 							PCout(11)// PB5

#define Belt11_Speed(x3,x2,x1)	do{	BELT11_SPEED1 = x1;BELT11_SPEED2 = x2;BELT11_SPEED3 = x3;}while(0)
/**********************皮带1 IO配置****************************/
/**********************皮带2 IO配置****************************/
#define	BELT12_SPEED1_IO					GPIO_Pin_5
#define	BELT12_SPEED1_PORT				GPIOA
#define	BELT12_SPEED2_IO					GPIO_Pin_6
#define	BELT12_SPEED2_PORT				GPIOA
#define	BELT12_SPEED3_IO					GPIO_Pin_7
#define	BELT12_SPEED3_PORT				GPIOA
#define	BELT12_DIR_IO							GPIO_Pin_8
#define	BELT12_DIR_PORT						GPIOA
#define	BELT12_ERR_IO							GPIO_Pin_1
#define	BELT12_ERR_PORT						GPIOB

#define	BELT12_SENSOR1_IO					GPIO_Pin_10
#define	BELT12_SENSOR1_PORT				GPIOB
#define	BELT12_SENSOR2_IO					GPIO_Pin_11
#define	BELT12_SENSOR2_PORT				GPIOB

#define BELT12_SPEED1_SET  				GPIO_SetBits(BELT12_SPEED1_PORT, BELT12_SPEED1_IO)
#define BELT12_SPEED1_RESET 			GPIO_ResetBits(BELT12_SPEED1_PORT, BELT12_SPEED1_IO)
#define BELT12_SPEED2_SET  				GPIO_SetBits(BELT12_SPEED2_PORT, BELT12_SPEED2_IO)
#define BELT12_SPEED2_RESET 			GPIO_ResetBits(BELT12_SPEED2_PORT, BELT12_SPEED2_IO)
#define BELT12_SPEED3_SET  				GPIO_SetBits(BELT12_SPEED3_PORT, BELT12_SPEED3_IO)
#define BELT12_SPEED3_RESET 			GPIO_ResetBits(BELT12_SPEED3_PORT, BELT12_SPEED3_IO)
#define BELT12_DIR_SET  					GPIO_SetBits(BELT12_DIR_PORT, BELT12_DIR_IO)
#define BELT12_DIR_RESET 					GPIO_ResetBits(BELT12_DIR_PORT, BELT12_DIR_IO)

#define READ_BELT12_ERR    				GPIO_ReadInputDataBit(BELT12_ERR_PORT,BELT12_ERR_IO) 
#define READ_BELT12_SENSOR1    		GPIO_ReadInputDataBit(BELT12_SENSOR1_PORT,BELT12_SENSOR1_IO)
#define READ_BELT12_SENSOR2    		GPIO_ReadInputDataBit(BELT12_SENSOR2_PORT,BELT12_SENSOR2_IO)

#define BELT12_SPEED1				 			PAout(5)// PB5
#define BELT12_SPEED2 						PAout(6)// PB5
#define BELT12_SPEED3							PAout(7)// PB5
#define BELT12_DIR								PAout(8)// PB5
// #define BELT_ERR 							PCout(10)// PB5

#define Belt12_Speed(x3,x2,x1)	do{	BELT12_SPEED1 = x1;BELT12_SPEED2 = x2;BELT12_SPEED3 = x3;}while(0)
/**********************皮带2 IO配置****************************/

/**现场人员只允许修改这三个参数,时间只是个大概**/
#define START_TIMEOUT		80			//软启动时间，80*5ms*2 =0.8S
#define STOP_TIMEOUT		80			//软停止时间, 80*5ms*2 =0.8S
#define BELT_STOP_DELAY			200		//200*2*5ms=2s,延时多久只需要改这个参数
/**现场人员只允许修改这三个参数,时间只是个大概**/




void Belt11_Config(void);
void Belt12_Config(void);
void Belt11_Control(void);
void Belt12_Control(void);
void belt11_time_control(void);
void belt12_time_control(void);
#endif




