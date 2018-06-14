#include "stm32f10x.h"
#include "sys.h"
#include "stm32f10x_dma.h"
#include "declare.h"
#include "stm32f10x_tim.h"

//工作指示灯、伺服电机驱动器控制端、直流电机控制端、各光电传感器端口初始化
void PORT_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				//声明结构体GPIO_InitStructure
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);			//使能PA, PB,PC端口时钟

	//****************************************伺服电机驱动器控制端口初始化**************************************
	//////PA0-PLUS(伺服驱动器脉冲)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//根据参数初始化GPIOA
	
	//////PA1-SON(伺服驱动器使能), PA2-CLR(伺服驱动器报警清除)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					//速度为2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//根据参数初始化GPIOA

	//////PB11-SIGN(伺服驱动器方向), PB9-TEST_LED(测试指示灯)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					//速度为2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//根据参数初始化GPIOB
	
	//////PB12-ALM(伺服电机驱动器报警信号), PB13-EDM(伺服电机安全急停信号)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//根据参数初始化GPIOB
	
	//**********************************************直流电机端口初始化***********************************************
	//////PB14-Dou_A(药斗直流电机A), PB15-Dou_B(药斗直流电机B)
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					//速度为2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//根据参数初始化GPIOB

	//////PC6-Gate_A(闸门直流电机A), PC7-Gate_B(闸门直流电机B),PC13-Working(工作指示灯)
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					//速度为2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);										//根据参数初始化GPIOC

	//************************************光电传感器端口初始化(光电传感器信号作为输入信号)***************************
	////// PA3-RST(提升机复位信号), PA4-Top(提升机顶端限位信号), PA5-GS_CLOSE(闸门关闭信号), PA7-GS_OPEN(闸门打开信号), 
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;							//下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//根据参数初始化GPIOA

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);										//根据参数初始化GPIOA

//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//上拉输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);										//根据参数初始化GPIOA

////// PB0-DS_OPEN(药斗打开传感器信号)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;							//下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//根据参数初始化GPIOB

	////// PC4-Opposite(安全对射传感器信号),PC5-DS_CLOSE(药斗关闭信号),PC10-BUTTON(测试按键)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;							//下拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);										//根据参数初始化GPIOC
                                                                                                                                                                                                                                               
}

//闸门正在打开
void Gate_Opening(void)
{
	Gate_A = 1;			//闸门直流电机正转
	Gate_B = 0;	
}

//闸门正在关闭
void Gate_Closing(void)
{
	Gate_A = 0;			//闸门直流电机反转
	Gate_B = 1;	
}

//闸门停止
void Gate_Stop(void)
{
	Gate_A = 1;			//阀门直流电机
	Gate_B = 1;
}

//闸门完全打开
void Gate_Opened(void)
{
	InitFlag = 1;										//初始化标志位置1
	TimeoutDly = TIMER7_CNT + 500;	//机构超时时间5s
	Gate_Opening();									//闸门直流电机正转
	while(InitFlag)
	{
		if(TimeoutDly == TIMER7_CNT)
		{
			Gate_Stop();										//电机停止运行
			GateState = 1;									//闸门打开状态
			GateStatus = 2;									//闸门打开异常报警
			InitFlag = 0;										//标志位清零
		}
		else if(GS_OPEN_Level)
		{
			Gate_Stop();										//电机停止运行
			GateState = 1;									//闸门为打开状态				
			GateStatus = GateState;					//闸门打开
			InitFlag = 0;										//标志位清零
		}
	}
}

//闸门完全关闭
void Gate_Closed(void)
{
	InitFlag = 1;										//初始化标志位置1
	TimeoutDly = TIMER7_CNT + 500;	//机构超时时间5s
	Gate_Closing();									//闸门直流电机反转
	while(InitFlag)
	{
		if(TimeoutDly == TIMER7_CNT)
		{//检测传感器超时
			Gate_Stop();										//电机停止运行
			GateState = 1;									//闸门关闭
			GateStatus = 2;									//闸门关闭异常报警
			InitFlag = 0;										//标志位清零
		}
		else if(GS_CLOSE_Level)
		{
			Gate_Stop();										//电机停止运行
			GateState = 0;									//闸门为关闭状态				
			GateStatus = GateState;					//闸门关闭
			InitFlag = 0;										//标志位清零
		}
	}
}

//药斗打开进行时
void Dou_Opening(void)
{
	Dou_A = 0;			//左闸门直流电机B2 正转
	Dou_B = 1;	
}

//药斗关闭进行时
void Dou_Closing(void)
{
	Dou_A = 1;			//左阀门直流电机B2 反转
	Dou_B = 0;	
}
//药斗停止
void Dou_Stop(void)
{
		Dou_A = 1;			//左阀门直流电机B2
		Dou_B = 1;
}

//药斗完全打开
void Dou_Opened(void)
{
	InitFlag = 1;										//初始化标志位置1
	TimeoutDly = TIMER7_CNT + 500;	//机构超时时间5s
	Dou_Opening();
	while(InitFlag)
	{
		if(TimeoutDly == TIMER7_CNT)
		{
			Dou_Stop();											//电机停止运行
			DouState = 1;										//斗门打开
			DouStatus = 2;									//斗门打开异常报警
			InitFlag = 0;										//标志位清零
		}
		else if(DS_OPEN_Level)
		{
			Dou_Stop();											//电机停止运行
			DouState = 1;										//斗门为打开状态				
			DouStatus = DouState;						//斗门为打开状态	
			InitFlag = 0;										//标志位清零
		}
	}
}

//药斗完全关闭
void Dou_Closed(void)
{
	InitFlag = 1;										//初始化标志位置1
	TimeoutDly = TIMER7_CNT + 500;	//机构超时时间5s
	Dou_Closing();
	while(InitFlag)
	{
		if(TimeoutDly == TIMER7_CNT)
		{
			Dou_Stop();										//电机停止运行
			DouState = 0;									//斗门关闭状态
			DouStatus = 2;								//斗门关闭异常报警
			InitFlag = 0;									//标志位清零
		}
		else if(DS_CLOSE_Level)
		{
			Dou_Stop();										//电机停止运行
			DouState = 0;									//斗门为关闭状态				
			DouStatus = DouState;					//斗门为关闭状态		
			InitFlag = 0;									//标志位清零
		}
	}
}

//提升机复位程序
void LiftReset(void)
{
	if(DouState == 1)
	{//药斗标志位=1，即打开状态
		Dou_Closed();							//关闭药斗
	}		
	else
	{
		switch(Slave_ID)
		{
			case 2:  	//左提升机
				SERVO_FWDRun(3);    	//提升机向下运行 75r/min											
				break;
			
			case 3:		//右提升机
				SERVO_REVRun(3);    	//提升机向下运行 75r/min												
				break;
			
			default:
				break;
		}
		InitFlag = 1;										//初始化标志位置1
		while(InitFlag)
		{
			if(Bottom_Level)
			{
				SERVO_STOP();														//传感器有信号伺服电机停止
				InitFlag = 0;						//标志位清零
				MachaStatus = 0;				//提升机回到原点					
				delay_ms(100);
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(1);    	//提升机向上运行 25r/min												
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(1);    	//提升机向上运行 25r/min											
						break;
					
					default:
						break;
				}
				delay_ms(10);
				SERVO_STOP();														//传感器有信号伺服电机停止
			}
		}
	}
	
}

//清除报警信号
void CLRALM(void)
{//清除报警信号时序：高——低——高
//	CLR = 1;
//	delay_ms(10);		
	CLR = 0;
	delay_ms(250);
	CLR = 1;
	delay_ms(10);
	AllSystem_Init();     //使所有机构初始化
}

/*************************伺服电机控制********1KHz = 25r/min******************/
//f: 1-25r/min, 8-200r/min, 24-600r/min, 32-800r/min, 40-1000r/min
//传输皮带左运行,左提升机上运行，右提升机下运行
void SERVO_FWDRun(u8 f)
{
	FWD();																	//正转(根据实际情况设定)
	TIM2PWM_Set(f*1000,SERVO_ENABLE);			  //f预定值，可设定
}

//传输皮带右运行,左提升机下运行，右提升机上运行 
void SERVO_REVRun(u8 f)
{
	REV();																	//反转(根据实际情况设定)
	TIM2PWM_Set(f*1000,SERVO_ENABLE);			  //f预定值，可设定
}

//伺服电机停止
void SERVO_STOP(void)
{
	TIM2PWM_Set(20*1000,SERVO_DISABLE);  		//伺服电机停止
}

//机构初始化
void AllSystem_Init(void)  
{
	if(Slave_ID == 1)
	{
		SERVO_FWDRun(3);   //左75r/min
		delay_ms(1000);
		SERVO_STOP();
		delay_ms(1000);
		SERVO_REVRun(3);   //右75r/min
		delay_ms(1000);
		SERVO_STOP();
		MachaStatus = 0;
		DouStatus = 0;
		GateStatus = 0;
		ServoStatus = 0;
		Reason = 0;						//从机空闲
	}
	else if((Slave_ID == 2) || (Slave_ID == 3))
	{
		Gate_Closed();				//闸门关闭
		Dou_Closed();					//药斗关闭
		LiftReset();
		ServoStatus = 0;			//伺服驱动器无报警
		MachaStatus = 0;			//提升机在底部
		if((GateStatus == 2)||(DouStatus == 2))
		{
			Reason = 4;						//从机机构有不可恢复的故障			
		}
		else
		{
			Reason = 0;						//从机空闲						
		}
		delay_ms(1000);
	}
	else
	{
	}
}

