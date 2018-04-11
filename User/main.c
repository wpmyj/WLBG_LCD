/**
  ******************************************************************************
  * @file    main.c 
  * @version V0.1
  * @date    2017.12.13
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
#include"HeadType.h"
#include "lcd.h"
#include "text.h"	

extern void USART1_Config(u32 baudrate);
extern void KEY_GPIO_Config(void);
extern u8 Read_key(void);
/**
  * @brief  主函数
  * @param  无
  * @return 无
  */

int main(void)
{
	delay_init();
//	USART1_Config(9600);
  TIM2_Config();
//	LED_GPIO_Config();
//  BEEP_GPIO_Config();
	ROM_GT30L_Init();
	LCD_Init();
	delay_ms(200);
	while(1){
		LCD_Clear(BLUE);
		Show_Str_Mid(0,0,"我们是好孩子",16,320);
	  Show_Str(20,20,6*32,"我们是好孩子",24,0);
		Show_Str(40,50,6*32,"我们是好孩子",24,0);
		Show_Str(100,100,16*6,"Hello",32,0);
		LCD_ShowString(10,130,12*32,"World",24,0);
		delay_ms(1000);
		delay_ms(1000);
		LCD_Clear(RED);
		delay_ms(1000);

	}

}



