#ifndef __24CXX_H
#define __24CXX_H
#include "stm32f4xx_hal.h"

//如果移植程序时只要改一下三个地方就行了
/* 定时使用的IO口 */
#define I2C_SCL       GPIO_PIN_0	  
#define I2C_SDA       GPIO_PIN_1	  
#define GPIO_I2C      GPIOA
#define I2C_RCC				RCC_APB2Periph_GPIOA
// #define I2C_SCL       GPIO_Pin_6	  
// #define I2C_SDA       GPIO_Pin_7
// #define GPIO_I2C      GPIOB
// #define I2C_RCC				RCC_APB2Periph_GPIOB

#define I2C_SCL_H 	HAL_GPIO_WritePin(GPIO_I2C,I2C_SCL,GPIO_PIN_SET)
#define I2C_SCL_L 	HAL_GPIO_WritePin(GPIO_I2C,I2C_SCL,GPIO_PIN_RESET)

#define I2C_SDA_H 	HAL_GPIO_WritePin(GPIO_I2C,I2C_SDA,GPIO_PIN_SET)
#define I2C_SDA_L 	HAL_GPIO_WritePin(GPIO_I2C,I2C_SDA,GPIO_PIN_RESET)

#define AT24C01  127
#define AT24C02  255
#define AT24C04  511
#define AT24C08  1023
#define AT24C16  2047
#define AT24C32  4095
#define AT24C64  8191
#define AT24C128 16383
#define AT24C256 32767

#define EE_TYPE  AT24C02

void delay_nms(unsigned long nms);
/* 声明全局函数 */
void I2C_INIT(void);
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
unsigned char   I2C_Wait_Ack(void);
void I2C_Send_Byte(unsigned char txd);
unsigned char  I2C_Read_Byte(unsigned char ack);




/* 声明全局函数 */
void AT24CXX_Init(void);
unsigned char AT24CXX_ReadOneByte(unsigned short addr);
void AT24CXX_WriteOneByte(unsigned short addr,unsigned char dt);
unsigned char AT24CXX_Check(void);
void AT24CXX_WriteLenByte(unsigned short WriteAddr,unsigned long DataToWrite,unsigned char Len);
unsigned long AT24CXX_ReadLenByte(unsigned short ReadAddr,unsigned char Len);
void AT24CXX_Read(unsigned short ReadAddr,unsigned char *pBuffer,unsigned short NumToRead);
void AT24CXX_Write(unsigned short WriteAddr,unsigned char *pBuffer,unsigned short NumToWrite);
#endif
















