#include"HeadType.h"

//=============================================================================
//函数名称:TIM2_Interrupts_Config
//功能概要:TIM2 中断优先级配置
//参数说明:无
//函数返回:无
//=============================================================================
static void TIM2_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//函数名称:TIM3_Interrupts_Config
//功能概要:TIM3 中断优先级配置
//参数说明:无
//函数返回:无
//=============================================================================
static void TIM3_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//函数名称:TIM4_Interrupts_Config
//功能概要:TIM4 中断优先级配置
//参数说明:无
//函数返回:无
//=============================================================================
static void TIM4_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//函数名称:TIM2_Config
//功能概要:TIM2 定时器配置
//参数说明:无
//函数返回:无
//=============================================================================
void TIM2_Config(void )
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
		TIM_DeInit(TIM2);
		TIM_TimeBaseStructure.TIM_Period =4999; //5	MS;		 		
		TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);				  
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
			
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);						
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM2, ENABLE);	
		TIM2_Interrupts_Config();
}
//=============================================================================
//函数名称:TIM3_Config
//功能概要:TIM3 定时器配置
//参数说明:无
//函数返回:无
//=============================================================================
void TIM3_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
		TIM_DeInit(TIM3);
		TIM_TimeBaseStructure.TIM_Period = 1999;    //2MS
		TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM3, ENABLE );    
    
    TIM3_Interrupts_Config();
}

//=============================================================================
//函数名称:TIM4_Config
//功能概要:TIM4 定时器配置
//参数说明:无
//函数返回:无
//=============================================================================
void TIM4_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
		TIM_DeInit(TIM4);
		TIM_TimeBaseStructure.TIM_Period = 199;    //0.2MS
		TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM4, DISABLE );    //初始化不开启定时器
		//TIM_Cmd(TIM4, ENABLE ); 
		TIM4_Interrupts_Config();
}

