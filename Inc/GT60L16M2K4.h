#ifndef _GT60M2_H
#define _GT60M2_H



#define  u8  unsigned  char
#define  u16 unsigned  int
#define  u32 unsigned  long
#define ASCII_16X16_T      1
#define ASCII_16X16_A      2
#define ASCII_24X24_T      3 
#define ASCII_24X24_A      4 
#define ASCII_32X32_T      5
#define ASCII_32X32_A      6

#define cr_zf_8X16         1
#define cr_zf_16X16_T      2
#define cr_zf_16X16_A      3
#define cr_zf_8X16_F      4

#define cr_zf_24X24_T      5
#define cr_zf_24X24_A      6
#define cr_zf_16X24_F      7
#define cr_zf_24X24_S      8


#define cr_zf_24X32_T      9
#define cr_zf_24X32_A      10
#define cr_zf_16X32_F      11
#define cr_zf_24X32_S      12


#define cr_zf_40X48_T      13
#define cr_zf_40X48_A      14
#define cr_zf_24X48_F      15
#define cr_zf_40X48_S      16


#define cr_zf_40X64_T      17
#define cr_zf_40X64_A      18
#define cr_zf_32X64_F      19
#define cr_zf_48X64_S      20


#define cr_zf_32X32_TB     21


void get_font(unsigned char *pBits,unsigned char sty,unsigned char msb,unsigned char lsb,unsigned char len,unsigned char wth,unsigned char thick);
extern unsigned char pBits[192*192/8];    /* 192X192*///点阵缓冲区

extern unsigned char pram[6];  //改函数无用

void hzbmp16X16( unsigned int code, unsigned int codeex, unsigned char size,unsigned char *buf);//换取16X16点GB18030汉字

unsigned char  ASCII_GetData_AT(unsigned char ASCIICode,unsigned long BaseAdd,unsigned char*DZ_Data);//换取不等宽的ASCII字符

unsigned int U2GB18030(unsigned int UN_CODE);//unicode 转GB18030的函数

unsigned int BIG52GBK( unsigned char h,unsigned char l);//BIG5转GBK的函数

void Gray_Process(unsigned char *OutPutData ,int width,int High,unsigned char Grade);

unsigned int italic(unsigned char *pInData,unsigned char *pOutData,unsigned char width,unsigned char high);

void font_hollow(unsigned char *pInData,unsigned char *pOutData,unsigned char high,unsigned char width);

void rgb24_to_rgb565(unsigned char *rgb24,unsigned char *rgb16,unsigned int len); 


void get_cr_zf(unsigned char Sequence,unsigned long BaseAdd,unsigned char*DZ_Data);

#endif
