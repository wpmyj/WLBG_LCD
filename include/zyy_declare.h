#ifndef __ZYY_DECLARE_H__
#define __ZYY_DECLARE_H__
#include"HeadType.h"

//ASCII 转换表声明
extern unsigned char AsciiCode[];			//HEX转ASCII
extern unsigned char HexCode[];				//ASCII转HEX

//UART1 缓冲区长度定义
#define Rxd_Length 8
#define Txd_Length 10

#define CRC_Hi	DMA_REV_BUFF[UART1.Rev_Length-2]
#define CRC_Lo 	DMA_REV_BUFF[UART1.Rev_Length-1]

//UART1
extern volatile unsigned char RS485_Delay;		//RS485 COM端 切换延迟状态 0=不切换 1=需要从TX切换到RX

// #define  RS485_COM PAout(11)       
// #define  RS485_REN()	RS485_COM = 0	
// #define  RS485_TEN()	RS485_COM = 1
extern void UART1_Com_Check(void);//UART1通讯处理程序


//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

////UART通讯数据结构——使用的变量 
//typedef struct
//{
//	u8 State;											//通讯状态，0=无通讯内容 1=有通讯内容需要处理
//	u8 Rev_Length;								//通讯接收的数据长度
//	u8 Send_Length;								//通讯发送的数据长度
//}UART;
//extern UART volatile UART1;

//指示灯端口定义
#define WORK_LED PAout(0)       //工作指示灯
#define RFID_LED PAout(1)       //读卡器指示灯

//RC500端口定义
#define RC500_NRD 	PAout(8)		//RC500：读：MF RC500寄存器读出数据(D0~D7)选通
#define RC500_ALE 	PCout(6)		//RC500：地址锁存使能：为高时将AD0~AD5锁存为内部地址
#define RC500_RST 	PCout(7)		//RC500: 复位和掉电：当为高时，内部灌电流关闭，振荡器停止，输入端和外部断开，该管脚的下降沿启动内部复位
#define RC500_NCS 	PCout(8)		//RC500：片选：选择和激活MF RC500的微处理器接口
#define RC500_NWR 	PCout(9)		//RC500：写：MF RC500寄存器写入数据(D0~D7)选通
#define RC500_IRQ		PCin(11)		//RC500：中断请求，输出中断事件请求
#define RC500_ADBUS_IN	(GPIOB -> IDR & 0XFF00) >> 8

#define Lock_Feedback PCin(0)	//电磁锁反馈信号定义
#define BEEP PCout(4)						//蜂鸣器
#define FSD 64									//RC500 FIFO BUFFER SIZE

//RC500变量数据结构定义声明
typedef struct
{
	unsigned int port_init_error;		//端口初始化错误标识 1=AD总线检测失败
	unsigned int rc500_rst_faile;		//RS500重启失败标识 1 = 重启失败
	unsigned char rc500_readcard;		//RC500读卡状态标志 0 = 读卡失败, 1 = 读卡成功, 2 = 有卡冲撞, 3 = 有效区域内没有卡, 4 = 在规定时间内没有完成任务 
}RC500;
extern RC500 volatile RC500_STATE;

extern volatile u8 RC500_BUF[16];		//RC500数据缓冲区

