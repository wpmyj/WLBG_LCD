#include"HeadType.h"
#include "stm32f4xx_hal.h"

#define ENABLE_PWM 1

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
#if ENABLE_PWM == 0
TIM_HandleTypeDef htim3;
#else
TIM_HandleTypeDef  htim3;      	//定时器句柄 
TIM_OC_InitTypeDef htim3_CH4;	//定时器3通道1句柄
#endif
//=============================================================================
//函数名称:TIM2_Config
//功能概要:TIM2 定时器配置
//参数说明:无
//函数返回:无
//=============================================================================
void TIM2_Config(void )
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 5000;
  htim2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	
  HAL_TIM_Base_Start_IT(&htim2);
}
//=============================================================================
//函数名称:TIM3_Config
//功能概要:TIM3 定时器配置
//参数说明:无
//函数返回:无
//=============================================================================
#if ENABLE_PWM == 0
void TIM3_Config(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 5000;
  htim3.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
	
	HAL_TIM_Base_Start_IT(&htim3);
}
#else
//TIM14 PWM部分初始化 
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
    htim3.Instance=TIM3;          	//定时器3
    htim3.Init.Prescaler=psc;       //定时器分频
    htim3.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    htim3.Init.Period=arr;          //自动重装载值
    htim3.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);       //初始化PWM
    
    htim3_CH4.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    htim3_CH4.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    htim3_CH4.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&htim3,&htim3_CH4,TIM_CHANNEL_4);//配置TIM14通道4
	
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);//开启PWM通道4
}


//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
	GPIO_Initure.Pin=GPIO_PIN_1;           	//PB1
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;	//PF9复用为TIM14_CH1
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//设置TIM通道4的占空比
//compare:比较值
void TIM_SetTIM3Compare1(u32 compare)
{
	TIM3->CCR4=compare; 
}
#endif

//=============================================================================
//函数名称:TIM4_Config
//功能概要:TIM4 定时器配置
//参数说明:无
//函数返回:无
//=============================================================================
void TIM4_Config(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 72-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 5000;
  htim4.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim4);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);
	
	HAL_TIM_Base_Start_IT(&htim4);
}



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM2){

    __HAL_RCC_TIM2_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  }else if(htim_base->Instance==TIM3){
#if ENABLE_PWM == 0
    __HAL_RCC_TIM3_CLK_ENABLE();
		
    HAL_NVIC_SetPriority(TIM3_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
#endif
  }else if(htim_base->Instance==TIM4){

    __HAL_RCC_TIM4_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM4_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  }
}


void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM2){
    __HAL_RCC_TIM2_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  }else if(htim_base->Instance==TIM3){
		#if ENABLE_PWM == 0
    __HAL_RCC_TIM3_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
		#endif
  }else  if(htim_base->Instance==TIM4){
    __HAL_RCC_TIM4_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  }
} 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2){

  }else if(htim->Instance==TIM3){

  }else  if(htim->Instance==TIM4){

  }
}






