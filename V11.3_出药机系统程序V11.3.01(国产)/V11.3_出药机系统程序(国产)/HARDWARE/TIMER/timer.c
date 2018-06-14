#include "timer.h"
#include "stm32f10x_tim.h"
#include "declare.h"

//通用定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM7_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能
	
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM7中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM7, ENABLE);  //使能TIMx					 
}

//定时器7中断服务程序
void TIM7_IRQHandler(void)   //TIM7中断
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM7更新中断发生与否
	{
		TIMER7_CNT++;						//10ms一个计数
		if(WORK_LED_CNT == TIMER7_CNT)
		{
			WORK_LED = !WORK_LED;
			WORK_LED_CNT = TIMER7_CNT + 50;
		}

		if(Test.LEDEnable == ENABLE)   //如果开始进入测试程序，则闪烁测试指示灯
		{
			if(Test.LEDTime == TIMER7_CNT)
			{
				TEST_LED 		 = !TEST_LED;
				Test.LEDTime = TIMER7_CNT + 50;		//500ms test_led闪烁用
			}
		}
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIMx更新中断标志 
	}
}

//使用TIM2定时器产生PWM初始化
//f:PWM波输出频率；duty:占空比
void TIM2PWM_Set(u32 f, u8 duty)
{	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			//打开定时器时钟
	
	//定时器2设置
	TIM_TimeBaseStructure.TIM_Period = 99;									//20KHz,16位的值，最大65536
	TIM_TimeBaseStructure.TIM_Prescaler = (72000000 / (100 * f)) - 1;		//16位的值，最大65536
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;					//时钟分频系数，不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数溢出模式
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);					//初始化定时器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//主定时器TIM2为PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能输出比较状态
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	//ch1配置项
	TIM_OCInitStructure.TIM_Pulse = duty;										//ch1占空比50%
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);									
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);				//使能的预装载寄存器
		
	TIM_OCInitStructure.TIM_Pulse = duty;
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);									
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);				//使能的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);											//使能TIM2在ARR上的预装载寄存器
	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);										//开定时器中断
	TIM_CtrlPWMOutputs(TIM2,ENABLE);												//使能TIM2的外设的主输出
	TIM_Cmd(TIM2,ENABLE);																		//使能定时器2
}

//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 
		
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//捕获1发生捕获事件 PA6
	{
		//脉冲计数
		FeedbackNowNum ++;
		if(FeedbackNowNum == FeedbackHopeNum)
		{//编码器实际反馈脉冲值 = 编码器期望反馈脉冲值
			PulseCntComplete = 1;							 //反馈脉冲计数完成
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 		//清除中断标志位
	}
	
}

//伺服电机编码器反馈 脉冲输入捕捉初始化
void Coder_InCap_Tim3(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;				//声明结构体GPIO_InitStructure	
	TIM_ICInitTypeDef  TIM3_ICInitStructure;								//通道输入初始化结构
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;					//定时器初始化结构
  NVIC_InitTypeDef NVIC_InitStructure;										//NVIC中断寄存器组初始化结构定义

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//TIM3时钟开启
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//使能PA端口时钟
	
	//PA6-Feedback_plus(提升机伺服电机脉冲反馈信号)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//根据参数初始化GPIOA
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
	//TIM3输出初始化	 
	TIM_TimeBaseStructure.TIM_Period 				= arr; //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler 		=	psc; 	//预分频器,1M的计数频率,1us加1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	//TIM3通道的捕捉初始化
	TIM3_ICInitStructure.TIM_Channel 			= TIM_Channel_1; 					//CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection 	= TIM_ICSelection_DirectTI;  //管脚与寄存器对应关系
	TIM3_ICInitStructure.TIM_ICPrescaler 	= TIM_ICPSC_DIV1;	 				//配置输入分频,不分频 
	TIM3_ICInitStructure.TIM_ICFilter 		= 0x00;										//IC1F=0000 配置输入滤波器 不滤波 经历几个周期跳变认定波形稳定 0x0~0xf
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);												//初始化

//	TIM3_ICInitStructure.TIM_Channel 			= TIM_Channel_2; 					//选择输入端 IC2映射到TI2上
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器3
	TIM_ITConfig( TIM3,TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
}

