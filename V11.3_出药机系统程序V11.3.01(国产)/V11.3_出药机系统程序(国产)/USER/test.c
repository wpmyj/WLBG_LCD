#include "stm32f10x.h"
#include "sys.h"
#include "declare.h"
#include "stm32f10x_tim.h"
#include "delay.h"

//传送皮带测试按键检测
void BeltButton_Check(void)
{
	switch(Test.ButtonState)
	{
		case 0:		//按键按下5s
			if(BUTTON_Level == 1)
			{
				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//去抖100ms
				Test.ButtonState = 1;					//case 1:检测去抖延时时间100ms		
			}
			break;
			
		case 1:		//case 1:检测去抖延时时间100ms	
			if(Test.ButtonDlyTime == TIMER7_CNT)
			{
				Test.ButtonState = 2;					//当前按键状态为高电平，即按键松开
			}
			break;
			
		case 2:
			if(BUTTON_Level == 0)
			{//案件再次被按下，停止测试程序，并返回初始状态
				SERVO_STOP(); 								//电机停止
				TEST_LED = 1;									//测试灯灭					
				Test.LEDEnable = DISABLE;			//停止闪烁
				Test.State = 0;								//返回重新判断
			}
			break;
			
		default:
			break;

	}
}
	
//提升机测试按键检测
void LiftButton_Check(void)
{
	switch(Test.ButtonState)
	{
		case 0:		//按键按下5s
			if(BUTTON_Level == 1)
			{
				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//去抖100ms
				Test.ButtonState = 1;					//case 1:检测去抖延时时间100ms		
			}
			break;
			
		case 1:		//case 1:检测去抖延时时间100ms	
			if(Test.ButtonDlyTime == TIMER7_CNT)
			{
				Test.ButtonState = 2;					//当前按键状态为高电平，即按键松开
			}
			break;
		
//		case 2:
//			if(BUTTON_Level == 0)
//			{
//				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//去抖100ms
//				Test.ButtonState = 3;					//case 1:检测去抖延时时间100ms		
//			}
//			break;
		
		case 2:
//			if(Test.ButtonDlyTime == TIMER7_CNT)
//			{
				if(BUTTON_Level == 0)
				{//按键再次被按下
					SERVO_STOP(); 								//电机停止
					TEST_LED = 1;									//测试灯灭					
					Test.LEDEnable = DISABLE;			//停止闪烁
					Test.State = 0;								//返回重新判断
					AllSystem_Init();     				//使所有机构初始化
				}
//			}
			break;
		
		default:
			break;
	}

}