void BeltClearALM(void)
{
	switch(ALMState)
	{
		case 1:		//接收到报警信号，将CLR = 1
			DlyTime = TIMER7_CNT + 1;									//延时10ms
			ALMState = 2;															//case 2: 延时10ms检测
			break;
		
		case 2:		//case 2: 延时10ms检测
			if(DlyTime == TIMER7_CNT)
			{//clr = 1,延时10ms时间到
				CLR = 0;																//CLR = 0,延时150ms
				DlyTime = TIMER7_CNT + 15;							//150ms延时
				ALMState = 3;														//case 3:150ms延时检测
			}
			break;
			
		case 3:		//case 3:150ms延时检测
			if(DlyTime == TIMER7_CNT)
			{//clr = 0;延时150ms时间到
				CLR = 1;																//CLR = 1,延时110ms
				DlyTime = TIMER7_CNT + 11;							//延时110ms
				ALMState = 4;														//case 4:延时110ms检测
			}
			break;
			
		case 4:		//case 4:延时110ms检测
			if(DlyTime == TIMER7_CNT)
			{//clr = 1,延时110ms检测
				if(ALM_Level == 0)
				{//若报警信号不可清除
					Reason = 4;			//伺服驱动器出现不可清除的错误信号
				}
				else
				{//报警信号可清除
					Reason = 0;			//伺服驱动器报警已清除
					ServoStatus = 0;						//伺服无报警
				}
			}
			break;
			
		default:
			break;
	}
}

void DOOR_Check(void)
{
	switch(EDMState)
	{
		case 0:		//判断安全急停信号			
			if(EDM_Level == 1)
			{//安全急停信号 = 1
				EDMDlyTime = TIMER7_CNT + 30;		//延时300ms
				EDMState = EDM_DLY_300ms;				//case 1:EDM信号防抖检测
			}
			break;
			
		case EDM_DLY_300ms:	//case 1:EDM信号防抖检测
			if(EDMDlyTime == TIMER7_CNT)
			{
				if(EDM_Level == 1)
				{//安全急停信号 = 1,确定有EDM信号
					SERVO_STOP(); 								//电机停止
					Disable_SON();
					LiftRunState = 0;
					Test.State = 0;								//返回重新判断
					TEST_LED = 1;									//测试灯灭					
					Test.LEDEnable = DISABLE;			//停止闪烁
					Reason = 5;										//急停信号，提升机门打开报警
					EDMState = DoorOpen;				
				} 
				else
				{
					EDMState = 0;				
				}
			}
			break;
		
		case DoorOpen:
			if(EDM_Level == 0)
			{
				EDMState = DoorClose;
			}
			break;
		
		case DoorClose:		//门被关上
			Enable_SON();				//开伺服使能	
			EDMDlyTime = TIMER7_CNT + 11;		//延时110ms
			EDMState = EDM_DLY_110ms;
			break;			
		
		case EDM_DLY_110ms:		//检测110ms
			AllSystem_Init();     //使所有机构初始化
			EDMState = 0;					//返回到初始状态
			break;

		default:
			SERVO_STOP(); 								//电机停止
			LiftRunState = 0;
			Test.State = 0;								//返回重新判断
			TEST_LED = 1;									//测试灯灭					
			Test.LEDEnable = DISABLE;			//停止闪烁
			AllSystem_Init();     //使所有机构初始化
			EDMState = 0;					//返回到初始状态
			break;
	}	
}

void ALM_Check(void)
{
	switch(ALMState)
	{
		case 0:		//伺服驱动器报警信号初始状态,判断是否有伺服报警
			if(ALM_Level == 0)
			{//有伺服报警信号
				ALMDlyTime = TIMER7_CNT + 10;		//报警信号延时100ms
				ALMState = ALM_DLY_100ms;				//case 1:ALM报警信号延时100ms时间检测
			}
			break;
		
		case ALM_DLY_100ms:
			if(ALMDlyTime == TIMER7_CNT)
			{
				if(ALM_Level == 0)
				{
					SERVO_STOP(); 								//电机停止
					Disable_SON();
					LiftRunState = 0;
					RunFlag = 0;
					Test.State = 0;								//返回重新判断
					TEST_LED = 1;									//测试灯灭					
					Test.LEDEnable = DISABLE;			//停止闪烁
					ServoStatus = 1;							//伺服有报警
					Reason = 4;										//伺服电机报警
					ALMDlyTime = TIMER7_CNT + 1000;		//清除报警需要延时10s
					ALMState = ALM_DLY_10s;									
				}
				else
				{
					ALMState = 0;
				}
			}
			break;
			
		case ALM_DLY_10s:
			if(ALMDlyTime == TIMER7_CNT)
			{
				CLR = 1;
				ALMDlyTime = TIMER7_CNT + 1;		//延时10ms
				ALMState = ALM_DLY_10ms;
			}
			break;
			
		case ALM_DLY_10ms:
			if(ALMDlyTime == TIMER7_CNT)
			{
				CLR = 0;
				ALMDlyTime = TIMER7_CNT + 15;		//延时150ms
				ALMState = ALM_DLY_150ms;
			}
			break;
			
		case ALM_DLY_150ms:
			if(ALMDlyTime == TIMER7_CNT)
			{
				CLR = 1;
				ALMDlyTime = TIMER7_CNT + 10;		//延时100ms
				ALMState = ALM_DLY_1000us;			
			}
			break;
			
		case ALM_DLY_1000us:
			
			if(ALMDlyTime == TIMER7_CNT)
			{
				if(ALM_Level == 1)
				{//报警解除
					Enable_SON();				//开伺服使能	
					Reason = 0;										//伺服电机报警解除
					ALMDlyTime = TIMER7_CNT + 11;		//延时110ms
					ALMState = ALM_DLY_110ms;
				}
				else
				{//报警未解除
					ServoStatus = 1;							//伺服有报警
					Reason = 4;										//伺服电机报警
				}
			}
			break;
			
		case ALM_DLY_110ms:
			AllSystem_Init();     //使所有机构初始化
			ALMState = 0;					//返回到初始状态			
			break;

			
		default:
			SERVO_STOP(); 								//电机停止
			LiftRunState = 0;
			Test.State = 0;								//返回重新判断
			TEST_LED = 1;									//测试灯灭					
			Test.LEDEnable = DISABLE;			//停止闪烁
			AllSystem_Init();     //使所有机构初始化
			ALMState = 0;					//返回到初始状态
			break;		
	}
}

//////////传送皮带控制板/////////////////
void TransmitBeltControl(void)
{
	u16 RunTime;    		//伺服电机运行时间
	u8	RunMode;				//运行模式
	u16 ID = 0x0011;		//传送皮带的机构代码地址：0x0011
	RunTime = DMA_REV_BUFF[5];		//伺服电机运行时间
	RunMode = DMA_REV_BUFF[4];		//伺服电机运行模式
		if(RunFlag == 1)
		{
			RunFlag = 0;
			if(ID == ( (DMA_REV_BUFF[2]<<8)|(DMA_REV_BUFF[3]) ))
			{//若ID=0x0011,传送皮带代码正确
				switch(RunMode)
				{//判断第5字节，运动模式
					case 0:		//停止 
						SERVO_STOP(); 															//电机停止
						MachaStatus = 0;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						Reason = 0;									//空闲
						ServoStatus = 0;						//伺服无报警
						break;
					
					case 0x10:		//向左运动 200r/min
						SERVO_FWDRun(8);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 2;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						ServoStatus = 0;						//伺服无报警
						break;
					
					case 0x11:		//向左运动 600r/min
						SERVO_FWDRun(24);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 2;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						ServoStatus = 0;						//伺服无报警
						break;
					
					case 0x12:		//向左运动 800r/min
						SERVO_FWDRun(32);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 2;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						ServoStatus = 0;						//伺服无报警
						break;
					
					case 0x13:		//向左运动 1000r/min
						SERVO_FWDRun(40);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 2;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						ServoStatus = 0;						//伺服无报警
						break;
					
					case 0x20:		//向右运动 200r/min
						SERVO_REVRun(8);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 3;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						ServoStatus = 0;						//伺服无报警
						break;
					
					case 0x21:		//向右运动 600r/min
						SERVO_REVRun(24);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 3;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						ServoStatus = 0;						//伺服无报警
						break;
					
					case 0x22:		//向右运动 800r/min
						SERVO_REVRun(32);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 3;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						ServoStatus = 0;						//伺服无报警
						break;
					
					case 0x23:		//向右运动 1000r/min
						SERVO_REVRun(40);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 3;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						ServoStatus = 0;						//伺服无报警
						break;
										
					default:
						SERVO_STOP(); 															//电机停止
						MachaStatus = 0;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
						Reason = 0;									//空闲
						ServoStatus = 0;						//伺服无报警
						break;
				}
			}
			else
			{
			
			}		
		}
		if(RunTime)			//检测运行时间DMA_REV_BUFF[5]
		{//RunTime>0，则运行N秒后停止
			if(DlyTime == TIMER7_CNT)			//延时时间
			{//延时时间到
				SERVO_STOP(); 															//电机停止			
				MachaStatus = 0;						//电机状态：0——停止 2——传送皮带左转 3——传送皮带右转
				Reason = 0;									//空闲
				ServoStatus = 0;						//伺服无报警
			}
		}
}

