#include "beep.h"
#include"HeadType.h"

u16 Beep_Time;
u8 Beep_Num;
u8 ERR_Beep_State;
//=============================================================================
//函数名称:BEEP_GPIO_Config
//功能概要:蜂鸣器引脚配置
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
void BEEP_GPIO_Config(void)
{	
	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//使能GPIOC的外设时钟
	RCC_APB2PeriphClockCmd(BEEP_RCC,ENABLE);
	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = BEEP_IO;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//调用库函数，初始化GPIO
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
	BEEP_OFF;
	ERR_Beep_State = 0;
	Beep_Num = 0;
}

//=============================================================================
//函数名称:Beep_Response
//功能概要:蜂鸣器响，使用定时器来实现
//参数名称:无
//函数返回:无
//注意    :必须在定时器调用
//=============================================================================

void Beep_Response(void )
{
	static u8 beep_state=1;
	static u8 retry = 50; //led闪烁时间间隔
	if(Beep_Num > 0){
		retry--;
		if(retry <=0){
			if(beep_state){
			   BEEP_ON;
			   beep_state = 0;
			}else{
			   BEEP_OFF;
			   Beep_Num--;
			   beep_state = 1;
			}
			retry = 50;
		}
	}
	
}
void Beep_Sound(u8 num,u16 ms_time)
{
   u8 i;
  for(i=0;i<num;i++){
    BEEP_ON;
    delay_ms(ms_time);
    BEEP_OFF;
    delay_ms(ms_time);
  }
  
}