//传送皮带测试程序,传送皮带正反转，正转30s--停3s--反转30s
void TransmitBelt_Test(void)
{
	switch(Test.State)
	{
		case 0:    //无测试
			TEST_LED = 1;																//测试灯灭
			if(BUTTON_Level == 0)												//如果按键被按下
			{
				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//去抖100ms
				Test.State = Down_DLY_100ms;              //进入状态1:判断按键防抖时间
			}
			else
			{
				Test.State = 0;
			}
			break;

		case Down_DLY_100ms:		//case 1:按键按下防抖时间判断
			if(Test.ButtonDlyTime == TIMER7_CNT)
			{//按键按下 延时100ms(防抖)
				if(BUTTON_Level == 0)
				{//确定按键被按下
					Test.ButtonDownTime = TIMER7_CNT + 500;	//设测试按键按下5s，进入测试运行
					Test.State = BUTTON_DOWN_5s;            //进入状态2:判断按键按下时间
				}
				else
				{
					Test.State = 0;               					//进入状态0:无测试					
				}
			}			
			break;
			
		case BUTTON_DOWN_5s:		//case 2: 测试按键按下5s时间检测
			if(Test.ButtonDownTime == TIMER7_CNT)
			{//按下时间5s到
				Test.State = TEST_FWDRUN;					//进入状态3：测试程序 电机正转
				Test.ButtonState = 0;							//当前按键状态为低电平，被按下
				Test.LEDEnable = ENABLE;
				Test.LEDTime = TIMER7_CNT + 50;		//500ms test_led闪烁用
				WORK_LED_CNT = TIMER7_CNT + 50;
				WORK_LED = 0;
				TEST_LED = 1;
			}
			else if(BUTTON_Level == 1)
			{//5s时间未到，松开按键
				Test.State = 0;										//进入状态0, 即5S测试条件不满足，重新判断状态
			}
			break;
			
		case TEST_FWDRUN:		 	//case 3: 进入测试程序 电机正转
			SERVO_FWDRun(40);   							//启动电机左1000r/min
			Test.RunTime = TIMER7_CNT + 3000;	//传送皮带电机运行时间设置为30s
			Test.State = TEST_RUN_TIME;				//case 4: 进入30s转动等待时间
			BeltButton_Check();
			break;
		
		case TEST_RUN_TIME: 		//case 4: 进入30s运行时间检测
			if(Test.RunTime == TIMER7_CNT)                                                                                                                                                                       
			{//如果30s运行时间到
				SERVO_STOP(); 																//电机停止
				Test.StopTime = TIMER7_CNT + 300;							//电机停止3s定时
				Test.State = FWDRUN_STOP_TIME;								//case 5: 电机停止3s时间定时		
			}
			BeltButton_Check();
      break;
			
		case FWDRUN_STOP_TIME: 		//case 5:电机停止时间判断	
			if(Test.StopTime == TIMER7_CNT)
			{//停止时间到
				SERVO_REVRun(40);   							//启动电机反转 1000r/min
				Test.RunTime = TIMER7_CNT + 3000;	//传送皮带电机运行时间设置为30s			
				Test.State = TEST_REVRUN;					//case 6: 电机进入30s转动等待时间	
			}	
			BeltButton_Check();			
			break;

		case TEST_REVRUN:		 //case 6: 电机进入30s转动等待时间	
			if(Test.RunTime == TIMER7_CNT)
			{//如果30s运行时间到
				SERVO_STOP(); 										//电机停止
				Test.StopTime = TIMER7_CNT + 300;	//电机停止3s定时
				Test.State = REVRUN_STOP_TIME;		//case 7: 电机停止时间定时		
			}
			BeltButton_Check();
			break;
		
		case REVRUN_STOP_TIME:			//case 7: 电机停止时间定时
			if(Test.StopTime == TIMER7_CNT)
			{//停止时间到
				Test.State = 3;										//返回电机左转状态				
			}
			BeltButton_Check();
			break;
		
		default:		//出现错误状态，停止电机转动并放回初始状态
			SERVO_STOP(); 								//电机停止
			TEST_LED = 1;									//测试灯灭			
			Test.LEDEnable = DISABLE;			//停止闪烁
			Test.State = 0;								//返回重新判断状态
			break;
	}			
}