//void LeftLiftClearALM(void)
//{
//	switch(ALMState)
//	{
//		case 1:		//接收到报警信号，将CLR = 1
//			DlyTime = TIMER7_CNT + 1;									//延时10ms
//			ALMState = 2;															//case 2: 延时10ms检测
//			break;
//		
//		case 2:		//case 2: 延时10ms检测
//			if(DlyTime == TIMER7_CNT)
//			{//clr = 1,延时10ms时间到
//				CLR = 0;																//CLR = 0,延时150ms
//				DlyTime = TIMER7_CNT + 15;							//150ms延时
//				ALMState = 3;														//case 3:150ms延时检测
//			}
//			break;
//			
//		case 3:		//case 3:150ms延时检测
//			if(DlyTime == TIMER7_CNT)
//			{//clr = 0;延时150ms时间到
//				CLR = 1;																//CLR = 1,延时110ms
//				DlyTime = TIMER7_CNT + 11;							//延时110ms
//				ALMState = 4;														//case 4:延时110ms检测
//			}
//			break;
//			
//		case 4:		//case 4:延时110ms检测
//			if(DlyTime == TIMER7_CNT)
//			{//clr = 1,延时110ms检测
//				if(ALM_Level == 0)
//				{//若报警信号不可清除
//					Reason = 4;			//伺服驱动器出现不可清除的错误信号
//				}
//				else
//				{//报警信号可清除
//					Gate_Closing();				//关闭闸门
//					Dou_Closing();				//斗关闭
//					ALMState = 5;					//case 5:闸门传感器信号检测
//				}
//			}
//			break;
//			
//		case 5:		//case 5:闸门传感器信号检测
//			if(GS_CLOSE_Level == 1)
//			{//检测到闸门关闭传感器信号检测
//				Gate_Stop();											//当检测到关闭传感器时，电机停止运行
//				GateState = 0;										//闸门为关闭状态
//				GateStatus = GateState;						//闸门状态反馈: 0——闸门关闭 1——闸门打开 2——闸门打开或者关闭异常报警
//			}
//			if(DS_CLOSE_Level == 1)				//当检测到药斗关闭传感器时
//			{
//				Dou_Stop();									//当检测到药斗关闭传感器时，电机停止运行
//				DouState = 0;								//药斗为关闭状态
//				DouStatus = DouState;				//药斗状态反馈：0——药斗完成倒药动作 1——未完成倒药动作 2——药斗打开或关闭异常报警
//				SERVO_REVRun(1);						//提升机低速向下运行 25r/min
//				ALMState = 6;								//case 6: 提升机复位传感器信号检测
//			}
//			break;
//			
//		case 6:			//case 6: 提升机复位传感器信号检测
//			if(Bottom_Level == 1)
//			{//检测到复位传感器，停止电机
//				SERVO_STOP(); 							//电机停止	提升机回到原点再向上运行微小距离
//				DlyTime = TIMER7_CNT + 15;	//150ms定时
//				ALMState = 7;								//case 7: 提升机复位后停止延时检测
//			}
//			break;
//			
//		case 7:			//case 7: 提升机复位停止延时检测
//			if(DlyTime == TIMER7_CNT)
//			{//提升机复位停150ms到，回程150ms
//				SERVO_FWDRun(1);						//提升机向上运动 25r/min
//				DlyTime = TIMER7_CNT + 15;	//提升机复位停150ms
//				ALMState = 8;								//case 8: 提升机复位回程延时时间及检测
//			}
//			break;
//			
//		case 8:			//case 8: 提升机复位回程延时时间及检测
//			if(DlyTime == TIMER7_CNT)
//			{//提升机回程时间到，停止电机
//				SERVO_STOP(); 															//电机停止
//				Reason = 0;									//空闲
//				ServoStatus = 0;						//伺服报警清除
//				MachaStatus = 0;						//提升机状态：0——回到起始点, 1——未回到起始点
//			}
//			break;
//		
//		default:
//			break;
//	}	
//}

