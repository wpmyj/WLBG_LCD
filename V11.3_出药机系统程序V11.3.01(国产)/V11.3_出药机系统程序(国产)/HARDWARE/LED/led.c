#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "declare.h"

////通讯使能LED
//void enable_led(void)
//{
//	u16 tmp;
//	u8 *ptr;
//	
//	ptr = (u8 *)&DMA_REV_BUFF[1];
//	tmp = LRC16(ptr,3);			//计算通讯的LRC校验和
//	if(tmp == ((LRC_H <<8) | LRC_L))
//	{//若LRC校验和正确，则执行下列程序
//		if(UART1.Led_Color == 0)						//若第3个字节 = 0: 所有灯 1：绿灯  2:红灯 
//		{
//			if(UART1.Led_State == 0)					//第4位字节 = 0：灭所有灯
//			{
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				
//				LED_STATE = LED_DISABLE;				//状态：灭
//			}
//			else if(UART1.Led_State == 1)			//1：亮所有灯
//			{
//				GreenLed_Enable();
//				RedLed_Enable();
//				BUZZ_Disable();
//				LED_STATE = LED_ENABLE;					//状态：亮
//			
//			}
//			else if(UART1.Led_State == 2)			//2：红绿灯交替闪烁
//			{
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				LED_STATE = LED_BLINK;					//状态：闪
//				Led_Blink_TimeValue = TIMER7_CNT + 50;	//500ms亮灭
//			}
//			else															//无操作
//			{
//				
//			}
//			LED_COLOR = ALL_LED;
//		}
//		else if(UART1.Led_Color == 1)				//1：绿灯
//		{//使能绿灯 蜂鸣器响一次
//			if(UART1.Led_State == 0)					//第4位字节 = 0：灭灯 
//			{
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				LED_STATE = LED_DISABLE;				//状态：灭
//			}
//			else if(UART1.Led_State == 1)			//1：亮灯
//			{
//				GreenLed_Enable();
//				RedLed_Disable();
//				GREEN_LED_TIM_VALUE = TIMER7_CNT +50;	//响500ms
//				BUZZ_STATE = GREEN_LED_STATE;		//记录当前状态为绿灯蜂鸣器状态
//				BUZZ_Enable();
//				LED_STATE = LED_ENABLE;					//状态：亮
//			
//			}
//			else if(UART1.Led_State == 2)			//2：闪烁
//			{
//				GreenLed_Enable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				LED_STATE = LED_BLINK;					//状态：闪
//				Led_Blink_TimeValue = TIMER7_CNT + 50;	//500ms亮灭
//			}
//			else															//无操作
//			{
//				
//			}
//			LED_COLOR = GREEN_LED;
//		}
//		else if(UART1.Led_Color == 2)				//2：红灯
//		{//使能红灯 蜂鸣器响三次
//			if(UART1.Led_State == 0)					//第4位字节 = 0：灭灯 
//			{
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				LED_STATE = LED_DISABLE;				//状态：灭
//			}
//			else if(UART1.Led_State == 1)			//1：亮灯
//			{
//				GreenLed_Disable();
//				RedLed_Enable();
//				Red_Led_Num = 3;
//				BUZZ_STATE = RED_LED_STATE;			//记录当前状态为红灯蜂鸣器状态
//				RED_LED_TIM_VALUE = TIMER7_CNT +50;	//响500ms
////				BUZZ_Enable();
//				LED_STATE = LED_ENABLE;					//状态：亮
//			
//			}
//			else if(UART1.Led_State == 2)			//2：闪烁
//			{
//				GreenLed_Disable();
//				RedLed_Enable();
//				BUZZ_Disable();
//				LED_STATE = LED_BLINK;					//状态：闪
//				Led_Blink_TimeValue = TIMER7_CNT + 50;	//500ms亮灭
//			}
//			else															//无操作
//			{
//				
//			}
//			LED_COLOR = RED_LED;
//		}
//		else
//		{
//			
//		}			
//	}
//	else
//	{//LRC校验码错误
//		for(tmp=0;tmp<8;tmp++)
//		{
//			DMA_REV_BUFF[tmp] = 0;		//清除通讯缓冲区
//		}
//	}
//}

