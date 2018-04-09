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
	LCD_Init1();
	delay_ms(200);
	while(1){
		LCD_Clear(BLUE);
		LCD_ShowString(0,0,240,16,16,"hello,what can i do for you");
		LCD_ShowString(0,16,240,16,16,"hello,what can i do for you");
		LCD_ShowString(0,32,240,16,16,"hello,what can i do for you");
		delay_ms(1000);
		delay_ms(1000);
		LCD_Clear(RED);
		delay_ms(1000);

	}

}


//testBlockWrite();
//test_long2bytes();
//testWalletApps();
//testBlockRead();
//Mfrc_Loop();


