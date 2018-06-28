#ifndef __USART_H
#define __USART_H

#include "stm32f4xx_hal.h"
#include "stdio.h"

#define       RE485_SEND    	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET)
#define       RE485_REC  				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET)



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
void USART1_Do_Rx(unsigned char rxdata);
void USART2_Do_Rx(unsigned char rxdata);
void USART3_Do_Rx(unsigned char rxdata);
void RS485_start_send_byte(unsigned int  send_count);
//void Respond_Host_Comm(void);
//void Dispose_Data_For_Host(void);
#endif
