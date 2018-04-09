//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖÐ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼þ Ãû   : LCD_ZK.H
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : HuangKai
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üÐÞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : ×Ö¿â°æ OLED SPI½Ó¿ÚÑÝÊ¾Àý³Ì(STM32ÏµÁÐ)
//              ËµÃ÷: 
//              ------------------------ÒÔÏÂÎªOLEDÏÔÊ¾ËùÓÃµ½µÄ½Ó¿----------------------------------------
//              GND    µçÔ´µØ
//              VCC   ½Ó5V»ò3.3vµçÔ´
//              CLK   PA5£¨CLK£©
//              MOSI   PA7£¨DIN£©
//              DC   PB0
//              CS1   PB1
//              FSO   PA4                
//              CS2   PB10
//
// ÐÞ¸ÄÀúÊ·   :
// ÈÕ    ÆÚ   : 
// ×÷    Õß   : HuangKai
// ÐÞ¸ÄÄÚÈÝ   : ´´½¨ÎÄ¼þ
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÖÐ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//******************************************************************************/
#define _LCD_ZK_H_
#ifdef _LCD_ZK_H_



/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1


#define		LCD_SCLK_IO					GPIO_Pin_13					 //SCLK  Ê±ÖÓ D0£¨SCLK£©
#define		LCD_SCLK_PORT				GPIOB
#define   LCD_SCLK_RCC				RCC_APB2Periph_GPIOB
#define		LCD_SDA_IO					GPIO_Pin_15					 //SDA   D1£¨MOSI£© Êý¾Ý
#define		LCD_SDA_PORT				GPIOB
#define   LCD_SDA_RCC					RCC_APB2Periph_GPIOB
#define		LCD_RST_IO					GPIO_Pin_10					//_RES  hardware reset   ¸´Î» 
#define		LCD_RST_PORT				GPIOB
#define   LCD_RST_RCC					RCC_APB2Periph_GPIOB
#define		LCD_DC_IO						GPIO_Pin_11				 //A0  H/L ÃüÁîÊý¾ÝÑ¡Í¨¶Ë£¬H£ºÊý¾Ý£¬L:ÃüÁî
#define		LCD_DC_PORT					GPIOB
#define   LCD_DC_RCC					RCC_APB2Periph_GPIOB
#define		LCD_CS_IO						GPIO_Pin_12				 //OLEDÆ¬Ñ¡ÐÅºÅ
#define		LCD_CS_PORT					GPIOB
#define   LCD_CS_RCC					RCC_APB2Periph_GPIOB

#define		ROM_CS_IO						GPIO_Pin_5				 //romÆ¬Ñ¡ÐÅºÅ
#define		ROM_CS_PORT					GPIOA
#define   ROM_CS_RCC					RCC_APB2Periph_GPIOA
#define		ROM_OUT_IO					GPIO_Pin_4				 //
#define		ROM_OUT_PORT				GPIOA
#define   ROM_OUT_RCC					RCC_APB2Periph_GPIOA

#define   LCD_SCLK(N) 	 			{if(N==1){GPIO_SetBits(LCD_SCLK_PORT, LCD_SCLK_IO);}else{GPIO_ResetBits(LCD_SCLK_PORT, LCD_SCLK_IO);}}
#define   LCD_SDA(N) 	 				{if(N==1){GPIO_SetBits(LCD_SDA_PORT, LCD_SDA_IO);}else{GPIO_ResetBits(LCD_SDA_PORT, LCD_SDA_IO);}}
#define   LCD_RST(N) 	 				{if(N==1){GPIO_SetBits(LCD_RST_PORT, LCD_RST_IO);}else{GPIO_ResetBits(LCD_RST_PORT, LCD_RST_IO);}}
#define   LCD_DC(N) 	 				{if(N==1){GPIO_SetBits(LCD_DC_PORT, LCD_DC_IO);}else{GPIO_ResetBits(LCD_DC_PORT, LCD_DC_IO);}}
#define   ROM_CS(N) 	 				{if(N==1){GPIO_SetBits(ROM_CS_PORT, ROM_CS_IO);}else{GPIO_ResetBits(ROM_CS_PORT, ROM_CS_IO);}}
#define   ROM_OUTPUT(N) 	 		{if(N==1){GPIO_SetBits(ROM_OUT_PORT, ROM_OUT_IO);}else{GPIO_ResetBits(ROM_OUT_PORT, ROM_OUT_IO);}}

#if 1

//´ø²Îºê£¬¿ÉÒÔÏñÄÚÁªº¯ÊýÒ»ÑùÊ¹ÓÃ
#define lcd_cs1(a)	if (a)	\
					GPIO_SetBits(LCD_CS_PORT,LCD_CS_IO);\
					else		\
					GPIO_ResetBits(LCD_CS_PORT,LCD_CS_IO)

//#define lcd_rs(a)	if (a)	\
//					GPIO_SetBits(LCD_DC_PORT,LCD_DC_IO);\
//					else		\
//					GPIO_ResetBits(LCD_DC_PORT,LCD_DC_IO)
#define lcd_rs(a)  LCD_DC(a)

