#ifndef __ZYY_DECLARE_H__
#define __ZYY_DECLARE_H__

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

extern u8 CRCHiTable[];			 
extern u8 CRCLoTable[];		 

#define Rxd_Length 8
#define Txd_Length 8

#define CRC_Hi	DMA_REV_BUFF[UART1.Rev_Length-2]
#define CRC_Lo 	DMA_REV_BUFF[UART1.Rev_Length-1]

#define SERVO_ENABLE 50
#define SERVO_DISABLE 0XFF

#define BELT 			1
#define LeftLift 	2
#define RightLift 3

#define Down_DLY_100ms 	 1		 
#define BUTTON_DOWN_5s 	 2		 
#define TEST_FWDRUN 	 	 3		 
#define TEST_RUN_TIME  	 4		 
#define FWDRUN_STOP_TIME 5		 
#define TEST_REVRUN		 	 6		 
#define REVRUN_STOP_TIME 7	 

#define TEST_LIFT_START				 			3		 

#define TEST_GATE_OPEN_DLY4		 			4 	 
#define TEST_GATE_CLOSE_CHECK4 			5		 

#define TEST_3000_UP_CHECK4		 			6		 
#define TEST_Win4_Up_Accelerate_1 	7		 
#define TEST_Win4_Up_Accelerate_2 	8		 
#define TEST_Win4_Up_Accelerate_3 	9		 
#define TEST_Win4_Up_Accelerate_4 	10		 
#define TEST_Win4_Up_Accelerate_5 	11		 
#define TEST_Win4_Up_Accelerate_6 	12		 
#define TEST_Win4_Up_Accelerate_7 	13		 
#define TEST_Win4_Up_Uniform	 			21	 
#define TEST_Win4_Up_Decelerate_1 	22	 
#define TEST_Win4_Up_Decelerate_2 	23	 
#define TEST_Win4_Up_Decelerate_3 	24	 
#define TEST_Win4_Up_Decelerate_4 	25	 
#define TEST_Win4_Up_Decelerate_5 	26	 
#define TEST_Win4_Up_Decelerate_6 	27	 
#define TEST_Win4_Up_Decelerate_7 	28	 
#define TEST_Win4_Up_Decelerate_8 	29 
#define TEST_Win4_Up_Decelerate_9 	30	 
#define TEST_Win4_Up_Decelerate_10 	31	 

#define TEST_FEEDBACK_PULSE4	 			40	 
#define TEST_WINDOW4					 			41	 
#define TEST_DOU_OPEN_CHECK4	 			42	 
#define TEST_BUCKET_DLY4			 			43	 
#define TEST_DOU_CLOSE_CHECK4	 			44	 

#define TEST_Win4_Down_Accelerate_1	45	 
#define TEST_Win4_Down_Accelerate_2	46	 
#define TEST_Win4_Down_Accelerate_3	47	 
#define TEST_Win4_Down_Accelerate_4	48	 
#define TEST_Win4_Down_Accelerate_5 49	 
#define TEST_Win4_Down_Accelerate_6 50	 
#define TEST_Win4_Down_Accelerate_7 51	 
#define TEST_Win4_Down_Accelerate_8 52	 
#define TEST_Win4_Down_Uniform			70 
#define TEST_Win4_Down_Decelerate_1	71	 
#define TEST_Win4_Down_Decelerate_2	72	 
#define TEST_Win4_Down_Decelerate_3	73	 
#define TEST_Win4_Down_Decelerate_4	74	 
#define TEST_Win4_Down_Decelerate_5	75	 
#define TEST_Win4_Down_Decelerate_6	76	 
#define TEST_Win4_Down_Decelerate_7	77	 

#define TEST_3000_DOWN_CHECK4	 			90	 
#define TEST_LIFT_RST_CHECK4	 			91	 
#define TEST_DISTANCE_RETURN4	 			92	 
#define TEST_RESET_DLY4				 			93	 
#define TEST_RESET_STOP4			 			94	 

