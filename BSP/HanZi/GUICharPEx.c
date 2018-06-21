/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-03-30
  * 功    能: 汉字显示方式-（任意大小全字库生成和使用方法，GB2312编码）
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
#include "MainTask.h"
#include "text.h"	
#include "GT60L16M2K4.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 点阵数据缓存, 必须大于等于单个字模需要的存储空间*/ 
#define BYTES_PER_FONT      512 

/* 私有变量 ------------------------------------------------------------------*/
static U8 GUI_FontDataBuf[BYTES_PER_FONT];

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 读取点阵数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c) 
{ 
  U16 BytesPerFont; 
  U32 oft = 0, BaseAdd; 
	U8 hzcode_high,hzcode_low;
	
	char *font = (char *)pProp->paCharInfo->pData; 

	/* 每个字模的数据字节数 */
    BytesPerFont = GUI_pContext->pAFont->YSize * pProp->paCharInfo->BytesPerLine; 
    if (BytesPerFont > BYTES_PER_FONT){
			BytesPerFont = BYTES_PER_FONT;
		}
		oft = oft;
	/* 英文字符地址偏移计算 */ 
    if (c < 0x80){ 
			if(strncmp("A12", font, 3) == 0){     /* 6*12 ASCII字符 */
// 				BaseAdd = 0x00000000;
				get_font(GUI_FontDataBuf,0x02,0x00,c,6,12,12);
			}else if(strncmp("A16", font, 3) == 0){ /* 8*16 ASCII字符 */
// 				BaseAdd = 0x00000C00;
				get_font(GUI_FontDataBuf,0x02,0x00,c,8,16,16);
			}else if(strncmp("A24", font, 3) == 0){ /* 12*24 ASCII字符 */
// 				BaseAdd = 0x00001C00;
				get_font(GUI_FontDataBuf,0x01,0x00,c,12,24,24);
			}else if(strncmp("A32", font, 3) == 0){ /* 24*48 ASCII字符 */
// 				BaseAdd = 0x00004C00;
				get_font(GUI_FontDataBuf,0x04,0x00,c,16,32,32);
			}
			
			oft = c* BytesPerFont + BaseAdd; 
    } 
	/* 汉字和全角字符的偏移地址计算 */
    else                                                                           
    { 
		hzcode_high = c & 0xFF;
		hzcode_low = c>> 8;
		if(strncmp("H12", font, 3) == 0)      /* 12*12 字符 */
		{
// 			BaseAdd = 0x00008C00;
			get_font(GUI_FontDataBuf,0x05,hzcode_high,hzcode_low,12,12,12);
		}
		else if(strncmp("H16", font, 3) == 0)  /* 16*16 字符 */
		{
// 			BaseAdd = 0x00095060;
			get_font(GUI_FontDataBuf,0x05,hzcode_high,hzcode_low,16,16,16);
		}
		else if(strncmp("H24", font, 3) == 0)  /* 24*24 字符 */
		{
// 			BaseAdd = 0x001500E0;
		}
		else if(strncmp("H32", font, 3) == 0)  /* 32*32 字符 */
		{
// 			BaseAdd = 0x002F4E00;
			get_font(GUI_FontDataBuf,0x05,hzcode_high,hzcode_low,32,32,32);
		}
		
		/* 根据汉字内码的计算公式锁定起始地址 */
// 		code2 = c >> 8;
// 		code1 = c & 0xFF;
		
		/* 由于字符编码是安顺序存储的，先存储到高位（区号），然后是低位（位号）。而我们用的是小端格式，
		   一个汉字两个字节，获取的16位变量，正好相反，16位变量的高位是位号，低位是区号。
		*/
// 		oft = ((code1 - 0x81) * 190 + (code2- 0x40) - (code2 / 128))* BytesPerFont + BaseAdd;
    }
// 	printf("oft=%x\n",oft);
	/* 读取点阵数据 */
// 	SPI_FLASH_BufferRead(GUI_FontDataBuf, oft, BytesPerFont);
} 


/**
  * 函数功能: 显示字符
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void GUIUSER_DispChar(U16P c)  
{ 
    int BytesPerLine; 
    GUI_DRAWMODE DrawMode = GUI_pContext->TextMode; 
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = GUI_pContext->pAFont->p.pProp; 
	
    /* 确定显示的字符是否在有效范围内 */  
    for (; pProp; pProp = pProp->pNext)                                          
    { 
        if ((c >= pProp->First) && (c <= pProp->Last))break; 
    } 
	/* 字符有效，进行显示 */
    if (pProp) 
    { 
        GUI_DRAWMODE OldDrawMode;
        const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);
        BytesPerLine = pCharInfo->BytesPerLine;                
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        LCD_DrawBitmap(GUI_pContext->DispPosX, GUI_pContext->DispPosY,
                       pCharInfo->XSize, GUI_pContext->pAFont->YSize,
                       GUI_pContext->pAFont->XMag, GUI_pContext->pAFont->YMag,
                       1,    
                       BytesPerLine,
                       &GUI_FontDataBuf[0],
                       &LCD_BKCOLORINDEX
                       );
		
        /* 填充背景 */
        if (GUI_pContext->pAFont->YDist > GUI_pContext->pAFont->YSize) 
        {
            int YMag = GUI_pContext->pAFont->YMag;
            int YDist = GUI_pContext->pAFont->YDist * YMag;
            int YSize = GUI_pContext->pAFont->YSize * YMag;
            if (DrawMode != LCD_DRAWMODE_TRANS) 
            {
                LCD_COLOR OldColor = GUI_GetColor();
                GUI_SetColor(GUI_GetBkColor());
                LCD_FillRect(GUI_pContext->DispPosX, GUI_pContext->DispPosY + YSize, 
                             GUI_pContext->DispPosX + pCharInfo->XSize, 
                             GUI_pContext->DispPosY + YDist);
                GUI_SetColor(OldColor);
            }
        }
		
        LCD_SetDrawMode(OldDrawMode);
//      if (!GUI_MoveRTL)
        GUI_pContext->DispPosX += pCharInfo->XDist * GUI_pContext->pAFont->XMag;
    }
} 

/**
  * 函数功能: 获取字符的X轴间距
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int GUIUSER_GetCharDistX(U16P c, int * pSizeX) 
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUI_pContext->pAFont->p.pProp;  
    for (; pProp; pProp = pProp->pNext)                                         
    {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }
    return (pProp) ? (pProp->paCharInfo)->XSize * GUI_pContext->pAFont->XMag : 0;
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