//RC500 寄存器定义
//PAGE 0:命令和状态		
// #define	RegPage							0x00			//选择寄存器页
// #define RegCommand					0x01			//启动(或停止)命令的执行
// #define RegFIFOData					0x02			//64字节FIFO缓冲区输入和输出
// #define RegPrimaryStatus		0x03			//接收器和发送器以及FIFO缓冲区状态标志
// #define RegFIFOLength				0x04			//FIFO中缓冲的字节数
// #define RegSecondaryStatus 	0x05			//不同的状态标志
// #define RegInterruptEn		 	0x06			//使能和禁止中断请求通过的控制位
// #define RegInterruptRq			0x07			//中断请求标志
// //PAGE 1:控制和状态
// #define RegControl					0x09			//不同的控制标志：例如：定时器，节电
// #define RegErrorFlag				0x0A			//显示上次命令执行错误状态的错误标志
#define RegCollpos					0x0B			//RF接口检测到的第一个冲突位的位置
// #define RegTimerValue				0x0C			//定时器的实际值
// #define RegCRCResultLSB			0x0D			//CRC协处理器寄存器的最低位
// #define RegCRCResultMSB			0x0E			//CRC协处理器寄存器的最高位
// #define RegBitFraming				0x0F			//位方式帧的调节
// //PAGE 2:发送器和编码器控制 Transmitter and Coder Control
// #define RegTxControl				0x11			//天线驱动脚TX1和TX2的逻辑状态控制
// #define RegCwConductance		0x12			//选择天线驱动器TX1和TX2的电导率
// #define RegModConductance		0x13			//该值不会改变
// #define RegCoderControl			0x14			//该值不会改变
// #define RegModWidth					0x15			//选择调整脉冲的宽度
// #define RegModWidthSOF			0x16			//该值不会改变
// #define RegTypeBFraming			0x17			//该值不会改变
// //PAGE 3:接收器和解码控制  Receiver and Decoder Control 
// #define RegRxControl1				0x19			//控制接收器状态
// #define RegDecoderControl		0x1A			//控制解码器状态
// #define RegBitPhase					0x1B			//选择发送器和接收器时钟之间的位相位
// #define RegRxThreshold			0x1C			//选择位解码器的阀值
// #define RegBPSKDemControl   0x1D 			//该值不会改变
// #define RegRxControl2       0x1E 			//控制解码器状态和定义接收器的输入源
// #define RegClockQControl    0x1F 			//控制时钟产生用于90°相移的Q信道时钟
// // PAGE 4:时序和信道冗余   RF-Timing and Channel Redundancy 
// #define RegRxWait           	0x21 		//选择发送后，接收器启动前的时间间隔
// #define RegChannelRedundancy  0x22 		//选择RF信道上数据完整性检测的类型和模式
// #define RegCRCPresetLSB       0x23 		//CRC寄存器预设值的低字节
// #define RegCRCPresetMSB       0x24 		//CRC寄存器预设值的高字节
// #define RegTimeSlotPeriod     0x25 		//该值不会改变
// #define RegMfOutSelect        0x26 		//选择输出到管脚MFOUT的内部信号
// #define RFU27                 0x27 		//该值不会改变
// // PAGE 5:FIFO,定时器和IRQ脚配  FIFO, Timer and IRQ-Pin Configuration 
// #define RegFIFOLevel          0x29 		//定义FIFO上溢和下溢警告界限
// #define RegTimerClock         0x2A 		//选择定时器时钟的分频器
// #define RegTimerControl       0x2B 		//选择定时器的起始和停止条件
// #define RegTimerReload        0x2C 		//定义定时器的预装值
// #define RegIRqPinConfig       0x2D 		//配置IRQ脚的输出状态
// #define RFU2E                 0x2E 		//该值不会改变
// #define RFU2F                 0x2F 		//该值不会改变
// // PAGE 6:      RFU 
// #define RFU31                 0x31 		//保留将来之用
// #define RFU32                 0x32 		//保留将来之用
// #define RFU33                 0x33 		//保留将来之用
// #define RFU34                 0x34 		//保留将来之用
// #define RFU35                 0x35 		//保留将来之用
// #define RFU36                 0x36 		//保留将来之用
// #define RFU37                 0x37 		//保留将来之用
// // PAGE 7:测试控制   Test Control 
// #define RFU39                 0x39   	//保留将来之用
// #define RegTestAnaSelect      0x3A    //选择模块测试模式
// #define RFU3B                 0x3B    //该值不会改变
// #define RFU3C                 0x3C    //该值不会改变
// #define RegTestDigiSelect     0x3D    //选择数字测试模式
// #define RFU3E                 0x3E    //保留将来之用
// #define RFU3F		  						0x3F 		//保留将来之用