//#define lcd_sid(a)	if (a)	\
//					GPIO_SetBits(LCD_SDA_PORT,LCD_SDA_IO);\
//					else		\
//					GPIO_ResetBits(LCD_SDA_PORT,LCD_SDA_IO)
#define lcd_sid(a) LCD_SDA(a) 
					
//#define lcd_sclk(a)	if (a)	\
//					GPIO_SetBits(LCD_SCLK_PORT,LCD_SCLK_IO);\
//					else		\
//					GPIO_ResetBits(LCD_SCLK_PORT,LCD_SCLK_IO)
#define lcd_sclk(a) LCD_SCLK(a)		
					
//#define Rom_CS(a)	if (a)	\
//					GPIO_SetBits(ROM_CS_PORT,ROM_CS_IO);\
//					else		\
//					GPIO_ResetBits(ROM_CS_PORT,ROM_CS_IO)
#define Rom_CS(a) ROM_CS(a)

//#define Rom_OUT(a)	if (a)	\
//					GPIO_SetBits(ROM_OUT_PORT,ROM_OUT_IO);\
//					else		\
//					GPIO_ResetBits(ROM_OUT_PORT,ROM_OUT_IO)				
#define Rom_OUT(a)  ROM_OUTPUT(a)

#define ROM_OUT    GPIO_ReadInputDataBit(ROM_OUT_PORT,ROM_OUT_IO)
					
#else
//´ø²Îºê£¬¿ÉÒÔÏñÄÚÁªº¯ÊýÒ»ÑùÊ¹ÓÃ
#define lcd_cs1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define lcd_rs(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)


#define lcd_sid(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_7);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define lcd_sclk(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define Rom_CS(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_10);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define Rom_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_4)				

#define ROM_OUT    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#endif						
					
extern unsigned char const bmp1[];
extern unsigned char const bmp2[];
extern unsigned char const bmp3[];
extern unsigned char const jiong1[];
extern unsigned char const lei1[];


/*Ð´Ö¸Áîµ½LCDÄ£¿é*/
void transfer_command_lcd(int data1);   
/*Ð´Êý¾Ýµ½LCDÄ£¿é*/
void transfer_data_lcd(int data1);
/*ÑÓÊ±*/
void delay(int n_ms);               
/*LCDÄ£¿é³õÊ¼»¯*/
void initial_lcd(void);
void lcd_address(unsigned char page,unsigned char column);
/*È«ÆÁÇåÆÁ*/
//void clear_screen(void);
void clear_screen(u8 dat);
/*ÏÔÊ¾128x64µãÕóÍ¼Ïñ*/
void display_128x64(const unsigned char *dp);
/*ÏÔÊ¾132x64µãÕóÍ¼Ïñ*/
//void display_132x64(unsigned char *dp);
/*ÏÔÊ¾16x16µãÕóÍ¼Ïñ¡¢ºº×Ö¡¢ÉúÆ§×Ö»ò16x16µãÕóµÄÆäËûÍ¼±ê*/
void display_graphic_16x16(unsigned int page,unsigned int column,const unsigned char *dp);
/*ÏÔÊ¾8x16µãÕóÍ¼Ïñ¡¢ASCII, »ò8x16µãÕóµÄ×ÔÔì×Ö·û¡¢ÆäËûÍ¼±ê*/
void display_graphic_8x16(unsigned int page,unsigned char column,unsigned char *dp);
/*ÏÔÊ¾5*7µãÕóÍ¼Ïñ¡¢ASCII, »ò5x7µãÕóµÄ×ÔÔì×Ö·û¡¢ÆäËûÍ¼±ê*/
void display_graphic_5x7(unsigned int page,unsigned char column,unsigned char *dp);
/****ËÍÖ¸Áîµ½¾§ÁªÑ¶×Ö¿âIC***/
void send_command_to_ROM( unsigned char datu );
/****´Ó¾§ÁªÑ¶×Ö¿âICÖÐÈ¡ºº×Ö»ò×Ö·ûÊý¾Ý£¨1¸ö×Ö½Ú£©***/
static unsigned char get_data_from_ROM(void);
/*´ÓÏà¹ØµØÖ·£¨addrHigh£ºµØÖ·¸ß×Ö½Ú,addrMid£ºµØÖ·ÖÐ×Ö½Ú,addrLow£ºµØÖ·µÍ×Ö½Ú£©ÖÐÁ¬Ðø¶Á³öDataLen¸ö×Ö½ÚµÄÊý¾Ýµ½ pBuffµÄµØÖ·*/
/*Á¬Ðø¶ÁÈ¡*/
void get_n_bytes_data_from_ROM(unsigned char addrHigh,unsigned char addrMid,unsigned char addrLow,unsigned char *pBuff,unsigned char DataLen );
/******************************************************************/
void display_GB2312_string(unsigned char y,unsigned char x,unsigned char *text);
void display_string_5x7(unsigned char y,unsigned char x,unsigned char *text);
void LCD_P8x16Showchar(unsigned char x, unsigned char y,unsigned char dat);
void LCD_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
void display_bmp(u8 x,u8 y,u8 size_x,u8 size_y,const unsigned char *dp);
void Gui_Battery(u8 x,u8 y,u8 bat);


#endif
