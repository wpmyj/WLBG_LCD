#include "stm32f4xx_hal.h"
#include"HeadType.h"
#include "lcd.h"
#include "text.h"	
#include "GUI.h"

#define	DEVICE1_KEY1_IO					 GPIO_PIN_2
#define	DEVICE1_KEY1_PORT				 GPIOA
#define	DEVICE1_KEY2_IO					 GPIO_PIN_4
#define	DEVICE1_KEY2_PORT				 GPIOE
#define READ_DEVICE1_KEY1    		 HAL_GPIO_ReadPin(DEVICE1_KEY1_PORT,DEVICE1_KEY1_IO)//返回的是一个字节，读的是一个位
#define READ_DEVICE1_KEY2    		 HAL_GPIO_ReadPin(DEVICE1_KEY2_PORT,DEVICE1_KEY2_IO)//返回的是一个字节，读的是一个位


#define KEY_SHORT_TIME 		    15
#define KEY_LONG_TIME			    100
#define KEY_LONGLONG_TIME			400
u8 Key_ScanNum;
u8 Key_SetParamFlag;
Menu_Option Menu;
unsigned short  timeflag;
unsigned short  Menu_Exit_Time;
u8   AdrrOK_Flag;
unsigned long Menu_Valid_Time = MENU_VALID_TIME;
u8 slaveaddr = 3;
//=============================================================================
//函数名称: KEY_GPIO_Config
//功能概要:LED灯引脚配置
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
void KEY_GPIO_Config(void)
{	
	 GPIO_InitTypeDef  GPIO_InitStruct;
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOE_CLK_ENABLE();
	
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

u8 Key_Scan(void)
{
	 u8 key_num;
	 static u8 key1_triggerstate;
	 static u16 key1_timercount;
	 static u8 longlongkey_flag = 0;
	  key_num = 0;
	 //key1
		if(READ_DEVICE1_KEY1 == READLOW){
			if(longlongkey_flag == 0){
				if(key1_triggerstate == 0){
					key1_triggerstate = 1;
				}else{
					key1_timercount++;
					if(key1_timercount >= KEY_LONGLONG_TIME){
						key_num |=0xFF;
						longlongkey_flag = 1;
				  }
				}
			}
		}else{
			if(longlongkey_flag == 1){
				longlongkey_flag = 0;
				key1_timercount = 0;
				key1_triggerstate =0;
			}else{
				if(key1_triggerstate == 1){
					if(key1_timercount <KEY_SHORT_TIME){
						key_num &=0xfe;
					}else if((key1_timercount >=KEY_SHORT_TIME)&&(key1_timercount <KEY_LONG_TIME)){
							key_num |=0x01;
					}else{
							key_num |=0x11;
					}
					key1_triggerstate =0;				
				}
				key1_timercount = 0;
			}
		}
	
	 return key_num;
}

void dispose_key(void )
{
	if(Key_ScanNum == 0xFF){
		if(Key_SetParamFlag == 0){
			Key_SetParamFlag = 1;
			LCD_Clear(BLACK);
			Menu = MENU_READPARAM;
			Menu_Exit_Time = MENU_EXIT_TIME;
		}else{
			Menu = MENU_SAVEPARAM;
		}
		Key_ScanNum = 0;
	}
}

void dispose_menu(void)
{
	static u8 slaveaddrtemp1,slaveaddrtemp2;
	static u8 slavedisplay[4];
	static u8 param_flash_flag = 0;
	switch(Menu){
	case MENU_RESERVE:break;
	case MENU_READPARAM: slaveaddrtemp1 = AT24CXX_ReadOneByte(0x00);slaveaddrtemp2 = AT24CXX_ReadOneByte(0x01);
	  	Show_Str(32,32*4,8*32,"修改前设备地址: ",BACK_COLOR,POINT_COLOR,32,0);
		  Show_Str(32,32*2,8*32,"请修改当前地址: ",BACK_COLOR,POINT_COLOR,32,0);
		if((slaveaddrtemp1 == slaveaddrtemp2)&&(slaveaddrtemp1 < 128)){
//					slaveaddr=slaveaddrtemp1;
					slavedisplay[0] = slaveaddrtemp1/100+ 0x30;
					slavedisplay[1] = slaveaddrtemp1%100/10 + 0x30;
					slavedisplay[2] = slaveaddrtemp1%10 + 0x30;
					slavedisplay[3] = '\0';
			    Show_Str(32*4,32*3,7*32,(u8 *)slavedisplay,BACK_COLOR,POINT_COLOR,32,0);
			    Show_Str(32*4,32*5,7*32,(u8 *)slavedisplay,BACK_COLOR,POINT_COLOR,32,0);
		}else{
				slaveaddrtemp1=0;
				slavedisplay[0] = slavedisplay[1] = slavedisplay[2] ='0';
				slavedisplay[3] = '\0';
				Show_Str(32*4,32*3,7*32,(u8 *)slavedisplay,BACK_COLOR,POINT_COLOR,32,0);
			  Show_Str(32*4,32*5,7*32,"0",BACK_COLOR,POINT_COLOR,32,0);
		}
		Menu = MENU_SETPARAM;
		timeflag = 60;
		param_flash_flag = 0;
		break;
	case MENU_SETPARAM:
		if((Key_ScanNum > 0)&&(Key_ScanNum != 0xFF)){
			if(Key_ScanNum == 0x01){
					slaveaddrtemp1++;
					if(slaveaddrtemp1 > 50){
						slaveaddrtemp1 = 0;
					}
				}else if(Key_ScanNum == 0x11){
						if(slaveaddrtemp1 > 0){
							slaveaddrtemp1--;
						}else{
							slaveaddrtemp1 = 50;
						}
				}
			slavedisplay[0] = slaveaddrtemp1/100+ 0x30;
			slavedisplay[1] = slaveaddrtemp1%100/10 + 0x30;
			slavedisplay[2] = slaveaddrtemp1%10 + 0x30;
			slavedisplay[3] = '\0';
			Show_Str(32*4,32*3,7*32,(u8 *)slavedisplay,BACK_COLOR,POINT_COLOR,32,0);
			Key_ScanNum = 0;
			Menu_Exit_Time = MENU_EXIT_TIME;
		}
		if(timeflag == 0){
			if(param_flash_flag == 0){
				Show_Str(32*4,32*3,7*32,(u8 *)slavedisplay,BACK_COLOR,POINT_COLOR,32,0);
				param_flash_flag = 1;
			}else{
				Show_Str(32*4,32*3,7*32,"   ",BACK_COLOR,POINT_COLOR,32,0);
				param_flash_flag = 0;
			}
			 timeflag = 60;
		}
		if(Menu_Exit_Time == 0){//菜单设置无动作20秒后自动保存退出菜单
				Menu = MENU_SAVEPARAM;
		}
		break;
	case MENU_SAVEPARAM:
		 LCD_Clear(BLACK);
		 if(slaveaddrtemp1 != 0){
				AT24CXX_WriteOneByte(0x00,slaveaddrtemp1);AT24CXX_WriteOneByte(0x01,slaveaddrtemp1);
				if((slaveaddrtemp1 == AT24CXX_ReadOneByte(0x00))&&(slaveaddrtemp1 == AT24CXX_ReadOneByte(0x01))){
					slaveaddr=slaveaddrtemp1;					
					Show_Str(32*2,32*2,8*32,"地址设置完成",BACK_COLOR,POINT_COLOR,32,0);
					Show_Str(32,32*3,8*32,"修改后设备地址: ",BACK_COLOR,POINT_COLOR,32,0);
					Show_Str(32*4,32*4,8*32,(u8 *)slavedisplay,BACK_COLOR,POINT_COLOR,32,0);
					delay_ms(1500);
					delay_ms(1500);
					delay_ms(1500);
					LCD_Clear(BLACK);
					AdrrOK_Flag = 1;
				}else{
					Show_Str(32*1,32*2,8*32,"保存地址错误",BACK_COLOR,POINT_COLOR,32,0);
					Show_Str(32*1,32*3,8*32,"请重新设置地址",BACK_COLOR,POINT_COLOR,32,0);
					Show_Str(32*1,32*4,8*32,"如果错误一直出现",BACK_COLOR,POINT_COLOR,32,0);
					Show_Str(32*1,32*5,8*32,"请更换存储芯片",BACK_COLOR,POINT_COLOR,32,0);
				}
		}else{
			AT24CXX_WriteOneByte(0x00,slaveaddrtemp1);AT24CXX_WriteOneByte(0x01,slaveaddrtemp1);
			Show_Str(32*2,32*2,8*32,"地址设置错误",BACK_COLOR,POINT_COLOR,32,0);
			Show_Str(32*2,32*3,8*32,"当前地址:0",BACK_COLOR,POINT_COLOR,32,0);
			Show_Str(32*2,32*4,8*32,"该地址设备异常",BACK_COLOR,POINT_COLOR,32,0);
			Show_Str(32*2,32*5,8*32,"1<=地址<=128",BACK_COLOR,POINT_COLOR,32,0);
		}
		Menu = MENU_END;
		break;
	case MENU_END:Key_SetParamFlag = 0; Menu = MENU_RESERVE;break;
	}
	
}




