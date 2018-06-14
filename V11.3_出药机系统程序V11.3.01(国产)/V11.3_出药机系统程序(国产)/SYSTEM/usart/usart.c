#include "sys.h"
#include "delay.h"
#include "usart.h"	  
#include "stm32f10x_dma.h"
#include "declare.h"
#include "stm32f10x_rcc.h"
////////////////////////////////////////////////////////////////////////////////// 	 
UART volatile UART1;

//UART1通讯接收校验码程序  异或校验
u16 CRC16_Check1(u8 *bufdata, u8 length)			//方法1：查表法
{
	u8 CRCHi = 0xff;
	u8 CRCLo = 0xff;
	int Index;
	u16 shift = 0xffff;
	while(length--)
	{
		Index = CRCLo ^ *(bufdata++);
		CRCLo = CRCHi ^ CRCHiTable[Index];
		CRCHi = CRCLoTable[Index];
	}
	shift = (CRCLo << 8) | CRCHi;
	return shift;
}

//u16 CRC16_Check2(u8 *bufdata, u8 length)			//方法2：使用直接结算的方法
//{
//	u16 shift = 0xffff;
//	u8 i,j;
//	for(i=0;i<length;i++)
//	{
//		shift = shift ^ *(bufdata++);
//		for(j=0;j<8;j++)
//		{
//			for((shift & 0x0001) == 0)
//			{
//				shift = shift >> 1;
//				shift = shift ^ 0xA001;
//			}
//		}
//	}	
//	shift = (shift >> 8) ^ (shift << 8);
//	return shift;
//	
//}


