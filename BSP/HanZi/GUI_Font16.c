/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: 16��������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "GUI.h"
#include "GUI_Type.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
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

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