//////////左提升机控制板///左提升机向上运行SERVO_REVRun()  左提升机向下运行SERVO_FWDRun()/////////////  
//////////右提升机控制板   右提升机向上运行SERVO_FWDRun()  右提升机向下运行SERVO_REVRun()
void HoisterControl(void)
{
		switch(LiftRunState)
		{////判断提升机运行状态
			case 0:     //提升机初始状态，判断提升机运行状态
				if(RunFlag == 1)
				{//机构运行标志位=1，机构可运行
					switch(ID)
					{
						case 0x0021:		//提升机控制:确保药斗门关闭——低速将提升机完全提起——高速运动到设定距离——停留3s(可调)——高速下降到提升机完全提起的位置——低速使提升机复位
							if(DouState == 1)
							{//药斗标志位=1，即打开状态
								Dou_Closed();																		//关闭药斗
							}		
							if(Distance == 0)																	//距离=0，使药斗复位
							{
								switch(Slave_ID)
								{
									case 2:  	//左提升机
										SERVO_FWDRun(3);														//提升机低速向下运行 75r/min
										break;
									
									case 3:		//右提升机
										SERVO_REVRun(3);    												//提升机低速向下运行 75r/min												
										break;
									
									default:
										break;
								}
								LiftRunState = Lift_RST_Check;									//case 1:提升机复位传感器检测
							}
							else if((0 < Distance) && ( Distance < 135))			//提升机提升高度 < 135
							{ 
								PulseCntComplete = 0;							 							//清除反馈脉冲计数标志位
								FeedbackNowNum = 0;						
								FeedbackHopeNum = 2400 * Distance / 108;  	 		//设定反馈脉冲数 = 2400 / 108 * 提升距离(mm), 反馈脉冲设置Pr0.11=2400
								switch(Slave_ID)
								{
									case 2:  	//左提升机
										SERVO_REVRun(3);    												//提升机向上运行 75r/min												
										break;
									
									case 3:		//右提升机
										SERVO_FWDRun(3);    												//提升机向上运行 75r/min											
										break;
									
									default:
										break;
								}
								LiftRunState = Distance_Short_FeedbackPulse;		//case 4: 检测提升机未完全提起反馈脉冲数
							}
							else if((135 <= Distance) && (Distance < 2095))		//提升机可完全提起
							{//先低速运动75r/min,将提升机完全提起								
								PulseCntComplete = 0;							 							//清除反馈脉冲计数标志位
								FeedbackNowNum = 0;		
								FeedbackHopeNum = 3000;													//3000p = 135mm 即：完全提起高度
								switch(Slave_ID)
								{
									case 2:  	//左提升机
										SERVO_REVRun(3);    												//提升机低速向上运行 75r/min										
										break;
									
									case 3:		//右提升机
										SERVO_FWDRun(3);    												//提升机低速向上运行 75r/min											
										break;
									
									default:
										break;
								}
								LiftRunState = Distance_3000_Up_FeedbackPulse;	//case 6: 检测提升机刚好完全提起反馈脉冲数=3000
							}
							else if(Distance == 0x0fff)
							{
								CLRALM();																				//清除报警信号			
							}
							break;
							
					case 0x0022:		//闸门控制
						if(GateAction == 0)		
						{//闸门关闭
							Gate_Closing();										//闸门直流电机反转
							LiftRunState = Gate_Close_Check;	//case 71: 闸门关闭检测					
						}
						else if(GateAction==1)	
						{//闸门打开
							if(MachaStatus == 0)
							{
								Gate_Opening();										//闸门直流电机 
								TimeoutDly = TIMER7_CNT + 500;		//机构超时时间5s
								Reason = 6;
								LiftRunState = Gate_Open_Check;		//case 72: 闸门关闭检测					
							}
						}
						else
						{						
						}
						break;
																
					case 0x0023:		//药斗控制
						if(DouAction == 0)		
						{//药斗关闭
							Dou_Closing();										//药斗直流电机B2
							LiftRunState = Dou_Close_Check;		//case 73: 药斗关闭检测					
						}
						else if(DouAction==1)	
						{//药斗打开
							Dou_Opening();										//药斗直流电机B2
							LiftRunState = Dou_Open_Check;		//case 74: 药斗关闭检测					
						}
						else
						{
						
						}
						break;
					
					case 0x0024:		//联动控制
						TimeoutDly = TIMER7_CNT + 500;	//机构超时时间5s
						if(DouState == 1)
						{//如果药斗门是打开的，就关闭斗门
							Dou_Closing();														//关闭斗门
							LiftRunState = LinkageBefor_Dou_Close_Check;		//case 75: 联合控制：联动控制之前检测药斗是否关闭状态													
						}
						else
						{
							LiftRunState = LinkageBefor_Dou_Close_Check;		//case 75: 联合控制：联动控制之前检测药斗是否关闭状态													
						}
						Reason = 6;										//从机设备忙碌，从机正忙于处理一个长时程序命令，请求主机在从机空闲时发送信息

						break;
						
					case 0x0031:		//提升机控制:确保药斗门关闭——低速将提升机完全提起——高速运动到设定距离——停留3s(可调)——高速下降到提升机完全提起的位置——低速使提升机复位
						if(DouState)
						{//药斗标志位=1，即打开状态
							Dou_Closed();																		//关闭药斗
						}													
						if(Distance == 0)																	//距离=0，使药斗复位
						{
							switch(Slave_ID)
							{
								case 2:  	//左提升机
									SERVO_FWDRun(3);														//提升机低速向下运行 75r/min
									break;
								
								case 3:		//右提升机
									SERVO_REVRun(3);    												//提升机低速向下运行 75r/min												
									break;
								
								default:
									break;
							}
							LiftRunState = Lift_RST_Check;									//case 1:提升机复位传感器检测
						}
						else if((0 < Distance) && ( Distance < 135))			//提升机未完全提起，即： 0 < Distance < 135
						{
							PulseCntComplete = 0;							 							//清除反馈脉冲计数标志位
							FeedbackNowNum = 0;						
							FeedbackHopeNum = 2400 * Distance / 108;  	 		//设定反馈脉冲数 = 2400/108 * 提升距离(mm), 反馈脉冲设置Pr0.11=2400
							switch(Slave_ID)
							{
								case 2:  	//左提升机
									SERVO_REVRun(3);    												//提升机向上运行 75r/min												
									break;
								
								case 3:		//右提升机
									SERVO_FWDRun(3);    												//提升机向上运行 75r/min											
									break;
								
								default:
									break;
							}
							MachaStatus = 1;																//提升机提起不在原点
							LiftRunState = Distance_Short_FeedbackPulse;		//case 4: 检测提升机未完全提起反馈脉冲数
						}
						else if((135 <= Distance) && (Distance < 2095))		//提升机可完全提起, 135 =< Distance < 2095
						{//先低速运动75r/min(可调),将提升机完全提起								
							PulseCntComplete = 0;							 							//清除反馈脉冲计数标志位
							FeedbackNowNum = 0;		
							FeedbackHopeNum = 3000;													//完全提起脉冲数
							switch(Slave_ID)
							{
								case 2:  	//左提升机
									SERVO_REVRun(3);    												//提升机低速向上运行 75r/min(可调)												
									break;
								
								case 3:		//右提升机
									SERVO_FWDRun(3);    												//提升机低速向上运行 75r/min(可调)										
									break;
								
								default:
									break;
							}
							MachaStatus = 1;																//提升机提起不在原点
							LiftRunState = Distance_3000_Up_FeedbackPulse;	//case 6: 检测提升机刚好完全提起反馈脉冲数=3000
						}
						else if(Distance == 0x0fff)
						{
							CLRALM();																				//清除报警信号			
						}
						break;
						
				case 0x0032:		//闸门控制
					if(GateAction == 0)		
					{//闸门关闭
						Gate_Closing();										//闸门直流电机反转
						LiftRunState = Gate_Close_Check;	//case 71: 闸门关闭检测					
					}
					else if(GateAction==1)	
					{//闸门打开
						if(MachaStatus == 0)
						{
							Gate_Opening();										//闸门直流电机 
							TimeoutDly = TIMER7_CNT + 500;		//机构超时时间5s
							Reason = 6;
							LiftRunState = Gate_Open_Check;		//case 72: 闸门打开检测					
						}
					}
					else
					{
					
					}
					break;
										
				case 0x0033:		//药斗控制
					if(DouAction == 0)							
					{//药斗关闭
						Dou_Closing();										//药斗直流电机B2
						LiftRunState = Dou_Close_Check;		//case 73: 药斗关闭检测					
					}
					else if(DouAction==1)	
					{//药斗打开
						Dou_Opening();										//药斗直流电机B2
						LiftRunState = Dou_Open_Check;		//case 74: 药斗关闭检测					
					}
					else
					{
					
					}
					break;
				
				case 0x0034:		//联动控制  首先保证药斗门是关闭状态
					TimeoutDly = TIMER7_CNT + 500;	//机构超时时间5s
					if(DouState == 1)
					{//如果药斗门是打开的，就关闭斗门
						Dou_Closing();																	//关闭斗门
						LiftRunState = LinkageBefor_Dou_Close_Check;		//case 75: 联合控制：联动控制之前检测药斗是否关闭状态													
					}
					else
					{
						LiftRunState = LinkageBefor_Dou_Close_Check;		//case 75: 联合控制：联动控制之前检测药斗是否关闭状态													
					}
					Reason = 6;										//从机设备忙碌，从机正忙于处理一个长时程序命令，请求主机在从机空闲时发送信息
					break;
					
					default:
						break;
				}	
//					RunFlag = 0;					//清除从机响应程序标识
			}
			else
			{
//					RunFlag = 0;					//清除从机响应程序标识
			}
			break;
					
			case Lift_RST_Check:		//case 1:提升机复位传感器检测
				if(Bottom_Level == 1)
				{//检测到复位传感器，停止电机
					delay_ms(10);	
					if(Bottom_Level == 1)
					{
						SERVO_STOP(); 										//电机停止	提升机回到原点再向上运行微小距离
						DlyTime = TIMER7_CNT + 15;				//150ms定时
						LiftRunState = Distance_Return;		//case 2:0距离回程
					}
				}
				break;
							
			case Distance_Return:   //case 2: 提升机复位后回程时间判断
				if(DlyTime == TIMER7_CNT)
				{//提升机复位停150ms到，回程150ms
					PulseCntComplete = 0;							 	//清除反馈脉冲计数标志位
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(1);    	//提升机向上运行 25r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(1);    	//提升机向上运行 25r/min											
							break;
						
						default:
							break;
					}
					DlyTime = TIMER7_CNT + 1;						//提升机复位停10ms
					LiftRunState = Lift_Return_End;			//case 3: 回程结束，停止电机，返回到重新判断提升机运行状态
				}
			break;
			
			case Lift_Return_End:		//case 3: 回程结束，停止电机，返回到重新判断提升机运行状态
				if(DlyTime == TIMER7_CNT)
				{//回程结束，停止电机，返回重新判断运行状态
					SERVO_STOP(); 				//电机停止			
					MachaStatus = 0;			//提升机回到原点
					DouStatus = DouState;				//倒药完成
					GateStatus = GateState;				//倒药完成
					ServoStatus = 0;			//伺服驱动器无报警
					LiftRunState	= 0;		//提升机运行状态重新判断
					RunFlag = 0;					//清除从机响应程序标识
					Distance_Reg = 0;
					if((DouStatus == 2)||(GateStatus == 2))
					{
						Reason = 4;						//从机故障
					}
					else
					{
						Reason = 0;						//从机空闲
					}
				}
				break;
				
			case Distance_Short_FeedbackPulse:	//case 4:提升机未完全提起时反馈脉冲数检测
				if(PulseCntComplete == 1)
				{//反馈脉冲数计数完成
					PulseCntComplete = 0;							 	//清除反馈脉冲计数标志位
					SERVO_STOP(); 										 	//提升机到达设定距离，电机停止
					MachaStatus = 1;										//提升机未在原点
					DouStatus = 0;											//倒药完成
					ServoStatus = 0;										//伺服驱动器无报警
					Reason = 6;													//从机忙碌
					DlyTime = TIMER7_CNT + 300;				 	//延时3s
					LiftRunState = Distance_Short_Dly;	//case 5: 0 < 距离　< 135mm 提升机未完全提起高度延时时间
				}
				break;
			
			case Distance_Short_Dly:	//case 5: 0 < 距离 < 580  提升机未完全提起，到位延时3s
				if(DlyTime == TIMER7_CNT)
				{//3s延时到
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(3);														//提升机低速向下运行 75r/min
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(3);    												//提升机低速向下运行 75r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Lift_RST_Check;	//case 1:提升机复位传感器检测
				}
				break;	
							
			case Distance_3000_Up_FeedbackPulse:	//case 6: 提升机向上到刚好完全提起反馈脉冲数检测
				if(PulseCntComplete == 1)
				{//提升机刚好完全提起时反馈脉冲数计数完成，再高速运动到设定距离 反馈脉冲数 = 2050 ，提升距离=155mm
					PulseCntComplete = 0;				 			//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 200;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(6);    					//提升机上升伺服电机第1次加速 150r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(6);    					//提升机上升伺服电机第1次加速 150r/min										
							break;
						
						default:
							break;
					}
					MachaStatus = 1;									//提升机提起不在原点
					DouStatus = 0;										//药斗关闭
					ServoStatus = 0;									//伺服驱动器无报警
					Reason = 6;												//从机忙碌
					LiftRunState = Up_Accelerate_1;		//case 7:提升机伺服电机1加速阶段,反馈脉冲数检测
				}
				else
				{//提升机刚好提起的反馈脉冲数2050未数到
					
				}
				break;
				
			case Up_Accelerate_1:		//case 7:提升机伺服电机1加速阶段,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 300;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(10);    					//提升机上升伺服电机第2次加速 250r/min										
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(10);    					//提升机上升伺服电机第2次加速 250r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_2;		//case 8:提升机上升伺服电机第2次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Accelerate_2:		//case 8:提升机伺服电机2加速阶段,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 400;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(14);    					//提升机上升伺服电机第3次加速 350r/min										
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(14);    					//提升机上升伺服电机第3次加速 350r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_3;		//case 9:提升机上升伺服电机第3次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;
				
			case Up_Accelerate_3:		//case 9:提升机上升伺服电机第3次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 500;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(18);							//提升机伺服电机第4次加速 450r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(18);							//提升机伺服电机第4次加速 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(18);							//提升机伺服电机第4次加速 450r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(18);							//提升机伺服电机第4次加速 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_4;		//case 10:提升机上升伺服电机第4次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Accelerate_4:		//case 10:提升机上升伺服电机第4次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 600;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(22);							//提升机伺服电机第5次加速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(22);							//提升机伺服电机第5次加速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(22);							//提升机伺服电机第5次加速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(22);							//提升机伺服电机第5次加速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_5;		//case 11:提升机上升伺服电机第5次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Accelerate_5:		//case 11:提升机上升伺服电机第5次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 700;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机伺服电机第6次加速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(26);							//提升机伺服电机第6次加速 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机伺服电机第6次加速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(26);							//提升机伺服电机第6次加速 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_6;		//case 12:提升机上升伺服电机第6次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Accelerate_6:		//case 12:提升机上升伺服电机第6次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 800;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机伺服电机第7次加速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(30);							//提升机伺服电机第7次加速 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机伺服电机第7次加速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(30);							//提升机伺服电机第7次加速 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_7;		//case 13:提升机上升伺服电机第7次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;
				
			case Up_Accelerate_7:		//case 13:提升机上升伺服电机第7次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 												//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = ((2400 * Distance / 108)  - 3000) - 8450;  //快速运动的脉冲数，其中：设定反馈脉冲数 = 13 * 提升距离(mm) - 2050(完全提起的脉冲数), 反馈脉冲设置Pr0.11=2400
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机伺服电机匀速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(32);							//提升机伺服电机匀速 800r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机伺服电机匀速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(32);							//提升机伺服电机匀速 800r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Uniform;							//case 20:提升机上升伺服电机匀速运动，反馈脉冲数检测
				}
				else
				{
				}
				break;
			
			case Up_Uniform:				//case 20:提升机上升伺服电机匀速运动，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;				 			//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 700;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机上升伺服电机第1次减速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(30);							//提升机上升伺服电机第1次减速 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机上升伺服电机第1次减速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(30);							//提升机上升伺服电机第1次减速 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_1;		//case 21:提升机上升伺服电机第1次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;
			
			case Up_Decelerate_1:			//case 21:提升机上升伺服电机第1次减速，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 650;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机上升伺服电机第2次减速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(26);							//提升机上升伺服电机第2次减速 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机上升伺服电机第2次减速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(26);							//提升机上升伺服电机第2次减速 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_2;		//case 22:提升机伺服电机2减速阶段，反馈脉冲数检测
				}
				else
				{
				}
				break;
			
			case Up_Decelerate_2:			//case 22:提升机伺服电机2减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 600;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(22);							//提升机上升伺服电机第3次减速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(22);							//提升机上升伺服电机第3次减速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(22);							//提升机上升伺服电机第3次减速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(22);							//提升机上升伺服电机第3次减速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_3;		//case 23:提升机伺服电机3减速阶段，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Decelerate_3:			//case 23:提升机伺服电机3减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 550;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(18);							//提升机上升伺服电机第4次减速 450r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(18);							//提升机上升伺服电机第4次减速 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(18);							//提升机上升伺服电机第4次减速 450r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(18);							//提升机上升伺服电机第4次减速 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_4;		//case 24:提升机上升第4次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Decelerate_4:			//case 24:提升机伺服电机3减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 500;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(14);    					//提升机伺服电机第5次减速 350r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(14);    					//提升机伺服电机第5次减速 350r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_5;		//case 25:提升机上升第5次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Decelerate_5:			//case 25:提升机伺服电机5减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 450;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(10);    					//提升机伺服电机第6次减速 250r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(10);    					//提升机伺服电机第6次减速 250r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_6;		//case 26:提升机上升第6次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Decelerate_6:			//case 26:提升机伺服电机6减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 400;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(8);    					//提升机伺服电机第7次减速 200r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(8);    					//提升机伺服电机第7次减速 200r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_7;		//case 27:提升机上升第7次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

				
			case Up_Decelerate_7:			//case 27:提升机伺服电机7减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 350;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(6);    					//提升机伺服电机第8次减速 150r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(6);    					//提升机伺服电机第8次减速 150r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_8;		//case 28:提升机上升第8次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Decelerate_8:			//case 28:提升机伺服电机8减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 300;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(5);    					//提升机上升第9次减速 125r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(5);    					//提升机上升第9次减速 125r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_9;		//case 29:提升机上升第9次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Up_Decelerate_9:			//case 29:提升机伺服电机9减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 						//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 250;  											//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(4);    					//提升机上升第10次减速 100r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(4);    					//提升机上升第10次减速 100r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_10;		//case 30:提升机伺服电机10减速阶段，反馈脉冲数检测
				}
				else
				{
				}
				break;
	
			case Up_Decelerate_10:			//case 30:提升机伺服电机10减速阶段，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 						//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 200;  											//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_REVRun(3);    					//提升机上升第11次减速 75r/min												
							break;
						
						case 3:		//右提升机
							SERVO_FWDRun(3);    					//提升机上升第11次减速 75r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Distance_Long_FeedbackPulse;		//case 40:提升机到达设定位置时反馈脉冲数检测
				}
				else
				{
				}
				break;