//UART1指令控制符号处理
void UART1_Ctrl_Check(void)
{
	u8 i;			//变量发送校验和
	u16 CRC16;		//CRC校验和
	u8  *ptr;			//接收数据首地址
	ptr = (u8 *) & DMA_SEND_BUFF[0];
	switch(DMA_REV_BUFF[1])
	{//控制符号检查
		case 6:		//表示写命令字
			//正常返回通信内容  AD       06     ID[2][3]  Function[4][5]  CRC_L  CRC_H      
			//            		从机地址 写命令字 机构代码    机构功能       CRC16校验码
			//错误返回通信内容  AD       86     ID[2][3]  Reason[4][5]    CRC_L  CRC_H      
			//            		从机地址 错误字   机构代码    错误原因       CRC16校验码
			if(Reason)
			{
				DMA_SEND_BUFF[0] = DMA_REV_BUFF[0];
				DMA_SEND_BUFF[1] = 0x86;
				DMA_SEND_BUFF[2] = DMA_REV_BUFF[2];
				DMA_SEND_BUFF[3] = DMA_REV_BUFF[3];
				DMA_SEND_BUFF[4] = 0;
				DMA_SEND_BUFF[5] = Reason;
				CRC16 = CRC16_Check1(ptr,6);		//CRC校验检查，方法1：查表法
				DMA_SEND_BUFF[6] = (CRC16 & 0xff00) >> 8;
				DMA_SEND_BUFF[7] = CRC16;
				MYDMA_Enable(DMA_UART1_TX,8);				
				RunFlag = 0;								//机构运行标志位：0——不运行 1——运行

//				DMA_Cmd(DMA_UART1_RX,DISABLE);		//关闭DMA,防止处理期间有数据
//				DMA_UART1_RX -> CNDTR = 65535;		//重装填
//				DMA_Cmd(DMA_UART1_RX,ENABLE);			//处理完，重开DMA
			}
			else
			{
				for(i=0;i<UART1.Rev_Length;i++)
				{
					DMA_SEND_BUFF[i] = DMA_REV_BUFF[i];			
				}
				MYDMA_Enable(DMA_UART1_TX,8);
				RunFlag	= 1;					//建立机构运行标志位
			  ID = ((DMA_REV_BUFF[2]<<8) | DMA_REV_BUFF[3]);     //机构代码
			  Distance = ((DMA_REV_BUFF[4]<<8) | (DMA_REV_BUFF[5])) & 0x0fff;   //提升机提升高度
			  SpeedGread = (DMA_REV_BUFF[4] & 0xf0) >> 4;												//电机转速
				GateAction = DMA_REV_BUFF[5];					//闸门动作指令
				DouAction = DMA_REV_BUFF[5];					//药斗动作指令
			}
			break;
		
		case 3:		//查询机构状态命令及版本号
			if(DMA_REV_BUFF[3] == 0x63)
			{//若第4个字节=0x63,读版本号
				DMA_SEND_BUFF[0] = DMA_REV_BUFF[0];								
				DMA_SEND_BUFF[1] = 3;
				DMA_SEND_BUFF[2] = DMA_REV_BUFF[2];
				DMA_SEND_BUFF[3] = DMA_REV_BUFF[3];
				DMA_SEND_BUFF[4] = 0x0B;						
				DMA_SEND_BUFF[5] = 1;						   		//版本号：V11.01 
				CRC16 = CRC16_Check1(ptr,6);		      //CRC校验检查，方法1：查表法
				DMA_SEND_BUFF[6] = (CRC16 & 0xff00) >> 8;
				DMA_SEND_BUFF[7] = CRC16;
				MYDMA_Enable(DMA_UART1_TX,8);				
			}
			
			else
			{//查询机构状态
				DMA_SEND_BUFF[0] = DMA_REV_BUFF[0];								
				DMA_SEND_BUFF[1] = 3;
				DMA_SEND_BUFF[2] = DMA_REV_BUFF[2];
				DMA_SEND_BUFF[3] = DMA_REV_BUFF[3];
				DMA_SEND_BUFF[4] = (MachaStatus << 4) | DouStatus;						
				DMA_SEND_BUFF[5] = (GateStatus << 4) | ServoStatus;						   		
				CRC16 = CRC16_Check1(ptr,6);		//CRC校验检查，方法1：查表法
				DMA_SEND_BUFF[6] = (CRC16 & 0xff00) >> 8;
				DMA_SEND_BUFF[7] = CRC16;
				MYDMA_Enable(DMA_UART1_TX,8);				
				
			}
			
			break;
		
		default:	//控制字错误
			DMA_Cmd(DMA_UART1_RX,DISABLE);		//关闭DMA,防止处理期间有数据
			DMA_UART1_RX -> CNDTR = 65535;		//重装填
			DMA_Cmd(DMA_UART1_RX,ENABLE);			//处理完，重开DMA
			break;		
	}
	
}

	
//UART1通讯处理程序
  void UART1_Com_Check(void)
{
	u16 CRC16;		//CRC校验和
	u8  *ptr;			//接收数据首地址
	ptr = (u8 *)&DMA_REV_BUFF[0];
	
	if (UART1.State == 1)				//UART1.State：通讯响应 作为从机的通信处理标识
	{//串口接收上位机的通讯处理,检查帧头和帧尾
		if(DMA_REV_BUFF[0] == Slave_ID) 
		{//如果第一个字节 = 拨码开关设定值，OK
				CRC16 = CRC16_Check1(ptr,UART1.Rev_Length-2);		//CRC校验检查，方法1：查表法
				
//				CRC16 = CRC16_Check2(ptr,UART1.Rev_Length-2);  	//CRC校验检查，方法2：直接结算法
			  
			if(CRC16 == ((DMA_REV_BUFF[6] << 8) | DMA_REV_BUFF[7]))
			{//CRC校验码正确
				UART1_Ctrl_Check();			//通讯命令处理
			}
			else  //帧头和帧尾检查错误
			{
				DMA_Cmd(DMA_UART1_RX, DISABLE);	//关闭DMA,防止处理期间有数据	
				DMA_UART1_RX -> CNDTR = 65535;	//重新填写
				DMA_Cmd(DMA_UART1_RX,ENABLE);		//处理完，重开DMA
			}
		}
		else
		{//ERROR
			DMA_Cmd(DMA_UART1_RX,DISABLE);		//关闭DMA,防止处理期间有数据		
			DMA_UART1_RX -> CNDTR = 65535;		//重新填写
			DMA_Cmd(DMA_UART1_RX,ENABLE);			//处理完，重开DMA
		}
		UART1.State = 0;
	}
	else  
	{//无通讯
		
	}
	
}

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound)
{
    //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);	//使能USART1，GPIOA时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
 	USART_DeInit(USART1);  //复位串口1
// 	USART_DeInit(USART2);  //复位串口2
	//------------------USART1-------------------------------//
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10


	//------------------USART2-------------------------------//
 //USART2_TX   PA.2
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
// 
//	//USART2_RX	  PA.3
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
//  USART_Init(USART2, &USART_InitStructure); //初始化串口2
		 
//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启中断 空闲中断
  USART_Cmd(USART1, ENABLE);                    //使能串口 
	USART_ClearFlag(USART1, USART_FLAG_TC);		    //解决第一个字节无法发送问题

//  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启中断 空闲中断
//  USART_Cmd(USART2, ENABLE);                    //使能串口 
//	USART_ClearFlag(USART2, USART_FLAG_TC);		    //解决第一个字节无法发送问题

}

//串口1中断服务程序
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //空闲中断
	{		
		//清除IDLE标识
		Res = USART1->SR;
		Res = USART1->DR;
		Res = Res;
		UART1.State = 1;			//建立通讯处理标志位
		
		DMA_Cmd(DMA_UART1_RX, DISABLE);//关闭DMA,防止处理其间有数据
		UART1.Rev_Length = 65535 - DMA_GetCurrDataCounter(DMA_UART1_RX);
//		DMA_UART1_RX->CNDTR = 65535;//重装填
//	  DMA_Cmd(DMA_UART1_RX, ENABLE);//处理完,重开DMA
  } 
	
	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
} 

