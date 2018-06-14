#include "stm32f10x.h"
#include "sys.h"
#include "declare.h"
#include "delay.h"
								    
//按键初始化函数
void DialSwitch_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->GPIOA.13,KEY1-->GPIOA.15  上拉输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//使能PA端口时钟

	//拨码开关输入端口	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;    //PA11-CODER1, PA12-CODER2										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//根据参数初始化GPIOA
}

//拨码开关处理函数
//返回拨码开关值
//SLAVE ID低8位
u8 DialSwitch_Scan_gewei(void)
{	 
	u8 id;
	
	id = ((Two&0x1)<<1) | (One&0x1);
	
	return id;
}

//返回SLAVE ID
u8 SW_ID(void)
{
		unsigned char Adress_ID;

		Adress_ID = DialSwitch_Scan_gewei();
		return Adress_ID;
}