extern volatile unsigned char InitFlag;			 
extern volatile unsigned short TimeoutDly;	 

#define InitGate					1  
#define InitGate_Timeout	2		 



#define NONE 		0  
#define FWDRUN  1	 
#define REVRUN  2	 
#define STOP		3  

#define DLY_OK	 2

typedef struct 
{
	u8 State; 						 
	u8 Rev_Length;				 
	u8 Send_Length;				 
}UART;
extern UART volatile UART1;

typedef struct
{
 u8   State;					 
 u8   ButtonDownEnable; 
 u16  ButtonDownTime; 
 u16  LEDTime;  			 
 u16	RunTime;				 
 u16  StopTime;				 
 u8   LEDEnable;			 
 u8 	RunFlag;				 
 u16 	ButtonDlyTime;	 
 u8		ButtonState;		 
 u16	DlyTime;				 
}TEST;
extern TEST volatile Test;

extern volatile unsigned char Slave_ID;			 
extern unsigned char SW_ID(void);
extern void DialSwitch_Init(void);

extern volatile unsigned short ID;						 
extern volatile unsigned short Distance;				 
extern volatile unsigned char  SpeedGread;			 
extern volatile unsigned char  GateAction;			 
extern volatile unsigned char  DouAction;			 

//time
extern volatile	unsigned short WORK_LED_CNT;		 
extern volatile unsigned short TIMER7_CNT;  			 
extern volatile unsigned short DlyTime;					 
extern volatile unsigned short ALMDlyTime;			 
extern volatile unsigned short EDMDlyTime;			 
//TIME
extern void TIM7_Int_Init(u16 arr,u16 psc);
extern void Coder_InCap_Tim3(u16 arr,u16 psc);


//反馈脉冲计数值
extern volatile unsigned int FeedbackNowNum;		 
extern volatile unsigned int FeedbackHopeNum;		 
extern volatile unsigned char  PulseCntComplete; 

extern volatile unsigned char RunningState;				 
extern volatile unsigned char RunFlag;						 
extern volatile unsigned char CLRAlarm;						 
extern volatile unsigned char GateState;					 
extern volatile unsigned char DouState;						 
extern volatile unsigned char ALMState;					 
extern volatile unsigned char Reason;						 
 extern volatile unsigned char DouStatus;					 
extern volatile unsigned char MachaStatus;				 
 extern volatile unsigned char ServoStatus;				 
extern volatile unsigned char GateStatus;					 
extern volatile unsigned char Distance_Reg;				 


extern volatile unsigned char EDMState;						 
#define EDM_DLY_300ms	1		 
#define DoorOpen			2	 
#define DoorClose 		3		 
#define EDM_DLY_110ms	4		 

extern volatile unsigned char ALMState;				 
#define ALM_DLY_100ms		1 
#define ALM_DLY_10s			2	 
#define ALM_DLY_10ms		3	 
#define ALM_DLY_150ms		4	 
#define ALM_DLY_1000us	5 
#define ALM_DLY_110ms		6

extern volatile unsigned char LiftRunState;				 

#define Lift_RST_Check									 1	 
#define Distance_Return									 2	 
#define Lift_Return_End									 3	 
#define Distance_Short_FeedbackPulse 		 4	 
#define Distance_Short_Dly							 5	 

#define Distance_3000_Up_FeedbackPulse 	 6 	 
#define Up_Accelerate_1 								 7	 
#define Up_Accelerate_2 								 8	 
#define Up_Accelerate_3 								 9 
#define Up_Accelerate_4 								 10 
#define Up_Accelerate_5 								 11	 
#define Up_Accelerate_6 								 12	 
#define Up_Accelerate_7 								 13 