// ///////////////////////////////////////////////////////////////////// 
// //RC500命令字 
// ///////////////////////////////////////////////////////////////////// 
// #define PCD_IDLE         0x00   //无动作:取消当前命令
// #define PCD_WRITEE2      0x01   //从FIFO缓冲区获得数据并写入内部EEPROM 
// #define PCD_READE2       0x03   //从内部E2PROM读出数据并将其放入FIFO缓冲区(密匙不能被读出)
// #define PCD_LOADCONFIG   0x07   //将EEPROM中保存的寄存器设置调入缓存  从E2PROM读取数据并初始化MF RC500寄存器
// #define PCD_LOADKEYE2    0x0B   //将EEPROM中保存的密钥调入缓存
// #define PCD_AUTHENT1     0x0C   //验证密钥第一步 
// #define PCD_AUTHENT2     0x14   //验证密钥第二步
// #define PCD_RECEIVE      0x16   //接收数据 (在接收器实际启动之前状态机经过寄存器RxWait配置的时间后才结束等待)
// #define PCD_LOADKEY      0x19   //传送密钥 从FIFO缓冲区读出密钥字节并将其放入密钥缓冲区(密钥必须以指定的格式准备)
// #define PCD_TRANSMIT     0x1A   //发送数据 将数据从FIFO缓冲区发送到卡
// #define PCD_TRANSCEIVE   0x1E   //发送并接收数据:将数据从FIFO发送到卡并在发送后自动启动接收器 
// 																// 在接收器实际启动之前状态机经过寄存器RxWait配置的时间后才结束等待
// 																// 该命令是发送和接收的组合
// #define PCD_RESETPHASE   0x3F   //复位 运行复位和初始化阶段(该命令不能通过软件,只能通过上电或硬件复位启动)
// #define PCD_CALCCRC      0x12   //CRC计算 (CRC计算结果可从寄存器CRCResultLSB和CRCResultMSB中读出)

// #define PICC_ANTICOLL1		0x93		// 一级防冲突
// 										// 参数: optional parts of the card's serial number; 
// 										// 返回:rest of) card'sserial number
// #define PICC_ANTICOLL2		0x95		// 二级防冲突
// #define PICC_ANTICOLL3		0x97		// 三级防冲突
// #define PICC_HALT					0x50		// 休眠(参数: Dummy address; 返回:None)

// #define MI_OK            0				 //状态正确标志位

// #define MI_NOTAGERR      1			//有效区域内没有卡
// #define MI_CRCERR        2
// #define MI_EMPTY         3
// #define MI_AUTHERR       4
// #define MI_BITCOUNTERR   11		// 没有卡片的话将产生 MI_BITCOUNTERR，请求16-bit的TagType错误
// #define MI_COLLERR       24		// 此处冲突时产生 MI_COLLERR，直接进入 级联防冲突 提示信息为 寻卡冲突

#define MI_RESETERR      25		//RC500复位失败
#define MI_INTERFACEERR  26		//RC500端口初始化失败
#define MI_ACCESSTIMEOUT 27		//ErrorFlag 没有任何错误，通讯超时  在规定的时间内没有完成任务

#define MI_COM_ERR       125		//通讯失败

//TIME
extern volatile unsigned int WORK_LED_STATUS;	//RC500初始化标志位 0=初始化成功 1=初始化失败
extern volatile unsigned int TIMER7_CNT;  //定时器7 定时10ms次数  
extern volatile unsigned int BEEP_TIME;		//蜂鸣器打开时间
extern volatile unsigned char CARD_500ms;		//500ms寻卡定时器
extern volatile unsigned char CARD_STATE;		//寻卡状态 0=无卡, 1=有卡
extern volatile unsigned int CARD_VALUE_HOLD;	//寻卡保持值
extern volatile unsigned int CARD_VALUE_TIME;	//用于判断1S内寻到的卡是否都为同一张卡
extern volatile unsigned int CARD_TIME_1S;			//寻到卡后的1S计时 0=计时时间没有到 1=时间到
extern volatile unsigned int cnt;

extern volatile unsigned int NO_CARD_Time;
extern volatile unsigned int NO_CARD_Cnt;

extern void TIM7_Int_Init(u16 arr,u16 psc);

//delay
extern void delay_us(u32 nus);		//nus:要延时的us数
extern void delay_ms(u16 nms);		//nms:要延时的ms数

extern void PORT_IO_Init(void);		//硬件端口初始化
extern void UART1_Cmd_Check(void);//UART1 通讯命令字处理

////////////RC500控制程序声明//////////////////
//RC500寄存器初始化
extern signed char RC500_CONFIG_INIT(void);	

// 复位 RC500
signed char PcdReset(void);   

//寻卡程序
extern void Find_Card(void);	

//寻A卡程序
extern u32 PcdRequest(unsigned  char req_code, volatile unsigned char *pTagType);

//防碰撞，读取卡序列号
extern char PbdAnticoll(unsigned char *pSnr);

///////////////RC500 AD总线读写程序声明//////////////////////
extern u8 RC500_READ(u8 addr);							//MCU对RC500读操作
extern void RC500_WRITE(u8 addr, u8 data);	//MCU对RC500写操作

//时钟设置 开启时钟配置外部8M倍频到72M
extern void RCC_Config(void);






#endif
