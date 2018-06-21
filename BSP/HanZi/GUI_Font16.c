/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-03-30
  * 功    能: 16点阵字体
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F4Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "GUI.h"
#include "GUI_Type.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ16_CharInfo[2] =  
{     
    {  8,    8,  1, (void *)"A16"},      
    {  16,  16,  2, (void *)"H16"},           
}; 

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ16_PropHZ= { 
      0x4081,  
      0xFEFE, 
      &GUI_FontHZ16_CharInfo[1], 
      (void *)0,  
}; 

GUI_CONST_STORAGE  GUI_FONT_PROP GUI_FontHZ16_PropASC= { 
      0x0000,  
      0x007F,  
      &GUI_FontHZ16_CharInfo[0], 
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropHZ,  
}; 

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16 =  
{ 
	  GUI_FONTTYPE_USER,
      16,  
      16,  
      1,   
      1,   
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropASC,
}; 
GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16x2 =  
{ 
	  GUI_FONTTYPE_USER,
      16,  
      16,  
      2,   
      2,   
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropASC 
};

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

