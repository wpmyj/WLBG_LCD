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
u8 testbuf1[32]=
{
	0x04,0x40,0x0E,0x50,0x78,0x48,0x08,0x48,0x08,0x40,\
	0xFF,0xFE,0x08,0x40,0x08,0x44,0x0A,0x44,0x0C,0x48,\
  0x18,0x30,0x68,0x22,0x08,0x52,0x08,0x8A,0x2B,0x06,\
  0x10,0x02,
};
u8 testbuf2[32]=
{
	0x04,0x40,0x0E,0x50,0x78,0x48,0x08,0x48,0x08,0x40,\
	0xFF,0xFE,0x08,0x40,0x08,0x44,0x0A,0x44,0x0C,0x48,\
  0x18,0x30,0x68,0x22,0x08,0x52,0x08,0x8A,0x2B,0x06,\
  0x10,0x02,
};
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
		Show_Str(10,10,6*16,16,"我们是好孩子",16,0);
		Show_Str(50,10,6*32,32,"我们是好孩子",32,0);
		Show_Str(100,100,12*11,24,"hello world",24,0);
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


