#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

#define		BEEP_IO					GPIO_Pin_7
#define		BEEP_PORT				GPIOB
#define   BEEP_RCC				RCC_APB2Periph_GPIOB


#define       BEEP_ON   GPIO_SetBits(BEEP_PORT,BEEP_IO	)
#define       BEEP_OFF  GPIO_ResetBits(BEEP_PORT, BEEP_IO	)

extern u8 Beep_Num;
extern u8 ERR_Beep_State;
void BEEP_GPIO_Config(void);
void Beep_Response(void );
void Beep_Sound(u8 num,u16 ms_time);
#endif
