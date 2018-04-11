#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"

#define       RE485_SEND    	  GPIO_SetBits(GPIOA, GPIO_Pin_11)
#define       RE485_REC  				GPIO_ResetBits(GPIOA, GPIO_Pin_11)



void USART1_IRQHandler(void);	
int fputc(int ch, FILE *f);
void USART1_Putc(unsigned char c);
void USART1_Puts(char * str);
void USART1_Config(void);
void USART2_Config(void);
void USART2_Putc(unsigned char c);
void USART2_Puts(char * str);
void USART3_Config(void);
void USART3_Putc(unsigned char c);
void USART3_Puts(char * str);

void USART1_Do_Tx(void );
void USART2_Do_Tx(void );
void USART3_Do_Tx(void );
void USART1_Do_Rx(u8 rxdata);
void USART2_Do_Rx(u8 rxdata);
void USART3_Do_Rx(u8 rxdata);
//void Respond_Host_Comm(void);
//void Dispose_Data_For_Host(void);
#endif