//左提升机测试程序  按下按键5S进入测试程序——打开闸门——3S——关闭闸门——提升机到4号出药口——1S——打开药斗——2S——关闭药斗——回到初始位置——重复以上动作
//////////左提升机控制板///左提升机向上运行SERVO_REVRun()  左提升机向下运行SERVO_FWDRun()/////////////  
//////////右提升机控制板   右提升机向上运行SERVO_FWDRun()  右提升机向下运行SERVO_REVRun()
void Hoister_Test(void)
{
		switch(Test.State)
	{
		case 0:    //case 0:无测试
			TEST_LED = 1;																//测试灯灭
			if(BUTTON_Level == 0)												//如果按键被按下
			{
				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//去抖100ms
				Test.State = Down_DLY_100ms;              //case 1:判断按键防抖时间
			}
			else
			{
				Test.State = 0;
			}
			break;

		case Down_DLY_100ms:		//case 1:防抖时间判断
			if(Test.ButtonDlyTime == TIMER7_CNT)
			{//按键按下
				if(BUTTON_Level == 0)
				{//延时100ms
					Test.ButtonDownTime = TIMER7_CNT + 500;	//设测试按键按下5s，进入测试运行
					Test.State = BUTTON_DOWN_5s;            //case 2:判断按键按下时间
				}
				else
				{
					Test.State = 0;               					//进入状态0:无测试					
				}
			}
			
			break;
			
		case BUTTON_DOWN_5s:     //case 2: 按键按下5s
			if(Test.ButtonDownTime == TIMER7_CNT)
			{//按下时间5s到
				Test.State = TEST_LIFT_START;				//case 3：提升机测试程序开始 
				Test.ButtonState = 0;								//当前按键状态为低电平，被按下
				Gate_Opening();											//打开闸门				
				Test.LEDEnable = ENABLE;
				Test.LEDTime = TIMER7_CNT + 50;			//500ms test_led闪烁用
				WORK_LED_CNT = TIMER7_CNT + 50;
				WORK_LED = 1;
				TEST_LED = 0;
			}
			else if(BUTTON_Level == 1)
			{//5s时间未到，松开按键
				Test.State = 0;				//进入状态0, 即5S测试条件不满足，重新判断状态
			}
			break;
			
		case TEST_LIFT_START:			//case 3:进入测试程序 闸门打开传感器信号检测
			if(GS_OPEN_Level == 1)
			{//检测到闸门打开传感器信号检测
				Gate_Stop();											//当检测到开门传感器时，电机停止运行
				GateState = 1;										//闸门为打开状态
				Test.DlyTime = TIMER7_CNT +300;		//闸门打开3s(时间可调)
				Test.State = TEST_GATE_OPEN_DLY4;	//case 4: 闸门打开延时时间检测		
			}
			LiftButton_Check();
			break;
			
		case TEST_GATE_OPEN_DLY4:				//case 4: 出药口4：闸门打开延时时间检测		
			if(Test.DlyTime == TIMER7_CNT)
			{//闸门打开3s,关闭闸门
				Gate_Closing();												//关闭闸门
				Test.State = TEST_GATE_CLOSE_CHECK4;	//case 5：出药口4：闸门关闭传感器检测								
			}
			LiftButton_Check();
			break;
			
		case TEST_GATE_CLOSE_CHECK4:		//case 5：出药口4：闸门关闭传感器检测					
			if(GS_CLOSE_Level == 1)
			{//检测到闸门关闭传感器信号检测
				Gate_Stop();												//当检测到开门传感器时，电机停止运行
				GateState = 0;											//闸门为关闭状态
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 3000;
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
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
				Test.State = TEST_3000_UP_CHECK4;		//case 6:  出药口4：提升机: 上升到刚好完全提起位置，反馈脉冲检测
			}
			LiftButton_Check();
			break;
		
		case TEST_3000_UP_CHECK4:		//case 6:  出药口4：2095 提升机: 上升到刚好完全提起位置，反馈脉冲检测
			if(PulseCntComplete == 1)
			{//提升机刚好完全提起时反馈脉冲数计数完成，再高速运动到设定距离
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
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
				Test.State = TEST_Win4_Up_Accelerate_1;		//case 7:提升机上升伺服电机第1次减速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;
				
		case TEST_Win4_Up_Accelerate_1:		//case 7:提升机上升伺服电机第1次减速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
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
				Test.State = TEST_Win4_Up_Accelerate_2;		//case 8:提升机上升伺服电机第2次加速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;
			
		case TEST_Win4_Up_Accelerate_2:		//case 8:提升机上升伺服电机第2次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
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
				Test.State = TEST_Win4_Up_Accelerate_3;		//case 9:提升机上升伺服电机第3次加速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Up_Accelerate_3:		//case 9:提升机上升伺服电机第3次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 							 //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = 500;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(18);							//提升机伺服电机第4次加速 450r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(18);							//提升机伺服电机第4次加速 450r/min							
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Accelerate_4;		//case 10:提升机上升伺服电机第3次加速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Accelerate_4:		//case 10:提升机上升伺服电机第3次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 							 //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = 600;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(22);							//提升机伺服电机第5次加速 550r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(22);							//提升机伺服电机第5次加速 550r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Accelerate_5;		//case 11:提升机上升伺服电机第5次加速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Accelerate_5:		//case 11:提升机上升伺服电机第5次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = 700;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(26);							//提升机伺服电机第6次加速 650r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(26);							//提升机伺服电机第6次加速 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Accelerate_6;		//case 11:提升机上升伺服电机第6次加速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Accelerate_6:		//case 11:提升机上升伺服电机第6次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = 1000;  								//设置反馈脉冲数 最高转速1000r/min
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(38);							//提升机伺服电机第7次加速 950r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(38);							//提升机伺服电机第7次加速 950r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Accelerate_7;		//case 12:提升机上升伺服电机第3次加速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;
			
		case TEST_Win4_Up_Accelerate_7:		//case 12:提升机上升伺服电机第3次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 							 //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = (46444  - 3000) - 9100;  //设置反馈脉冲数	(国产机)    1000r/min //2090mm / (0.045mm/p) = 46444p 即：2090mm需要46444个脉冲, 反馈脉冲设置Pr0.11=2400
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(40);							//提升机伺服电机第8次加速 1000r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(40);							//提升机伺服电机第8次加速 1000r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_2;		//case 23:提升机上升伺服电机第2次减速，反馈脉冲数检测
			}
			LiftButton_Check();
			break;
					
		case TEST_Win4_Up_Decelerate_2:		//case 23:提升机上升伺服电机第2次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 800;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(38);							//提升机伺服电机第1次减速 650r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(38);							//提升机伺服电机第1次减速 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_3;		//case 24:提升机上升伺服电机第3次减速，反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Decelerate_3:		//case 24:提升机上升伺服电机第3次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 		//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 750;  							//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(34);							//提升机伺服电机第1次减速 650r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(34);							//提升机伺服电机第1次减速 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_4;		//case 25:提升机上升第4次减速，反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Decelerate_4:		//case 25:提升机上升伺服电机第2次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(30);							//提升机伺服电机第1次减速 650r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(30);							//提升机伺服电机第1次减速 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_5;		//case 26:提升机上升伺服电机第3次减速，反馈脉冲数检测
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_5:		//case 26:提升机上升伺服电机第2次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 650;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(26);							//提升机伺服电机第1次减速 250r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(26);							//提升机伺服电机第1次减速 250r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_6;		//case 27:提升机上升伺服电机第3次减速，反馈脉冲数检测
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_6:		//case 27:提升机上升伺服电机第2次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  									//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(22);							//提升机伺服电机第1次减速 650r/min
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(22);							//提升机伺服电机第1次减速 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_7;		//case 28:提升机上升伺服电机第3次减速，反馈脉冲数检测
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_7:		//case 28:提升机上升伺服电机第2次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 550;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(18);    					//提升机伺服电机第8次减速 150r/min												
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(18);    					//提升机伺服电机第8次减速 150r/min										
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_8;		//case 29:提升机上升伺服电机第3次减速，反馈脉冲数检测
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_8:		//case 29:提升机上升伺服电机第2次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(14);    					//提升机伺服电机第9次减速 125r/min												
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(14);    					//提升机伺服电机第9次减速 125r/min										
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_9;		//case 30:提升机上升伺服电机第3次减速，反馈脉冲数检测
			}
			LiftButton_Check();				
			break;


		case TEST_Win4_Up_Decelerate_9:		//case 30:提升机上升伺服电机第3次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 		//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 450;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(10);    					//提升机伺服电机第10次减速 100r/min												
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(10);    					//提升机伺服电机第10次减速 100r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_10;		//case 40:提升机上升第4次减速，反馈脉冲数检测
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_10:		//case 30:提升机上升伺服电机第3次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 		//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  								//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_REVRun(6);    					//提升机伺服电机第11次减速 150r/min												
						break;
					
					case 3:		//右提升机
						SERVO_FWDRun(6);    					//提升机伺服电机第11次减速 150r/min										
						break;
					
					default:
						break;
				}
				Test.State = TEST_FEEDBACK_PULSE4;		//case 40:提升机上升第4次减速，反馈脉冲数检测
			}
			LiftButton_Check();				
			break;
			
		case TEST_FEEDBACK_PULSE4:		//case 40:提升机上升第4次减速，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{//提升机到达设定位置，反馈脉冲数完成
				PulseCntComplete = 0;							 //清除反馈脉冲计数标志位				
				SERVO_STOP(); 										 //提升机到达设定高度，电机停止			
				Test.DlyTime = TIMER7_CNT + 100;	 //提升机到达设定高度，停1s，打开药斗倒药
				Test.State = TEST_WINDOW4;			 	 //case 41: 出药口4：提升机到达出药口4，延时时间检测		
			}			
			LiftButton_Check();
			break;
		
		case TEST_WINDOW4: //case 41: 提升机到达出药口4: 2095mm
			if(Test.DlyTime == TIMER7_CNT)                                                                                                                                                                       
			{//提升机到出药口3： 停1s,打开药斗
				Dou_Opening();												 //打开药斗倒药
				Test.State 	 = TEST_DOU_OPEN_CHECK4;   //case 42: 出药口4：药斗打开延时时间检测						
			}
			LiftButton_Check();
      break;
			
		case TEST_DOU_OPEN_CHECK4:		 //case 42: 出药口4：药斗打开延时时间检测	
			if(DS_OPEN_Level == 1)				//当未检测到药斗开门传感器时
			{
				Dou_Stop();												//当检测到药斗开门传感器时，电机停止运行
				DouState = 1;											//药斗为打开状态
				Test.DlyTime = TIMER7_CNT + 200;	//打开药斗倒药设定时间2s
				Test.State = TEST_BUCKET_DLY4;		//case 43: 出药口4：药斗倒药延时时间检测						
			}
			LiftButton_Check();
			break;
			
		case TEST_BUCKET_DLY4: //case 43:第4次药斗倒药时间判断
			if(Test.DlyTime == TIMER7_CNT)
			{//药斗倒药时间2s到，关闭药斗
				Dou_Closing();												//关闭药斗
				Test.State = TEST_DOU_CLOSE_CHECK4;		//case 44: 关闭药斗传感器信号检测									
			}	
			LiftButton_Check();			
			break;

		case TEST_DOU_CLOSE_CHECK4:		//case 44: 出药口3：关闭药斗传感器信号检测	
			if(DS_CLOSE_Level == 1)				//当检测到药斗关闭传感器时
			{
				Dou_Stop();																	//当检测到药斗关闭传感器时，电机停止运行
				DouState = 0;																//药斗为关闭状态
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 100;  										//设置反馈脉冲数
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(6);    					//提升机下降伺服电机第1次加速 50r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(6);    					//提升机下降伺服电机第1次加速 50r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_1;		//case 45:提升机向下第1加速阶段，判断反馈脉冲数
			}			
			LiftButton_Check();
			break;
			
		case TEST_Win4_Down_Accelerate_1:		//case 45:提升机向下第1加速阶段，判断反馈脉冲数
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
				Test.State = TEST_Win4_Down_Accelerate_2;		//case 46:提升机下降伺服电机第2次加速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Down_Accelerate_2:		//case 46:提升机下降伺服电机第2次加速,反馈脉冲数检测
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
				Test.State = TEST_Win4_Down_Accelerate_3;		//case 47:提升机下降伺服电机第3次加速,反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_3:		//case 47:提升机下降伺服电机第3次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
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
				Test.State = TEST_Win4_Down_Accelerate_4;		//case 48:提升机伺服电机4加速阶段,反馈脉冲数检测
			}
			LiftButton_Check();
			break;
			
		case TEST_Win4_Down_Accelerate_4:		//case 47:提升机下降伺服电机第4次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(18);    					//提升机下降伺服电机第4次加速 350r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(18);    					//提升机下降伺服电机第4次加速 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_5;		//case 48:提升机伺服电机5加速阶段,反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_5:		//case 47:提升机下降伺服电机第3次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(22);    					//提升机下降伺服电机第4次加速 350r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(22);    					//提升机下降伺服电机第4次加速 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_6;		//case 48:提升机伺服电机3加速阶段,反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_6:		//case 47:提升机下降伺服电机第3次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(26);    					//提升机下降伺服电机第4次加速 350r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(26);    					//提升机下降伺服电机第4次加速 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_7;		//case 48:提升机伺服电机3加速阶段,反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_7:		//case 47:提升机下降伺服电机第3次加速,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 800;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(30);    					//提升机下降伺服电机第4次加速 350r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(30);    					//提升机下降伺服电机第4次加速 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_8;		//case 48:提升机伺服电机3加速阶段,反馈脉冲数检测
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_8:		//case 48:提升机伺服电机3加速阶段,反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 								//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = (46444 - 3000) - 8500;   //设置反馈脉冲数	(国产机)    //2096mm / (0.045mm/p) = 46577p 即：2096mm需要46577个脉冲
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(32);    					//提升机下降伺服电机第4次加速 350r/min									
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(32);    					//提升机下降伺服电机第4次加速 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Uniform;							//case 107:提升机伺服电机匀速运动，反馈脉冲数检测
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Down_Uniform:			//case 107:提升机伺服电机匀速运动，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(30);    					//提升机下降伺服电机第1次减速 750r/min							
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(30);    					//提升机下降伺服电机第1次减速 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_1;		//case 108:提升机下降伺服电机1减速阶段，反馈脉冲数检测
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Down_Decelerate_1:		//case 108:提升机下降伺服电机1减速阶段，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 650;  										//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(26);    					//提升机下降伺服电机第1次减速 750r/min							
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(26);    					//提升机下降伺服电机第1次减速 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_2;		//case 109:提升机伺服电机2减速阶段，反馈脉冲数检测
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Down_Decelerate_2:		//case 109:提升机伺服电机2减速阶段，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  							//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(22);    					//提升机下降伺服电机第1次减速 750r/min							
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(22);    					//提升机下降伺服电机第1次减速 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_3;		//case 110: 提升机向下运动到刚好提起的位置，判断反馈脉冲数
			}
			LiftButton_Check();
			break;
	
		case TEST_Win4_Down_Decelerate_3:		//case 109:提升机伺服电机2减速阶段，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 550;  							//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(18);    					//提升机下降伺服电机第1次减速 750r/min							
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(18);    					//提升机下降伺服电机第1次减速 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_4;		//case 110: 提升机向下运动到刚好提起的位置，判断反馈脉冲数
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Decelerate_4:		//case 109:提升机伺服电机2减速阶段，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  							//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(14);    					//提升机下降伺服电机第1次减速 750r/min							
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(14);    					//提升机下降伺服电机第1次减速 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_5;		//case 110: 提升机向下运动到刚好提起的位置，判断反馈脉冲数
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Decelerate_5:		//case 109:提升机伺服电机2减速阶段，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 450;  							//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(10);    					//提升机下降伺服电机第1次减速 750r/min							
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(10);    					//提升机下降伺服电机第1次减速 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_6;		//case 110: 提升机向下运动到刚好提起的位置，判断反馈脉冲数
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Decelerate_6:		//case 109:提升机伺服电机2减速阶段，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  							//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(6);    					//提升机下降伺服电机第1次减速 750r/min							
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(6);    					//提升机下降伺服电机第1次减速 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_7;		//case 110: 提升机向下运动到刚好提起的位置，判断反馈脉冲数
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Decelerate_7:		//case 109:提升机伺服电机2减速阶段，反馈脉冲数检测
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //清除反馈脉冲计数标志位				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 350;  							//设置反馈脉冲数
				switch(Slave_ID)
				{
					case 2:  	//左提升机
						SERVO_FWDRun(3);    					//提升机下降伺服电机第1次减速 750r/min							
						break;
					
					case 3:		//右提升机
						SERVO_REVRun(3);    					//提升机下降伺服电机第1次减速 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_3000_DOWN_CHECK4;		//case 110: 提升机向下运动到刚好提起的位置，判断反馈脉冲数
			}
			LiftButton_Check();
			break;
			
		case TEST_3000_DOWN_CHECK4:		//case 110: 出药口4：提升机向下运行到完全提起的位置，检测反馈脉冲数
			if(PulseCntComplete == 1)
			{//提升机刚好完全提起时反馈脉冲数计数完成，再低速运动到设定距离
				PulseCntComplete = 0;							 //清除反馈脉冲计数标志位				
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
				Test.State = TEST_LIFT_RST_CHECK4;	//case 111: 出药口4：提升机复位传感器信号检测
			}
			LiftButton_Check();
			break;
			
		case TEST_LIFT_RST_CHECK4:		//case 111: 出药口4：提升机复位传感器信号检测
			if(Bottom_Level == 1)
			{//检测到复位传感器，停止电机
				delay_ms(10);	
				if(Bottom_Level == 1)
				{
					SERVO_STOP(); 											  //电机停止	提升机回到原点再向上运行微小距离
					Test.DlyTime = TIMER7_CNT + 15;				//150ms定时
					Test.State = TEST_DISTANCE_RETURN4;		//case 112: 出药口4：0距离回程
				}
			}
			LiftButton_Check();
			break;
		
		case TEST_DISTANCE_RETURN4:			//case 112: 出药口4：0距离回程
			if(Test.DlyTime == TIMER7_CNT)
			{//提升机复位停150ms到，回程150ms
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
				Test.DlyTime = TIMER7_CNT + 1;	//提升机复位停50ms
				Test.State = TEST_RESET_DLY4;		//case 113: 提升机复位回程延时时间及检测
			}
			LiftButton_Check();
			break;

		case TEST_RESET_DLY4:			 //case 113:提升机第4次复位后延时150ms
			if(Test.DlyTime == TIMER7_CNT)
			{//提升机回程时间到，停止电机
				SERVO_STOP(); 											//电机停止
				Test.DlyTime = TIMER7_CNT + 300;		//提升机完成第4次倒药动作，停顿3s
				Test.State 	 = TEST_RESET_STOP4;    //case 114:提升机完成第4次倒药动作归位							
			}
			LiftButton_Check();
			break;
		
		case TEST_RESET_STOP4:		//case 114:提升机复位完成
			if(Test.DlyTime == TIMER7_CNT)
			{//提升机回程时间到，停止电机
				Gate_Opening();											//打开闸门				
				Test.State = TEST_LIFT_START;			//进入状态3：提升机重复测试程序开始 
			}
			LiftButton_Check();
			break;
			
		default:		//出现错误状态，停止电机转动并放回初始状态
			SERVO_STOP(); 								//电机停止
			TEST_LED = 1;									//测试灯灭			
			Test.LEDEnable = DISABLE;			//停止闪烁
			Test.State = 0;								//返回重新判断状态
			AllSystem_Init();     				//使所有机构初始化
			break;
	}			
}


//////////////测试按键扫描程序
void ButtonScan(void)			
{
	if(Slave_ID == 1)
	{//从机地址 = 1(传送带)
		TransmitBelt_Test();		//传送皮带测试
	}
	else if((Slave_ID == 2)||(Slave_ID == 3))
	{//从机地址 = 2(左提升机) || 从机地址 = 3(右提升机)
		Hoister_Test();				//提升机控制程序
	}
	else
	{
	}
}