#define Up_Uniform											 20	 
#define Up_Decelerate_1 							 	 21	 
#define Up_Decelerate_2 								 22  
#define Up_Decelerate_3 								 23	 
#define Up_Decelerate_4 								 24	 
#define Up_Decelerate_5 								 25	 
#define Up_Decelerate_6 								 26	 
#define Up_Decelerate_7 								 27	 
#define Up_Decelerate_8 								 28	 
#define Up_Decelerate_9 								 29	 
#define Up_Decelerate_10 								 30	 
#define Up_Decelerate_11 								 31	 
#define Up_Decelerate_12 								 32	 

#define Distance_Long_FeedbackPulse 		 40	 
#define Distance_Long_Dly 							 41	 
#define Down_Accelerate_1 							 42	 
#define Down_Accelerate_2 							 43	 
#define Down_Accelerate_3 							 44	 
#define Down_Accelerate_4 							 45	 
#define Down_Accelerate_5 							 46	 
#define Down_Accelerate_6 							 47	 
#define Down_Accelerate_7 							 48	 
#define Down_Accelerate_8 							 49	 

#define Down_Uniform										 60  
#define Down_Decelerate_1 							 61	 
#define Down_Decelerate_2 							 62	 
#define Down_Decelerate_3 							 63	 
#define Down_Decelerate_4 							 64	 
#define Down_Decelerate_5 							 65	 
#define Down_Decelerate_6 							 66	 
#define Down_Decelerate_7 							 67	 
#define Down_Decelerate_8 							 68	 

#define Distance_3000_Down_FeedbackPulse 70	 
#define Gate_Close_Check  							 71	 
#define Gate_Open_Check									 72	 
#define Dou_Close_Check 								 73	 
#define Dou_Open_Check									 74	 

#define LinkageBefor_Dou_Close_Check		 75  
#define Linkage_Gate_Close_Check				 76	 
#define Linkage_OppsiteCLR_Check				 77  
#define Linkage_OppsiteCLR_After				 78	 

#define Linkage_Distance_Check					 79  
#define Linkage_3000_Up_Check						 80	 
#define Linkage_Up_Accelerate_1 				 81	 
#define Linkage_Up_Accelerate_2 				 82	 
#define Linkage_Up_Accelerate_3 				 83	 
#define Linkage_Up_Accelerate_4 				 84	 
#define Linkage_Up_Accelerate_5 				 85	 
#define Linkage_Up_Accelerate_6 				 86	 
#define Linkage_Up_Accelerate_7 				 87	 

#define Linkage_Up_Uniform							 90	 
#define Linkage_Up_Decelerate_1 				 91	 
#define Linkage_Up_Decelerate_2 				 92	 
#define Linkage_Up_Decelerate_3 				 93	 
#define Linkage_Up_Decelerate_4 				 94	 
#define Linkage_Up_Decelerate_5 				 95	 
#define Linkage_Up_Decelerate_6 				 96	 
#define Linkage_Up_Decelerate_7 				 97	 
#define Linkage_Up_Decelerate_8 				 98	 
#define Linkage_Up_Decelerate_9 				 99	 
#define Linkage_Up_Decelerate_10 				 100	 
#define Linkage_Up_Decelerate_11 				 101	 
#define Linkage_Up_Decelerate_12 				 102	 

#define Linkage_FeedbackPulse 					 110	 
#define Lift_Stop_Dly										 111	 
#define Linkage_Dou_Open_Check					 112	 
#define Dou_Open_Dly										 113	 
#define Linkage_Dou_Close_Check					 114	 
#define Linkage_Down_Accelerate_1 			 115	 
#define Linkage_Down_Accelerate_2 			 116	 
#define Linkage_Down_Accelerate_3 			 117	 
#define Linkage_Down_Accelerate_4 			 118	 
#define Linkage_Down_Accelerate_5 			 119	 
#define Linkage_Down_Accelerate_6 			 120	 
#define Linkage_Down_Accelerate_7 			 121	 
#define Linkage_Down_Accelerate_8 			 122	 

