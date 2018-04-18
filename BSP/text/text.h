#ifndef __TEXT_H__
#define __TEXT_H__	 
#include "stm32f1xx_hal.h"
#include"HeadType.h"

#define ROM_IN_Clr() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET)
#define ROM_IN_Set() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET)

#define ROM_SCK_Clr() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET)
#define ROM_SCK_Set() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET)

#define ROM_CS_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET)
#define ROM_CS_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET)

#define READ_ROM_OUT()  HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0) 
 		

void ROM_GT30L_Init(void);
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//得到汉字的点阵码
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//在指定位置显示一个汉字
void Show_Str(u16 y,u16 x,u16 width,u8*str,u8 size,u8 mode);	//在指定位置显示一个字符串 
void Show_Str_Mid(u16 y,u16 x,u8*str,u8 size,u16 len);


//字库函数
void send_command_to_ZK(u8 dat);
unsigned char ZK_read(void);
void ZK_Read_1_n(unsigned char addrHigh,unsigned char addrMid,unsigned char addrLow,unsigned char *pBuff,uchar DataLen );
void Display_GB2312_String(unsigned char zk_num,unsigned int  x,unsigned int  y, unsigned char  text[]);
void Display_GB2312( unsigned char zk_num,unsigned int x, unsigned int y );
void Display_GBasc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[]);
void Display_GBasc( unsigned char zk_num,unsigned int x, unsigned int y );
void Display_Asc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[]);
void Display_Asc( unsigned char zk_num,unsigned int x, unsigned int y );

void Set_Addressing_Mode(unsigned char d) ;
void Set_Segment_Remap(unsigned char d)	;
void Set_Common_Remap(unsigned char d)	;
void Set_Common_Config(unsigned char d)	;
void Set_Contrast_Control(unsigned char d);
void Set_Entire_Display(unsigned char d);
void Set_Inverse_Display(unsigned char d) ;
void zk_map(  unsigned char *getdate, unsigned char *putdata, unsigned char high, unsigned char width);
void Set_Pos(unsigned char x, unsigned char y);
void Set_Start_Page(unsigned char d);
void Set_Start_Column(unsigned char d);
void Show_Ascchar(u16 y,u16 x,u8 Ascchar,u8 size,u8 mode);

#endif