//			case Up_Decelerate_11:			//case 29:提升机伺服电机9减速阶段，反馈脉冲数检测
//				if(PulseCntComplete == 1)
//				{
//					PulseCntComplete = 0;							 						//清除反馈脉冲计数标志位				
//					FeedbackNowNum = 0;						
//					FeedbackHopeNum = 150;  											//设置反馈脉冲数
//					SERVO_REVRun(2);															//提升机上升第9次减速 100r/min
//					LiftRunState = Up_Decelerate_12;		//case 40:提升机到达设定位置时反馈脉冲数检测
//				}
//				else
//				{
//				}
//				break;

//			case Up_Decelerate_12:			//case 29:提升机伺服电机9减速阶段，反馈脉冲数检测
//				if(PulseCntComplete == 1)
//				{
//					PulseCntComplete = 0;							 						//清除反馈脉冲计数标志位				
//					FeedbackNowNum = 0;						
//					FeedbackHopeNum = 100;  											//设置反馈脉冲数
//					SERVO_REVRun(1);															//提升机上升第9次减速 100r/min
//					LiftRunState = Distance_Long_FeedbackPulse;		//case 40:提升机到达设定位置时反馈脉冲数检测
//				}
//				else
//				{
//				}
//				break;
				
			case Distance_Long_FeedbackPulse:		//case 40:提升机到达设定位置时反馈脉冲数检测
				if(PulseCntComplete == 1)
				{//提升机到达设定位置，反馈脉冲数完成
					PulseCntComplete = 0;							 //清除反馈脉冲计数标志位				
					SERVO_STOP(); 										 //提升机到达设定距离，电机停止
					DlyTime = TIMER7_CNT + 300;				 //延时3s
					LiftRunState = Distance_Long_Dly;	 //case 41: 提升机在到达提起位置停止时间判断					
				}
				else
				{
				}
				break;

			case Distance_Long_Dly:		//case 41: 提升机在到达提起位置停止时间判断
				if(DlyTime == TIMER7_CNT)
				{//延时3s时间到
					PulseCntComplete = 0;							  //清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 100;  						//设置反馈脉冲数 300
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(2);    					//提升机下降伺服电机第1次加速 50r/min									
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(2);    					//提升机下降伺服电机第1次加速 50r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_1;		//case 42:提升机向下第1加速阶段，判断反馈脉冲数
				}				
				else
				{
				}
				break;
				
			case Down_Accelerate_1:		//case 42:提升机向下第1加速阶段，判断反馈脉冲数
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 	//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 200;  			 			//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(6);    					//提升机下降伺服电机第2次加速 150r/min									
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(6);    					//提升机下降伺服电机第2次加速 150r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_2;	 	//case 43:提升机下降伺服电机第2次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;
			
			case Down_Accelerate_2:			//case 43:提升机下降伺服电机第2次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 300;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(10);    					//提升机下降伺服电机第3次加速 250r/min									
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(10);    					//提升机下降伺服电机第3次加速 250r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_3;	//case 44:提升机下降伺服电机第3次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Accelerate_3:			//case 44:提升机下降伺服电机第3次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 400;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(14);    					//提升机下降伺服电机第4次加速 350r/min									
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(14);    					//提升机下降伺服电机第4次加速 350r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_4;	//case 45:提升机下降伺服电机第4次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;
				
			case Down_Accelerate_4:		//case 45:提升机下降伺服电机第4次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 500;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(18);							//提升机下降伺服电机第5次加速 450r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(18);							//提升机下降伺服电机第5次加速 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(18);							//提升机下降伺服电机第5次加速 450r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(18);							//提升机下降伺服电机第5次加速 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_5;	//case 46:提升机伺服电机匀速运动，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Accelerate_5:		//case 46:提升机下降伺服电机第4次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 600;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(22);							//提升机下降伺服电机第6次加速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(22);							//提升机下降伺服电机第6次加速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(22);							//提升机下降伺服电机第6次加速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(22);							//提升机下降伺服电机第6次加速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_6;	//case 47:提升机下降伺服电机第6次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Accelerate_6:		//case 47:提升机下降伺服电机第6次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 700;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机下降伺服电机第7次加速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(26);							//提升机下降伺服电机第7次加速 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机下降伺服电机第7次加速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(26);							//提升机下降伺服电机第7次加速 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_7;	//case 48:提升机下降伺服电机第7次加速,反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Accelerate_7:		//case 48:提升机下降伺服电机第7次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 800;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(22);							//提升机下降伺服电机第6次加速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(22);							//提升机下降伺服电机第6次加速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(22);							//提升机下降伺服电机第6次加速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(22);							//提升机下降伺服电机第6次加速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					switch(SpeedGread)
					{
						case 1:		//速度1 400r/min
							SERVO_FWDRun(16);				 			//提升机下降伺服电机匀速 100r/min
							break;
						
						case 2:		//速度2 600r/min
							SERVO_FWDRun(24);				 			//提升机下降伺服电机第7次加速 600r/min
							break;
						
						case 3:		//速度3 800r/min
							SERVO_FWDRun(30);				 			//提升机下降伺服电机第7次加速 750r/min
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_8;	//case 49:提升机伺服电机匀速运动，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Accelerate_8:		//case 49:提升机下降伺服电机第4次加速,反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 												//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = ((2400 * Distance / 108)  - 3000) - 8500;  //快速运动的脉冲数，其中：设定反馈脉冲数 = 22 * 提升距离(mm) - 2050(完全提起的脉冲数), 反馈脉冲设置Pr0.11=2400
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机下降伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机下降伺服电机匀速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(32);							//提升机下降伺服电机匀速 800r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机下降伺服电机匀速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机下降伺服电机匀速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(32);							//提升机下降伺服电机匀速 800r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Uniform;			//case 60:提升机伺服电机匀速运动，反馈脉冲数检测
				}
				else
				{
				}
				break;
				
			case Down_Uniform:				//case 60:提升机伺服电机匀速运动，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 700;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机下降伺服电机第1次减速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机下降伺服电机第1次减速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(30);							//提升机下降伺服电机第1次减速 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机下降伺服电机第1次减速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机下降伺服电机第1次减速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(30);							//提升机下降伺服电机第1次减速 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_1;	//case 61:提升机下降伺服电机第1次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;
			
			case Down_Decelerate_1:		//case 61:提升机下降伺服电机第1次减速，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 650;  					//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机下降伺服电机第2次减速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(24);							//提升机下降伺服电机第2次减速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(26);							//提升机下降伺服电机第2次减速 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机下降伺服电机第2次减速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(24);							//提升机下降伺服电机第2次减速 600r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(26);							//提升机下降伺服电机第2次减速 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_2;	//case 62:提升机下降伺服电机第2次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;
			
			case Down_Decelerate_2:		//case 62:提升机下降伺服电机第2次减速，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 600;  													//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机下降伺服电机第3次减速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(22);							//提升机下降伺服电机第3次减速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(22);							//提升机下降伺服电机第3次减速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机下降伺服电机第3次减速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(22);							//提升机下降伺服电机第3次减速 550r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(22);							//提升机下降伺服电机第3次减速 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_3;	//case 63: 提升机下降伺服电机第3次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Decelerate_3:		//case 63:提升机下降伺服电机第3次减速，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 550;  													//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_FWDRun(16);							//提升机下降伺服电机第4次减速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_FWDRun(18);							//提升机下降伺服电机第4次减速 450r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_FWDRun(18);							//提升机下降伺服电机第4次减速 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//右提升机
							switch(SpeedGread)
							{
								case 1:		//速度1 400r/min
									SERVO_REVRun(16);							//提升机下降伺服电机第4次减速 400r/min
									break;
								
								case 2:		//速度2 600r/min
									SERVO_REVRun(18);							//提升机下降伺服电机第4次减速 450r/min
									break;
								
								case 3:		//速度3 800r/min
									SERVO_REVRun(18);							//提升机下降伺服电机第4次减速 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_4;	//case 64:提升机下降伺服电机第4次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Decelerate_4:		//case 64:提升机下降伺服电机第4次减速，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 500;  													//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(14);				 			//提升机下降伺服电机第5次减速 350r/min
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(14);    					//提升机下降伺服电机第5次减速 350r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_5;	//case 65:提升机下降伺服电机第5次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Decelerate_5:		//case 65:提升机下降伺服电机第5次减速，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 450;  													//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(10);				 			//提升机下降伺服电机第6次减速 250r/min
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(10);    					//提升机下降伺服电机第6次减速 250r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_6;	//case 66:提升机下降伺服电机第6次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Decelerate_6:		//case 66:提升机下降伺服电机第6次减速，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 400;  													//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(6);				 			//提升机下降伺服电机第7次减速 150r/min
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(6);    					//提升机下降伺服电机第7次减速 150r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_7;	//case 67:提升机下降伺服电机第7次减速，反馈脉冲数检测
				}
				else
				{
				}
				break;

			case Down_Decelerate_7:		//case 67:提升机下降伺服电机第7次减速，反馈脉冲数检测
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//清除反馈脉冲计数标志位				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 350;  													//设置反馈脉冲数
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(3);				 			//提升机下降伺服电机第8次减速 75r/min
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(3);    					//提升机下降伺服电机第8次减速 75r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Distance_3000_Down_FeedbackPulse;	//case 70: 提升机运动：提升机向下运动到刚好提起的位置，判断反馈脉冲数
				}
				else
				{
				}
				break;
				
				case Distance_3000_Down_FeedbackPulse:		//case 70: 提升机运动：提升机向下运动到刚好提起的位置，判断反馈脉冲数
				if(PulseCntComplete == 1)
				{//提升机回到刚好提起的位置，反馈脉冲数完成
					PulseCntComplete = 0;							 //清除反馈脉冲计数标志位				
					switch(Slave_ID)
					{
						case 2:  	//左提升机
							SERVO_FWDRun(3);				 			//提升机下降伺服电机第8次减速 75r/min
							break;
						
						case 3:		//右提升机
							SERVO_REVRun(3);    					//提升机下降伺服电机第8次减速 75r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Lift_RST_Check;		 //case 1:提升机复位传感器检测									
				}
				else
				{
				}
				break;

			case Gate_Close_Check:	//case 71:闸门控制：闸门关闭检测
				if(GS_CLOSE_Level == 1)
				{//检测到关门传感器
					Gate_Stop();								//当检测到关门传感器时，电机停止运行
					GateState = 0;							//闸门为关闭状态
					GateStatus = 0;							//闸门关闭
					LiftRunState	= 0;					//提升机运行状态重新判断
					RunFlag = 0;								//清除从机响应程序标识
					Reason = 0;									//从机空闲
				}
				else
				{
				}
				break;
			
			case Gate_Open_Check:		//case 72: 闸门控制：闸门打开检测
				if(TimeoutDly == TIMER7_CNT)
				{
					Gate_Stop();								//电机停止运行
					GateState = 1;							//闸门打开状态
					GateStatus = 2;							//闸门打开异常报警
					LiftRunState	= 0;					//提升机运行状态重新判断
					RunFlag = 0;								//清除从机响应程序标识
					Reason = 4;									//从机故障报警						
				}
				else if(GS_OPEN_Level)
				{//检测到开门传感器
					Gate_Stop();								//电机停止运行
					GateState = 1;							//闸门为打开状态				
					GateStatus = GateState;			//闸门为打开状态
					LiftRunState	= 0;					//提升机运行状态重新判断
					RunFlag = 0;								//清除从机响应程序标识
					Reason = 0;									//从机空闲						
				}
				break;
				
			case Dou_Close_Check:		//case 73: 药斗控制：药斗关闭检测
				if(DS_CLOSE_Level == 1)				//当未检测到药斗关门传感器时，电机继续运行
				{
					Dou_Stop();									//当检测到药斗关门传感器时，电机停止运行
					DouState = 0;								//药斗为关闭状态
					DouStatus = 0;							//倒药完成
					LiftRunState	= 0;					//提升机运行状态重新判断
					RunFlag = 0;								//清除从机响应程序标识
					Reason = 0;									//从机空闲
				}
				else
				{
				}
				break;
			
			case Dou_Open_Check:		//case 74: 药斗控制：药斗打开检测
				if(DS_OPEN_Level == 1)				//当未检测到药斗开门传感器时，电机继续运行
				{
					Dou_Stop();									//当检测到药斗开门传感器时，电机停止运行
					DouState = 1;								//药斗为打开状态
					DouStatus = 1;							//倒药未完成
					LiftRunState	= 0;					//提升机运行状态重新判断
					RunFlag = 0;								//清除从机响应程序标识
					Reason = 0;									//从机空闲
				}
				else
				{
				}
				break;
				
		case LinkageBefor_Dou_Close_Check:			//case: 75 //联合控制：在联合控制之前，先检测药斗是否关闭
			if(TimeoutDly == TIMER7_CNT)
			{//检测时间超时
				Dou_Stop();							//电机停止运行
				DouState = 0;						//斗门关闭状态
				DouStatus = 2;					//斗门关闭异常报警
				LiftRunState = 0;				//case 0: 初始状态
				RunFlag	= 0;						//清除机构运行标志位
				Reason = 4;							//从机故障
			}
			else if(DS_CLOSE_Level)
			{//检测到斗门关传感器
				Dou_Stop();										//电机停止运行
				DouState = 0;									//斗门为关闭状态				
				DouStatus = DouState;							//斗门状态
				if(GateState == 1)
				{
					Gate_Closing();						//关闭闸门
					TimeoutDly = TIMER7_CNT + 500;		//设超时时间为5s
					LiftRunState	= Linkage_Gate_Close_Check;		//case 76: 提升机运行状态重新判断					
				}
			}
			
			break;
				
		case Linkage_Gate_Close_Check:			//case 76:提升机运行状态重新判断
			if(TimeoutDly == TIMER7_CNT)
			{//超时5s
				Gate_Stop();						//电机停止运行
				GateState = 0;					//闸门关闭状态
				GateStatus = 2;					//闸门关闭异常报警
				LiftRunState = 0;				//case 0: 初始状态
				RunFlag	= 0;						//清除机构运行标志位
				Reason = 4;							//从机故障
			}
			if(Oppsite_Level == 1)
			{//检测到对射传感器信号，即：堵药状态
				Gate_Stop();
				GateState = 1;
				GateStatus = 3;					//发生堵药情况，闸门处于打开状态
				LiftRunState	= Linkage_OppsiteCLR_Check;		//case 77: 提升机闸门发生堵药清除检测
			}
			else if(GS_CLOSE_Level == 1)
			{//检测到关门传感器
				Gate_Stop();															//当检测到关门传感器时，电机停止运行
				GateState = 0;														//闸门为关闭状态
				GateStatus = GateState;											//闸门状态反馈
				LiftRunState	= Linkage_Distance_Check;		//case 79: 提升机提升距离判断
			}
			break;
			
		case Linkage_OppsiteCLR_Check:				//case 77: 提升机闸门发生堵药清除检测
			if(Oppsite_Level == 0)
			{//当堵药清除
				delay_ms(500);		
				Gate_Closing();					//关闭闸门
				LiftRunState	= Linkage_OppsiteCLR_After;		//case 78: 堵药清除后关门检测
			}
			break;

		case	Linkage_OppsiteCLR_After:				//case 78: 堵药清除后关门检测			
			if(GS_CLOSE_Level == 1)
			{//检测到关门传感器
				Gate_Stop();															//当检测到关门传感器时，电机停止运行
				GateState = 0;														//闸门为关闭状态
				GateStatus = GateState;											//闸门状态反馈
				LiftRunState	= Linkage_Distance_Check;		//case 79: 提升机提升距离判断
			}
			break;
			
		case Linkage_Distance_Check:		//case 79: 提升机提升距离判断
			if((Distance >= 700) && (Distance <= 2095))									
			{//提升机可完全提起
				PulseCntComplete = 0;							 		//清除反馈脉冲计数标志位
				FeedbackNowNum = 0;					
				FeedbackHopeNum = 3000;
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(3);    							//提升机低速向上运行 75r/min										
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(3);    							//提升机低速向上运行 75r/min											
						break;
					
					default:
						break;
				}
				MachaStatus = 1;											//提升机提升，不在原点
				LiftRunState = Linkage_3000_Up_Check;	//case 78: 联合控制：提升机上升到完全提起的位置，检测反馈脉冲数					
			}
			else
			{
				LiftRunState	= 0;					//提升机运行状态清零
				RunFlag	= 0;								//清除机构运行标志位
				Reason = 0;									//从机空闲
			}
			break;


			case Linkage_3000_Up_Check:		//case 78: 联合控制：提升机上升到完全提起的位置，检测反馈脉冲数
			if(PulseCntComplete == 1)
			{//提升机刚好完全提起时反馈脉冲数计数完成，再高速运动到设定距离 反馈脉冲数 = 3000 ，提升距离=155mm
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 200;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(6);    							//提升机伺服电机第1次加速 150r/min										
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(6);    							//提升机伺服电机第1次加速 150r/min											
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_1;	//case 79:提升机上升伺服电机第1次加速,反馈脉冲数检测
			}
			else
			{//提升机刚好提起的反馈脉冲数3000未数到
				
			}
			break;
			
		case Linkage_Up_Accelerate_1:		//case 79:提升机上升伺服电机第1次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 300;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(10);    							//提升机伺服电机第2次加速 250r/min										
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(10);    							//提升机伺服电机第2次加速 250r/min											
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_2;	//case 80:提升机上升伺服电机第2次加速,反馈脉冲数检测
			}
			else
			{
			}
			break;
		
		case Linkage_Up_Accelerate_2:		//case 80:提升机上升伺服电机第2次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(14);    							//提升机伺服电机第3次加速 350r/min										
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(14);    							//提升机伺服电机第3次加速 350r/min											
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_3;	//case 81:提升机伺服电机3加速阶段,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Accelerate_3:		//case 81:提升机伺服电机3加速阶段,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(18);							//提升机伺服电机第4次加速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(18);							//提升机伺服电机第4次加速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(18);							//提升机伺服电机第4次加速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(18);							//提升机伺服电机第4次加速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_4;	//case 82:提升机伺服电机4加速阶段,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Accelerate_4:		//case 82:提升机上升伺服电机第4次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机伺服电机第5次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(22);							//提升机伺服电机第5次加速 550r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(22);							//提升机伺服电机第5次加速 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机伺服电机第5次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(22);							//提升机伺服电机第5次加速 550r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(22);							//提升机伺服电机第5次加速 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_5;	//case 83:提升机伺服电机5加速阶段,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Accelerate_5:		//case 83:提升机上升伺服电机第5次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机伺服电机第6次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							//提升机伺服电机第6次加速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(26);							//提升机伺服电机第6次加速 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机伺服电机第6次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							//提升机伺服电机第6次加速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(26);							//提升机伺服电机第6次加速 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_6;	//case 84:提升机伺服电机6加速阶段,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Accelerate_6:		//case 84:提升机上升伺服电机第6次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 1000;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机伺服电机第7次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							//提升机伺服电机第7次加速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(38);							//提升机伺服电机第7次加速 950r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机伺服电机第7次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							//提升机伺服电机第7次加速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(38);							//提升机伺服电机第7次加速 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_7;	//case 85:提升机伺服电机7加速阶段,反馈脉冲数检测
			}
			else
			{
			}
			break;
			
		case Linkage_Up_Accelerate_7:		//case 85:提升机伺服电机7加速阶段,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 	//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = ((2400 * Distance / 108 )  - 3000) - 9100;  //快速运动的脉冲数，其中：设定反馈脉冲数 = 13 * 提升距离(mm) - 2050(完全提起的脉冲数), 反馈脉冲设置Pr0.11=2400
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机伺服电机匀速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							//提升机伺服电机匀速 600r/min
								break;
							
							case 3:		//速度3 1000r/min
								SERVO_REVRun(40);							//提升机伺服电机匀速 1000r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机伺服电机匀速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							//提升机伺服电机匀速 600r/min
								break;
							
							case 3:		//速度3 1000r/min
								SERVO_FWDRun(40);							//提升机伺服电机匀速 1000r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Uniform;		//case 90:提升机上升伺服电机匀速运动，反馈脉冲数检测
			}
			else
			{
			}
			break;
		
		case Linkage_Up_Uniform:				//case 90:提升机上升伺服电机匀速运动，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 		//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 800;  						 	//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							 //提升机伺服电机第1次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							 //提升机伺服电机第1次减速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(38);							 //提升机伺服电机第1次减速 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							 //提升机伺服电机第1次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							 //提升机伺服电机第1次减速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(38);							 //提升机伺服电机第1次减速 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_1;	 //case 91:提升机上升伺服电机第1次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;
		
		case Linkage_Up_Decelerate_1:		//case 91:提升机上升伺服电机第1次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 	  //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 750;  						 	//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							 //提升机伺服电机第2次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							 //提升机伺服电机第2次减速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(34);							 //提升机伺服电机第2次减速 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							 //提升机伺服电机第2次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							 //提升机伺服电机第2次减速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(34);							 //提升机伺服电机第2次减速 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_2;	 //case 92:提升机上升伺服电机第2次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;
		
		case Linkage_Up_Decelerate_2:		 //case 92:提升机伺服电机第2次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;											//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700; 										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							 //提升机伺服电机第3次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(22);							 //提升机伺服电机第3次减速 550r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(30);							 //提升机伺服电机第3次减速 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							 //提升机伺服电机第3次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(22);							 //提升机伺服电机第3次减速 550r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(30);							 //提升机伺服电机第3次减速 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_3;		//case 93:提升机上升伺服电机第3次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_3:		//case 93:提升机上升伺服电机第3次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 650;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							 //提升机伺服电机第4次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(18);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(26);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							 //提升机伺服电机第4次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(18);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(26);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_4;		//case 94:提升机上升第4次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_4:		//case 94:提升机上升伺服电机第3次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							 //提升机伺服电机第4次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(18);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(22);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							 //提升机伺服电机第4次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(18);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(22);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_5;		//case 95:提升机上升第5次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_5:		//case 95:提升机上升伺服电机第5次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 550;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							 //提升机伺服电机第4次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(18);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(18);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							 //提升机伺服电机第4次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(18);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(18);							 //提升机伺服电机第4次减速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_6;		//case 96:提升机上升第6次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_6:		//case 96:提升机上升伺服电机第6次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(14);    					//提升机伺服电机第7次减速 200r/min												
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(14);    					//提升机伺服电机第7次减速 200r/min										
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_7;		//case 97:提升机上升第7次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_7:		//case 97:提升机上升伺服电机第7次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 450;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(10);    					//提升机伺服电机第8次减速 150r/min												
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(10);    					//提升机伺服电机第8次减速 150r/min										
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_8;	//case 98:提升机上升第8次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_8:		//case 98:提升机上升伺服电机第8次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(6);    					//提升机伺服电机第9次减速 125r/min												
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(6);    					//提升机伺服电机第9次减速 125r/min										
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_FeedbackPulse;	//case 99:提升机上升第9次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_FeedbackPulse:		//case 110: 联合控制： 提升机高速运行到设定位置，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{//联合控制：提升机到达设定位置，反馈脉冲数完成
				PulseCntComplete = 0;							 //清除反馈脉冲计数标志位				
				SERVO_STOP(); 										 //提升机到达设定高度，电机停止			
				DlyTime = TIMER7_CNT + 100;				 //提升机到达设定高度，停1s，打开药斗倒药
				LiftRunState = Lift_Stop_Dly;			 //case 111: 联动控制：提升机到达设定高度，延时时间检测		
			}			
			else
			{
			}
			break;
		
		case Lift_Stop_Dly:		//case 111: 联动控制：提升机到达设定高度，延时时间检测	
			if(DlyTime == TIMER7_CNT)
			{//联合控制：提升机停止时间已到，打开药斗
				Dou_Opening();													//打开药斗倒药
				TimeoutDly = TIMER7_CNT + 500;	//机构超时时间5s				
				LiftRunState = Linkage_Dou_Open_Check;	//case 112: 联动控制：打开药斗传感器信号检测	
			}
			else
			{
			}
			break;
			
		case Linkage_Dou_Open_Check:	//case 112: 联动控制：打开药斗传感器信号检测	
			if(TimeoutDly == TIMER7_CNT)
			{//传感器检测超时
				Dou_Stop();										//当检测到药斗开门传感器时，电机停止运行
				DouState = 1;									//药斗打开状态
				DouStatus = 2;								//药斗打开异常报警
				LiftRunState	= 0;					//提升机运行状态重新判断
				RunFlag = 0;								//清除从机响应程序标识
				Reason = 4;									//从机故障报警						
			}
			else if(DS_OPEN_Level == 1)					//当未检测到药斗开门传感器时
			{
				Dou_Stop();										//当检测到药斗开门传感器时，电机停止运行
				DouState = 1;									//药斗为打开状态
				DouStatus = DouState;					//倒药中
				DlyTime = TIMER7_CNT + 200;		//打开药斗倒药设定时间2s
				LiftRunState = Dou_Open_Dly;	//case 113: 联动控制：药斗倒药延时时间检测						
			}
			else
			{
			}
			break;

		case Dou_Open_Dly:	 //case 113: 联动控制：药斗倒药延时时间检测	
			if(DlyTime == TIMER7_CNT)
			{//药斗打开时间到
				Dou_Closing();														//关闭药斗
				TimeoutDly = TIMER7_CNT + 500;	//机构超时时间5s				
				LiftRunState = Linkage_Dou_Close_Check;		//case 114: 联动控制：关闭药斗传感器信号检测	
			}
			else
			{
			}
			break;
			
		case Linkage_Dou_Close_Check:		//case 114:联合控制：联动控制：关闭药斗传感器信号检测
			if(TimeoutDly == TIMER7_CNT)
			{
				Dou_Stop();																	//当检测到药斗关闭传感器时，电机停止运行
				DouState = 0;																//药斗为关闭状态
				DouStatus = 2;															//药斗状态反馈：0——药斗完成倒药动作 1——未完成倒药动作 2——药斗打开或关闭异常报警
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 100;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
//						SERVO_FWDRun(2);    					//提升机下降伺服电机第1次加速 50r/min									
						SERVO_FWDRun(6);    					//提升机下降伺服电机第1次加速 50r/min									
						break;
					
					case 3:		//右提升机
//						SERVO_FWDRun(2);    					//提升机下降伺服电机第1次加速 50r/min									
						SERVO_REVRun(6);    					//提升机下降伺服电机第1次加速 50r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_1;		//case 115:提升机向下第1加速阶段，判断反馈脉冲数
			}
			if(DS_CLOSE_Level == 1)				//当未检测到药斗开门传感器时
			{
				Dou_Stop();																	//当检测到药斗关闭传感器时，电机停止运行
				DouState = 0;																//药斗为关闭状态
				DouStatus = DouState;															//药斗状态反馈：0——药斗完成倒药动作 1——未完成倒药动作 2——药斗打开或关闭异常报警
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 100;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
//						SERVO_FWDRun(2);    					//提升机下降伺服电机第1次加速 50r/min									
						SERVO_FWDRun(6);    					//提升机下降伺服电机第1次加速 50r/min									
						break;
					
					case 3:		//右提升机
//						SERVO_REVRun(2);    					//提升机下降伺服电机第1次加速 50r/min												
						SERVO_REVRun(6);    					//提升机下降伺服电机第1次加速 50r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_1;		//case 115:提升机向下第1加速阶段，判断反馈脉冲数
			}				
			else
			{
			}
			break;
			
		case Linkage_Down_Accelerate_1:		//case 115:提升机向下第1加速阶段，判断反馈脉冲数
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 200;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(6);    					//提升机下降伺服电机第2次加速 150r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(6);    					//提升机下降伺服电机第2次加速 150r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_2;		//case 116:提升机下降伺服电机第2次加速,反馈脉冲数检测
			}
			else
			{
			}
			break;
		
		case Linkage_Down_Accelerate_2:			//case 116:提升机下降伺服电机第2次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 300;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(10);    					//提升机下降伺服电机第3次加速 250r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(10);    					//提升机下降伺服电机第3次加速 250r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_3;		//case 117:提升机下降伺服电机第3次加速,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_3:		//case 117:提升机下降伺服电机第3次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;				 								//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(14);    					//提升机下降伺服电机第4次加速 350r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(14);    					//提升机下降伺服电机第4次加速 350r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_4;		//case 118:提升机下降伺服电机第4次加速,反馈脉冲数检测
			}
			else
			{
			}
			break;
			
		case Linkage_Down_Accelerate_4:		//case 118:提升机下降伺服电机第4次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机第5次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(18);							//提升机下降伺服电机第5次加速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(18);							//提升机下降伺服电机第5次加速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机第5次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(18);							//提升机下降伺服电机第5次加速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(18);							//提升机下降伺服电机第5次加速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_5;		//case 119:提升机下降伺服电机第5次加速,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_5:		//case 119:提升机下降伺服电机第5次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机第5次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(22);							//提升机下降伺服电机第5次加速 550r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(22);							//提升机下降伺服电机第5次加速 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机第6次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(22);							//提升机下降伺服电机第6次加速 550r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(22);							//提升机下降伺服电机第6次加速 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_6;		//case 120:提升机下降伺服电机第6次加速,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_6:		//case 120:提升机下降伺服电机第4次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机第7次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							//提升机下降伺服电机第7次加速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(26);							//提升机下降伺服电机第7次加速 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机第7次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							//提升机下降伺服电机第7次加速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(26);							//提升机下降伺服电机第7次加速 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_7;		//case 121:提升机下降伺服电机第7次加速,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_7:		//case 121:提升机下降伺服电机第7次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 800;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机第8次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							//提升机下降伺服电机第8次加速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(30);							//提升机下降伺服电机第8次加速 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机第8次加速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							//提升机下降伺服电机第8次加速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(30);							//提升机下降伺服电机第8次加速 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_8;		//case 122:提升机下降伺服电机第8次加速,反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_8:		//case 122:提升机下降伺服电机第8次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = ((2400 * Distance / 108)  - 3000) - 8500;  //快速运动的脉冲数，其中：设定反馈脉冲数 = 13 * 提升距离(mm) - 2050(完全提起的脉冲数), 反馈脉冲设置Pr0.11=2400
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机匀速400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							//提升机下降伺服电机匀速600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(32);							//提升机下降伺服电机匀速800r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机匀速400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							//提升机下降伺服电机匀速600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(32);							//提升机下降伺服电机匀速800r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Uniform;				//case 130:提升机伺服电机匀速运动，反馈脉冲数检测
			}
			else
			{
			}
			break;
			
		case Linkage_Down_Uniform:				//case 130:提升机伺服电机匀速运动，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机第1次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							//提升机下降伺服电机第1次减速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(30);							//提升机下降伺服电机第1次减速 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机第1次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							//提升机下降伺服电机第1次减速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(30);							//提升机下降伺服电机第1次减速 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_1;	//case 131:提升机下降伺服电机第1次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;
		
		case Linkage_Down_Decelerate_1:		//case 131:提升机下降伺服电机第1次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 650;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机第2次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(24);							//提升机下降伺服电机第2次减速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(26);							//提升机下降伺服电机第2次减速 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机第2次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(24);							//提升机下降伺服电机第2次减速 600r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(26);							//提升机下降伺服电机第2次减速 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_2;		//case 132:提升机下降伺服电机第2次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;
		
		case Linkage_Down_Decelerate_2:		//case 132:提升机下降伺服电机第2次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机第3次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(22);							//提升机下降伺服电机第3次减速 550r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(22);							//提升机下降伺服电机第3次减速 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机第3次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(22);							//提升机下降伺服电机第3次减速 550r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(22);							//提升机下降伺服电机第3次减速 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_3;		//case 133:提升机下降伺服电机第3次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_3:		//case 133:提升机下降伺服电机第3次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 550;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_FWDRun(16);							//提升机下降伺服电机第4次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_FWDRun(18);							//提升机下降伺服电机第4次减速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_FWDRun(18);							//提升机下降伺服电机第4次减速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//右提升机
						switch(SpeedGread)
						{
							case 1:		//速度1 400r/min
								SERVO_REVRun(16);							//提升机下降伺服电机第4次减速 400r/min
								break;
							
							case 2:		//速度2 600r/min
								SERVO_REVRun(18);							//提升机下降伺服电机第4次减速 450r/min
								break;
							
							case 3:		//速度3 800r/min
								SERVO_REVRun(18);							//提升机下降伺服电机第4次减速 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_4;		//case 134:提升机下降伺服电机第4次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_4:		//case 134:提升机下降伺服电机第4次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(14);				 			//提升机下降伺服电机第5次减速 350r/min
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(14);    					//提升机下降伺服电机第5次减速 350r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_5;		//case 135:提升机下降伺服电机第5次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_5:		//case 135:提升机下降伺服电机第5次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 450;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(10);				 			//提升机下降伺服电机第6次减速 250r/min
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(10);    					//提升机下降伺服电机第6次减速 250r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_6;		//case 136:提升机下降伺服电机第6次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_6:		//case 136:提升机下降伺服电机第6次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(6);				 			//提升机下降伺服电机第7次减速 150r/min
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(6);    					//提升机下降伺服电机第7次减速 150r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_7;		//case 137:提升机下降伺服电机第7次减速，反馈脉冲数检测
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_7:		//case 137:提升机下降伺服电机第7次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 350;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(3);				 			//提升机下降伺服电机第8次减速 75r/min
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(3);    					//提升机下降伺服电机第8次减速 75r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_3000_Down_Check;		//case 140: 联合控制：提升机向下运动到刚好提起的位置，判断反馈脉冲数
			}
			else
			{
			}
			break;
			
		case Linkage_3000_Down_Check:		//case 140:联合控制：提升机向下运行到完全提起的位置，检测反馈脉冲数
			if(PulseCntComplete == 1)
			{//提升机刚好完全提起时反馈脉冲数计数完成，再低速运动到设定距离
				PulseCntComplete = 0;							//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(3);				 			//提升机下降伺服电机低速运动75r/min
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(3);    					//提升机下降伺服电机低速运动75r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Lift_RST_Check;		//case 1: 联合控制：提升机复位传感器信号检测
			}
			else
			{
			}
			break;
						
		default:
			AllSystem_Init();     //使所有机构初始化
			LiftRunState = 0;				
			break;
		}
}		

void AD_Check(void)
{
	if(Slave_ID == 1)
	{//从机地址 = 1(传送带)
		TransmitBeltControl();		//传送皮带控制程序
	}
	else if((Slave_ID == 2)||(Slave_ID == 3))
	{//从机地址 = 2(左提升机) || 从机地址 = 3(右提升机)
		HoisterControl();				//提升机控制程序
	}
	else
	{
	}
}


