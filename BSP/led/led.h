#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"
#define ON 	0
#define OFF 1

//“\”续行符的语法要求极其严格，在它的后面不能有空格、注释等一切“杂物”
#define LED1(a)	if(a)	\
				GPIO_SetBits(GPIOA, GPIO_Pin_0);\
				else	\
				GPIO_ResetBits(GPIOA, GPIO_Pin_0)

#define LED2(a)	if(a)	\
				GPIO_SetBits(GPIOA, GPIO_Pin_1);\
				else	\
				GPIO_ResetBits(GPIOA, GPIO_Pin_1)
				
// 直接操作寄存器的方法控制IO
#define digitalHigh(p,i) {p->BSRR = i;}			// 设置为高电平
#define digitalLow(p,i) {p->BRR = i;}			// 设置为低电平
#define digitalToggle(p,i) {p->ODR ^= i;}		// 设置为反转电平

#define LED1_TOGGLE digitalToggle(GPIOA,GPIO_Pin_0)
#define LED1_OFF 	digitalHigh(GPIOA,GPIO_Pin_0)
#define LED1_ON 	digitalLow(GPIOA,GPIO_Pin_0)

#define LED2_TOGGLE digitalToggle(GPIOCA,GPIO_Pin_1)
#define LED2_OFF 	digitalHigh(GPIOA,GPIO_Pin_1)
#define LED2_ON 	digitalLow(GPIOA,GPIO_Pin_1)

#define       GLED_OFF   	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET)
#define       GLED_ON  		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)
#define       RLED_OFF   	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET)
#define       RLED_ON 	 	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET)

#define       RGB_GLED_ON   	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET)
#define       RGB_GLED_OFF  	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET)
#define       RGB_RLED_ON   	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET)
#define       RGB_RLED_OFF	 	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET)
#define       RGB_BLED_ON   	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET)
#define       RGB_BLED_OFF 	 	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET)

extern unsigned char Gled_Num;
extern unsigned char Rled_Num;
extern unsigned char Gled_Light_Static;
extern unsigned char Rled_Light_Static;
void LED_GPIO_Config(void);
void Led_Flash(void);

#endif
