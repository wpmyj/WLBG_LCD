#include"HeadType.h"

#define LED_LIGHT_TIMEOUT  100			//N*5MS
u8 Gled_Num;
u8 Rled_Num;
u8 Gled_Light_Static;
u8 Rled_Light_Static;
//=============================================================================
//函数名称: LED_GPIO_Config
//功能概要:LED灯引脚配置
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
void LED_GPIO_Config(void)
{	
	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//使能GPIOC的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//调用库函数，初始化GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GLED_OFF;
	RLED_OFF;
	Gled_Num = 1;
	Rled_Num = 0;
}

//=============================================================================
//函数名称:Gled_Flash
//功能概要:绿色LED闪烁函数
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
static void Gled_Flash(void )
{
	static u8 retry = 50; //led闪烁时间间隔
	static u8 led_state=1;
	if(Gled_Num > 0){
		retry--;
		if(retry <=0){
			if(led_state){
			   GLED_ON;
			   led_state = 0;
			}else{
			   GLED_OFF;
//			   Gled_Num--;
			   led_state = 1;
			}
			retry = 50;
		}
	}	
}
//=============================================================================
//函数名称:Rled_Flash
//功能概要:红色LED闪烁函数
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
static void Rled_Flash(void )
{
	static u8 retry = 50; //led闪烁时间间隔
	static u8 led_state=1;
	if(Rled_Num > 0){
		retry--;
		if(retry <=0){
			if(led_state){
			   RLED_ON;
			   led_state = 0;
			}else{
			   RLED_OFF;
//			   Rled_Num--;
			   led_state = 1;
			}
			retry = 50;
		}
	}	
}
//=============================================================================
//函数名称:Led_Flash
//功能概要:LED闪烁函数
//参数名称:无
//函数返回:无
//注意    :此函数需要在5MS定时器2中断中调用
//=============================================================================
void Led_Flash(void)
{
	static u16 gled_light_time = 0;
	static u16 rled_light_time = 0;
	Gled_Flash();
	Rled_Flash();
	if(Gled_Light_Static == 1){
		gled_light_time++;
		if(gled_light_time >= LED_LIGHT_TIMEOUT ){
			GLED_OFF;
			Gled_Light_Static = 0;
			gled_light_time = 0;
		}
	}
	if(Rled_Light_Static == 1){
		rled_light_time++;
		if(rled_light_time >= LED_LIGHT_TIMEOUT ){
			RLED_OFF;
			Rled_Light_Static = 0;
			rled_light_time = 0;
		}
	}
}





