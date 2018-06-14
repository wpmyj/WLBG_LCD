#include "dma.h"
#include "delay.h"
#include "stm32f10x_dma.h"
#include "declare.h"


DMA_InitTypeDef DMA_InitStructure;

//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//Dir:传输方向  0=从内存发送到外设 1=从外设发送到内存
//ex_ph_addr:外设地址
//buff:存储器地址
//length:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u8 Dir,u32 ex_ph_addr,u32 buff,u16 length)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = ex_ph_addr;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = buff;  //DMA内存基地址
	if(Dir)
	{
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设发送到内存
	}
	else
	{
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	}
	DMA_InitStructure.DMA_BufferSize = length;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	  	
} 

//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 Length)
{ 
//	RS485_TEN();
	DMA_Cmd(DMA_UART1_TX, DISABLE );  //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA_UART1_TX,Length);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_UART1_TX, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}	  

 
//DMA 中断初始化
//功能描述：利用DMA把内存的数据传输到串口发送达到高速传输的目的
void DMA_INIT(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
//---------------DMA1 - USART1  
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
  DMA_ITConfig(DMA_UART1_TX,DMA_IT_TC,ENABLE);	
  DMA_ITConfig(DMA_UART1_RX,DMA_IT_TC,ENABLE);	
	 /* Enable USART1 DMA TX request */   
	 USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  
	 /* Enable USART1 DMA RX request */    
	 USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); 

	DMA_Cmd(DMA_UART1_TX,DISABLE);						  //失效DMA发送
	DMA_Cmd(DMA_UART1_RX,ENABLE);								//使能DMA接收

//---------------DMA1 - USART2  
//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	
//  DMA_ITConfig(DMA_UART2_TX,DMA_IT_TC,ENABLE);	
//  DMA_ITConfig(DMA_UART2_RX,DMA_IT_TC,ENABLE);	
//	 /* Enable USART1 DMA TX request */   
//	 USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  
//	 /* Enable USART1 DMA RX request */    
//	 USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); 

//	DMA_Cmd(DMA_UART2_TX,DISABLE);						  //失效DMA发送
//	DMA_Cmd(DMA_UART2_RX,ENABLE);								//使能DMA接收

}

//DMA1_URAT1_TX IRQ
void DMA1_Channel4_IRQHandler()
{
  if(DMA_GetITStatus(DMA1_IT_TC4))
  {
		DMA_Cmd(DMA_UART1_TX, DISABLE);//关闭DMA发送使能
	  DMA_Cmd(DMA_UART1_RX, DISABLE);//关闭DMA,防止处理其间有数据
		DMA_UART1_RX->CNDTR = 65535;//重装填
	  DMA_Cmd(DMA_UART1_RX, ENABLE);//处理完,重开DMA
//		RS485_Delay = 1;							//使能TX切换标识
		DMA_ClearITPendingBit(DMA1_IT_TC4);
  }
}

//DMA1_URAT1_RX IRQ
void DMA1_Channel5_IRQHandler()
{
  if(DMA_GetITStatus(DMA1_IT_TC5))
  {
	  DMA_Cmd(DMA_UART1_RX, DISABLE);//关闭DMA,防止处理其间有数据
	  DMA_UART1_RX->CNDTR = 65535;//重装填
	  DMA_Cmd(DMA_UART1_RX, ENABLE);//处理完,重开DMA
	  DMA_ClearITPendingBit(DMA1_IT_TC5);
  }
		
}

////DMA1_URAT2_TX IRQ
//void DMA1_Channe17_IRQHandler()
//{
//  if(DMA_GetITStatus(DMA1_IT_TC4))
//  {
//		DMA_Cmd(DMA_UART2_TX, DISABLE);//关闭DMA发送使能
//	  DMA_Cmd(DMA_UART2_RX, DISABLE);//关闭DMA,防止处理其间有数据
//		DMA_UART2_RX->CNDTR = 65535;//重装填
//	  DMA_Cmd(DMA_UART2_RX, ENABLE);//处理完,重开DMA

//		DMA_ClearITPendingBit(DMA1_IT_TC4);
//  }
//}

////DMA1_URAT2_RX IRQ
//void DMA1_Channe16_IRQHandler()
//{
//  if(DMA_GetITStatus(DMA1_IT_TC5))
//  {
//	  DMA_Cmd(DMA_UART2_RX, DISABLE);//关闭DMA,防止处理其间有数据
//	  DMA_UART2_RX->CNDTR = 65535;//重装填
//	  DMA_Cmd(DMA_UART2_RX, ENABLE);//处理完,重开DMA
//	  DMA_ClearITPendingBit(DMA1_IT_TC5);
//  }
//		
//}

