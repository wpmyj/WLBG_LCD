#ifndef __LOCK_H
#define __LOCK_H

#include "stm32f10x.h"


#define   LOCK_TIME			400

#define		SH_CLK_IO					  GPIO_Pin_11
#define		SH_CLK_PORT				  GPIOB
#define   SH_CLK_RCC					RCC_APB2Periph_GPIOB
#define		ST_CLK_IO					  GPIO_Pin_10
#define		ST_CLK_PORT				  GPIOB
#define   ST_CLK_RCC					RCC_APB2Periph_GPIOB
#define		HC595_DS_IO					GPIO_Pin_12
#define		HC595_DS_PORT				GPIOB
#define   HC595_DS_RCC				RCC_APB2Periph_GPIOB
#define		HC595_EN_IO					GPIO_Pin_9
#define		HC595_EN_PORT				GPIOB
#define   HC595_EN_RCC				RCC_APB2Periph_GPIOB

#define   SET_SH_CLK  		  	GPIO_SetBits(SH_CLK_PORT, SH_CLK_IO)
#define   RESET_SH_CLK   			GPIO_ResetBits(SH_CLK_PORT, SH_CLK_IO)
#define   SET_ST_CLK	 			  GPIO_SetBits(ST_CLK_PORT, ST_CLK_IO)
#define   RESET_ST_CLK    		GPIO_ResetBits(ST_CLK_PORT, ST_CLK_IO)

#define   RESET_HC595_DS  		GPIO_ResetBits(HC595_DS_PORT, HC595_DS_IO)
#define   SET_HC595_DS  	 		GPIO_SetBits(HC595_DS_PORT, HC595_DS_IO)
#define   HC595_ENABLE   		  GPIO_ResetBits(HC595_EN_PORT, HC595_EN_IO)
#define   HC595_DISABLE 	 		GPIO_SetBits(HC595_EN_PORT, HC595_EN_IO)

#define Delay_595   20
#define init595()	  { RESET_SH_CLK;RESET_ST_CLK;delay_us(Delay_595);}
#define Shife595()	{ SET_SH_CLK;delay_us(Delay_595); RESET_SH_CLK;delay_us(Delay_595);}
#define Out595()	  { SET_ST_CLK;delay_us(Delay_595*2); RESET_ST_CLK;delay_us(Delay_595*2);}


void LOCK_GPIO_Config(void);
void Lock_control(void );
void Write595_byte(u8 bytedata);


#endif




