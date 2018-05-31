#ifndef __LCD_H
#define __LCD_H		 
#include "stdlib.h"
#include"HeadType.h"
#include "GT60L16M2K4.h"

#define 	GT30L32S4W  		 5
#define 	GT60L16M2K4			 10
#define 	GTXXX			       GT30L32S4W
//#define RST (1<<7) 			// PB7
//#define WR (1<<5)  			// PA3
//#define RD (1<<4)  			// PA2
//#define RS (1<<6)   			// PA4
//#define CS (1<<7)   			// PA5

//#define RST_L GPIOB->ODR &= ~RST
//#define RST_H GPIOB->ODR |= RST
//					  
//#define NWR_L GPIOA->ODR &= ~WR
//#define NWR_H GPIOA->ODR |= WR
			  
//#define NRD_L GPIOA->ODR &= ~RD
//#define NRD_H GPIOA->ODR |= RD

//#define NRS_L GPIOA->ODR &= ~RS 
//#define NRS_H GPIOA->ODR |= RS

//#define NCS_L GPIOA->ODR &= ~CS 
//#define NCS_H GPIOA->ODR |= CS

#define RST_L 	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)
#define RST_H 	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)
					  
#define NWR_L 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)
#define NWR_H 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
					  
#define NRD_L 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
#define NRD_H 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
		
#define NRS_L 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)
#define NRS_H 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)

#define NCS_L 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)
#define NCS_H 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)
//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD端口定义---------------- 
#define	LCD_LED PBout(0) //LCD背光  PB0 	    
//LCD地址结构体
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 			    
#define LCD1_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD1             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

//#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向
#define DFT_SCAN_DIR  U2D_L2R  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
//#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void delay_ms(u16 nms);
void LCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(u16 Color);	 												//清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//设置光标
void LCD_DrawPoint(u16 x,u16 y);											//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//快速画点
u16  LCD_ReadPoint(u16 x,u16 y); 											//读点 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 y,u16 x,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 y,u16 x,u16 width,u8 *str,u8 size,u8 mode);		//显示一个字符串,12/16字体

void LCD_WR_REG(u16 regval);
void LCD_WR_DATA(u16 data);
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_SSD_BackLightSet(u8 pwm);							//SSD1963 背光控制
void LCD_Scan_Dir(u8 dir);									//设置屏扫描方向
void LCD_Display_Dir(u8 dir);								//设置屏幕显示方向
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//设置窗口					   						   																			 
//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		210		//水平前廊
#define SSD_HOR_FRONT_PORCH		45		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		34		//垂直前廊
#define SSD_VER_FRONT_PORCH		10		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_PULSE_WIDTH+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_PULSE_WIDTH+SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_PULSE_WIDTH+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH+SSD_VER_RESOLUTION)
#define SSD_VSP (SSD_VER_PULSE_WIDTH+SSD_VER_BACK_PORCH)

#endif  
	 
	 