#define Linkage_Down_Uniform						 130	 
#define Linkage_Down_Decelerate_1 			 131	 
#define Linkage_Down_Decelerate_2 			 132	 
#define Linkage_Down_Decelerate_3 			 133	 
#define Linkage_Down_Decelerate_4 			 134	 
#define Linkage_Down_Decelerate_5 			 135	 
#define Linkage_Down_Decelerate_6 			 136	 
#define Linkage_Down_Decelerate_7 			 137	 
#define Linkage_Down_Decelerate_8 			 138	 

#define Linkage_3000_Down_Check					 140	 


//DMA
#define DMA_UART1_TX	DMA1_Channel4		 
#define DMA_UART1_RX	DMA1_Channel5

extern volatile u8 DMA_SEND_BUFF[]; 
extern volatile u8 DMA_REV_BUFF[] ; 

extern void DMA_INIT(void);

extern void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u8 Dir,u32 ex_ph_addr,u32 buff,u16 length); 

extern void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 Length); 

#define  WORK_LED PCout(13)      

#define  PLUS PAout(0)							 
#define  SON  PAout(1)								 
#define  Enable_SON()	 SON = 0			 
#define  Disable_SON() SON = 1				 
#define  CLR	PAout(2) 								 
#define  Feedback_Plus PAin(6)				 
#define  SIGN PBout(11)								 
#define  FWD()	SIGN = 1;						  
#define  REV()	SIGN = 0;							 
#define  ALM  PBin(12)								 
#define  EDM  PBin(13)							 

#define ALM_Level				GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)		 

#define EDM_Level				GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)		 

#define  Gate_A PCout(6)							 
#define  Gate_B PCout(7)							 

#define  Dou_A PBout(14)							 
#define  Dou_B PBout(15)							 

#define  Bottom 	PAin(4)						 
#define  Top   		PAin(3)						 
#define  GS_CLOSE PAin(5)						 
#define  GS_OPEN 	PAin(7)						 
#define  Oppsite 	PCin(4)						 
#define  DS_OPEN 	PBin(0)						 
#define  DS_CLOSE PCin(5)						 

#define Bottom_Level 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)		 
#define Top_Level				GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)			 
#define Oppsite_Level 	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)			 
#define GS_OPEN_Level		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)			 
#define GS_CLOSE_Level 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)			 
#define DS_OPEN_Level 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)			 
#define DS_CLOSE_Level	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)			 

#define CODER1 PAin(11)					 
#define CODER2 PAin(12)					 

#define One	~GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)	 
#define Two ~GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)		 

#define BUTTON		PCin(10)						 

#define BUTTON_Level		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)     

#define TEST_LED	PBout(9)					 

extern void delay_us(u32 nus);
extern void delay_ms(u16 nms);

extern void variable_init(void);
extern void PORT_IO_Init(void);		 

extern volatile unsigned char Timer0Value;			   		 
extern void Timer0_Init(void);
extern void TIM2PWM_Set(u32 f, u8 duty);		  

extern void Timer2_Init(void); 

extern void UART1_Com_Check(void);				 
extern void AD_Check(void);								 

extern void delay(unsigned char time);

extern void AllSystem_Init(void);			 
extern void LiftReset(void);					 
extern void Gate_Opening(void);				 
extern void Gate_Opened(void);				 
extern void Gate_Closing(void);				 
extern void Gate_Closed(void);			 
extern void Gate_Stop(void);					 
extern void Dou_Opening(void);				 
extern void Dou_Opened(void);					 
extern void Dou_Closing(void);				 
extern void Dou_Closed(void);					 
extern void Dou_Stop(void);						 
extern void SERVO_FWDRun(u8 f);				 
extern void SERVO_REVRun(u8 f);				 
extern void SERVO_STOP(void);					 
extern void TransmitBeltControl(void);	 
extern void HoisterControl(void);			 
extern void ButtonScan(void);   			 
extern void ALM_Check(void);				 
extern void	DOOR_Check(void);

#endif	

