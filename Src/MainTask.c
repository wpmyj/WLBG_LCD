/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-03-30
  * 功    能: emwin各个功能的实现
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
#include "ff.h"
#include "lcd/bsp_lcd.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define Method3  

/* 私有变量 ------------------------------------------------------------------*/
extern FATFS fs;												/* FatFs文件系统对象 */
extern FIL file;												/* 文件对象 */
extern FRESULT f_res;                   /* 文件操作结果 */
extern UINT fnum;            					  /* 文件成功读写数量 */

#define XBF_KAITI24_ADDR       0     //字库存放在SPI_FLash地址
uint8_t tempbuf[256]={0};            /* 从SD卡读取数据的缓冲 */
#define XBF_KAITI24_SIZE    (1600)   // kaiti24.xbf字库文件存占用扇区数(每个扇区为4096个字节)

/* 扩展变量 ------------------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSong16;

/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 对话框信息
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "YSF1",             0,               0, 0,  320,480,0,0},
    { TEXT_CreateIndirect,      "硬石电子",        GUI_ID_TEXT0,     0, 30, 80, 20, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "硬石电子",        GUI_ID_TEXT1,     0, 120, 80, 120, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "硬石电子",        GUI_ID_TEXT2,     0, 240, 240, 240, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "硬石电子",        GUI_ID_TEXT3,     0, 340, 320, 320, 0, 0x0, 0},
};

/**
  * 函数功能: 重绘
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
}

/**
  * 函数功能: 对话框初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void InitDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN hWin = pMsg->hWin;

	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0), GUI_BLUE);
  TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_FontHZ12);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1), GUI_MAGENTA);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_FontHZ16);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT2), GUI_RED);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_FontHZ24);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT3), GUI_GREEN);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3),&GUI_FontHZ32);
}

/**
  * 函数功能: 对话框回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
		
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
		
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
			
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
					
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/**
  * 函数功能: 往SPIFlash写入字库文件
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void LoadFontLib(void)
{
  uint32_t j;
  uint32_t write_addr=0;

 /* 开背光 */
  LCD_BK_ON();	
	/* 清屏 */
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();

/* 第2步：打开文件 ***************************************************************/
	f_res = f_open(&file, "0:GBK.bin", FA_OPEN_EXISTING | FA_READ );
	if (f_res !=  FR_OK)
	{
		printf("font.bin文件打开失败");
	}
  write_addr=XBF_KAITI24_ADDR;
  for(j=0;j<XBF_KAITI24_SIZE;j++)//擦除扇区
  {
    SPI_FLASH_SectorErase(write_addr+j*4096);
  }
  
/* 第3步：复制SD卡中字库文件font.bin到SPI FLASH **********************************/
  write_addr=XBF_KAITI24_ADDR;
  while(f_res == FR_OK) 
  {
   /* 读取一个扇区的数据到tempbuf */
    f_res = f_read(&file, tempbuf, sizeof(tempbuf), &fnum);
    if(f_res!=FR_OK)break;			 //执行错误  
    /* 写数据到SPI FLASH */
    SPI_FLASH_BufferWrite(tempbuf, write_addr, 256);
    write_addr+=256;	
    j++;
    if(fnum !=256)break;    
  }
    f_close(&file);
      
	/* 等待1秒后开始进去emWin主界面 */
	GUI_Delay(1000);
}

/**
  * 函数功能: GUI功能函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void MainTask(void) 
{	
  /* 初始化 */
  GUI_Init();

  LCD_BK_ON();

  /* 初始化使用GUI以前，先将字库从SD卡中加载到SP FLASH里面 该函数调用一次即可，之后将其注释掉，因为字库已写入SPI-Flash */
  LoadFontLib();
  
  /* 创建对话框 */
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
  while(1)
	{	
		GUI_Delay(10);	
	}
  
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