////LED闪烁使能检查
//void Led_State_Check(void)
//{
//	switch(LED_STATE)
//	{
//		case 0: //  LED_STATE = 0：灯灭
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();

//			break;
//		case 1:	//  LED_STATE = 1：灯亮
//			if(UART1.Led_Color == 0)   //所有灯
//			{
//				GreenLed_Enable();
//				RedLed_Enable();
//			}
//			if(UART1.Led_Color == 1)	 //绿灯
//			{
//				GreenLed_Enable();
//				RedLed_Disable();
//			}
//			if(UART1.Led_Color == 2)	 //红灯
//			{
//				GreenLed_Disable();
//				RedLed_Enable();
//			}
//			else
//			{
//				
//			}
//			break;
//		case 2:		//  LED_STATE = 2：灯闪
//			if(UART1.Led_Color == 0)   //所有灯
//			{
//				if(Led_Blink_TimeValue == TIMER7_CNT)
//				{
//					if(GREEN)
//					{
//						GREEN = 0;
//						RED = 1;
//					}
//					else
//					{
//						GREEN = 1;
//						RED = 0;
//					}
//					Led_Blink_TimeValue = TIMER7_CNT + 50;
//				}
//			}
//			if(UART1.Led_Color == 1)	 //绿灯
//			{
//				if(Led_Blink_TimeValue == TIMER7_CNT)
//				{
//					if(GREEN)
//					{
//						GREEN = 0;
//						RED = 0;
//					}
//					else
//					{
//						GREEN = 1;
//						RED = 0;
//					}
//					Led_Blink_TimeValue = TIMER7_CNT + 50;
//				}
//			}
//			if(UART1.Led_Color == 2)	 //红灯
//			{
//				if(Led_Blink_TimeValue == TIMER7_CNT)
//				{
//					if(RED)
//					{
//						RED = 0;
//						GREEN = 0;
//					}
//					else
//					{
//						RED = 1;
//						GREEN = 0;
//					}
//					Led_Blink_TimeValue = TIMER7_CNT + 50;	//500ms亮灭
//				}
//			}
//			break;
//		default:
//			break;
//	}
//}

////蜂鸣器定时检测
//void Buzz_Tim_Check(void)
//{
//	switch(BUZZ_STATE)
//	{
//		case GREEN_LED_STATE:
//				if((BUZZ == 1)&&(GREEN_LED_TIM_VALUE == TIMER7_CNT))
//				{
//					BUZZ_Disable();
//				}
//			break;
//		case RED_LED_STATE:
//				if(Red_Led_Num)
//				{//3次没有响完
//					switch(RED_LED_BUZZ_STATE)
//					{
//						case RED_LED_BUZZ_DISABLE:
//							if(RED_LED_TIM_VALUE == TIMER7_CNT)
//							{//500ms不响完成 重启使能蜂鸣器
//								BUZZ_Enable();
//								RED_LED_TIM_VALUE = TIMER7_CNT + 50;		//响500ms
//								RED_LED_BUZZ_STATE = RED_LED_BUZZ_ENABLE;
//							}
//							break;
//						case RED_LED_BUZZ_ENABLE:
//								if(RED_LED_TIM_VALUE == TIMER7_CNT)
//								{//500ms使能完成 关闭蜂鸣器
//									BUZZ_Disable();
//									RED_LED_TIM_VALUE = TIMER7_CNT + 50;
//									RED_LED_BUZZ_STATE = RED_LED_BUZZ_DISABLE;
//									Red_Led_Num--;
//								}
//							break;
//						default:
//							break;
//					}
//				}
//			break;
//		default:
//			break;
//	}
//}


