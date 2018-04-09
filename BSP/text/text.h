#ifndef __TEXT_H__
#define __TEXT_H__	 
#include <stm32f10x.h>

#define ROM_IN_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define ROM_IN_Set() GPIO_SetBits(GPIOC,GPIO_Pin_2)

#define ROM_SCK_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define ROM_SCK_Set() GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define ROM_CS_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define ROM_CS_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define READ_ROM_OUT()  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0) 
 		

void ROM_GT30L_Init(void);
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//得到汉字的点阵码
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//在指定位置显示一个汉字
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	//在指定位置显示一个字符串 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);


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

#endif




