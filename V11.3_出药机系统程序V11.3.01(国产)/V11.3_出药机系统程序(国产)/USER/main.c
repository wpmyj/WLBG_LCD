/**********************************************
名称：出药机系统控制程序(国产机)   适用电路板：Drug Delivery CON_V11.32 PCB
功能：1、控制伺服电机运动 (左右运行)
			2、控制直流电机正反转
			3、检测光电传感器信号
			
原始版本号：V11.3.00   创建日期：2017-02-13		完成日期：2017-12-12       用时： 10个月

历史修改版本号： 无    修改日期：
修改原因：1、

最新版本号: V11.3.01    修改日期：2018-04-20  完成日期：
修改原因：1、增加机构初始化检测是否完成

***********************************************/
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "define.h"
#include "declare.h"

TEST volatile Test;

int main(void)
{		
	
	delay_init();	    	 		//延时函数初始化	  
	NVIC_Configuration(); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	PORT_IO_Init();					//端口初始化
	Disable_SON();
	DialSwitch_Init();			//拨码开关端口初始化
	uart_init(9600);	 			//串口初始化为9600 
  TIM2PWM_Set(20*1000,SERVO_DISABLE);  //所有伺服电机PWM驱动初始化，初始化频率 = 20KHz,(频率，占空比)
 	MYDMA_Config(DMA_UART1_TX,0,(u32)&USART1->DR,(u32)DMA_SEND_BUFF,16);	//DMA1通道4,外设为串口1发送   	 
 	MYDMA_Config(DMA_UART1_RX,1,(u32)&USART1->DR,(u32)DMA_REV_BUFF,65535);//DMA1通道5,外设为串口1接收   	 
	DMA_INIT();//DMA 中断初始化
	Slave_ID = SW_ID();
	TIM7_Int_Init(10000,71);	//10ms计时
  Coder_InCap_Tim3(10000,71);  //编码器反馈脉冲捕获初始化
	TEST_LED = 1;					//测试灯灭

	delay_ms(1000);		
	delay_ms(1000);			
	delay_ms(1000);		
	delay_ms(1000);			
	
	delay_ms(500);		

	Enable_SON();				//开伺服使能	
	delay_ms(110);		
//	DOOR_Check();							//安全门报警检测
	AllSystem_Init();     //使所有机构初始化
	WORK_LED_CNT = TIMER7_CNT + 50;		//500ms word_led闪烁用

///////////////测试程序//////////////
//	Gate_Opening();							//闸门直流电机正转
//	SERVO_REVRun(1);   				  //右提升机向下运行 25r/min											
/////////////////////////////////////////////

 	while(1)
	{	
///////////测试药斗////////
//			Dou_Closed();				
//			delay_ms(1000);		
//			delay_ms(1000);		
//			Dou_Opened();		
//			delay_ms(1000);		
//			delay_ms(1000);		
//////////////////////////////////////		

////////测试闸门////////
//			Gate_Closed();				
//			delay_ms(1000);		
//			delay_ms(1000);		
//			Gate_Opened();		
//			delay_ms(1000);		
//			delay_ms(1000);		
///////////////////////////////////
		
		ButtonScan();             //按键扫描，测试程序
		UART1_Com_Check();				//UART1 通讯处理程序
		AD_Check();								//从机地址AD检测处理程序
//		DOOR_Check();							//安全门报警检测
		ALM_Check();							//伺服电机报警检测
	}	 
}

