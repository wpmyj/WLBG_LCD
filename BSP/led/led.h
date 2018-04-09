#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
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

				
#define       GLED_OFF   	GPIO_SetBits(GPIOA, GPIO_Pin_0)
#define       GLED_ON  		GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define       RLED_OFF   	GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define       RLED_ON 	 	GPIO_ResetBits(GPIOA, GPIO_Pin_1)

extern u8 Gled_Num;
extern u8 Rled_Num;
extern u8 Gled_Light_Static;
extern u8 Rled_Light_Static;
void LED_GPIO_Config(void);
void Led_Flash(void);

#endif
