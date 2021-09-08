#include "usart_dma.h"
#include "stdio.h"

u8 USART2_DMA_TX_BUF[USART2_DMA_MAX_TX_LEN];
u8 USART3_DMA_TX_BUF[USART3_DMA_MAX_TX_LEN];
u8 u1rxbuf[USART2_DMA_MAX_RX_LEN];			//???????1
u8 u2rxbuf[USART2_DMA_MAX_RX_LEN];			//???????2
u8 witchbuf=0;
u8 USART2_DMA_RX_FLAG=0;
u8 USART2_DMA_TX_FLAG=0;
u8 USART3_DMA_TX_FLAG=0;

void usart2_dma_rx_init()
{
	DMA_InitTypeDef DMA1_Init;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);					
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);		//DMA中断分组请求，DMA的中断优先级要高于串口的中断优先级						

	DMA_DeInit(DMA1_Channel6);											//DMA对于RX而言，外设--内存
	DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);			//DMA的内存地址	
	DMA1_Init.DMA_MemoryBaseAddr = (u32)u1rxbuf;            			//DMA的外设地址
	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralSRC;							//传输方向，外设--内存
	DMA1_Init.DMA_BufferSize = USART2_DMA_MAX_RX_LEN;						//DMA通道的缓存大小
	DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设的地址是否递增（看传输方向，终地址递增）		
	DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存的地址是否递增（看传输方向，终地址递增）	
	DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//设定内存数据宽度
	DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//设定外设数据宽度
	DMA1_Init.DMA_Mode = DMA_Mode_Normal;								//有两种模式，循环缓存模式和正常缓存模式
	DMA1_Init.DMA_Priority = DMA_Priority_High; 						//DMA的优先级
	DMA1_Init.DMA_M2M = DMA_M2M_Disable;								//使能或者关闭或者开启内存到内存的传输模式，（DMA1只能内存与外设）
	DMA_Init(DMA1_Channel6,&DMA1_Init); 	

	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);			//DMA的中断传输完成使能开启或者关闭			
	DMA_Cmd(DMA1_Channel6,ENABLE);           					//使能或者失能DMA1的6通道		
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);  //允许串口2使用DMA接收
}
void usart2_dma_tx(u32 MemoryBaseAddr,u16 DMA_len)
{
	DMA_InitTypeDef DMA1_Init;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);					
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//一般RX的优先级高于TX的优先级			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);		//DMA中断分组请求，DMA的中断优先级要高于串口的中断优先级						

	DMA_DeInit(DMA1_Channel7);											//DMA对于TX而言内存--外设
	DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);			//DMA的内存地址	
	DMA1_Init.DMA_MemoryBaseAddr = MemoryBaseAddr;            			//DMA的外设地址
	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralDST;							//传输方向，内存--外设
	DMA1_Init.DMA_BufferSize = DMA_len;						//DMA通道的缓存大小
	DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设的地址是否递增（看传输方向，终地址递增）		
	DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存的地址是否递增（看传输方向，终地址递增）	
	DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//设定内存数据宽度
	DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//设定外设数据宽度
	DMA1_Init.DMA_Mode = DMA_Mode_Normal;								//有两种模式，循环缓存模式和正常缓存模式
	DMA1_Init.DMA_Priority = DMA_Priority_High; 						//DMA的优先级
	DMA1_Init.DMA_M2M = DMA_M2M_Disable;								//使能或者关闭或者开启内存到内存的传输模式，（DMA1只能内存与外设）
	DMA_Init(DMA1_Channel7,&DMA1_Init); 	

	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);			//DMA的中断传输完成使能开启或者关闭			
	DMA_Cmd(DMA1_Channel7,ENABLE);           					//使能或者失能DMA1的6通道		
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  //允许串口2使用DMA发送
}

void usart2_dma_tx_init()
{
	DMA_InitTypeDef DMA1_Init;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);					
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//一般RX的优先级高于TX的优先级			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);		//DMA中断分组请求，DMA的中断优先级要高于串口的中断优先级						

	DMA_DeInit(DMA1_Channel7);											//DMA对于TX而言内存--外设
	DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);			//DMA的内存地址	
	DMA1_Init.DMA_MemoryBaseAddr = (u32)USART2_DMA_TX_BUF;            			//DMA的外设地址
	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralDST;							//传输方向，内存--外设
	DMA1_Init.DMA_BufferSize = USART2_DMA_MAX_TX_LEN;						//DMA通道的缓存大小
	DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设的地址是否递增（看传输方向，终地址递增）		
	DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存的地址是否递增（看传输方向，终地址递增）	
	DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//设定内存数据宽度
	DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//设定外设数据宽度
	DMA1_Init.DMA_Mode = DMA_Mode_Normal;								//有两种模式，循环缓存模式和正常缓存模式
	DMA1_Init.DMA_Priority = DMA_Priority_High; 						//DMA的优先级
	DMA1_Init.DMA_M2M = DMA_M2M_Disable;								//使能或者关闭或者开启内存到内存的传输模式，（DMA1只能内存与外设）
	DMA_Init(DMA1_Channel7,&DMA1_Init); 	

	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);			//DMA的中断传输完成使能开启或者关闭			
//	DMA_Cmd(DMA1_Channel7,ENABLE);           					//使能或者失能DMA1的6通道		有发送函数的时候，关闭该使能，6.17
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  //允许串口2使用DMA发送
}

void DMA_USART2_Tx_Data(u8* buffer, u32 size)
{
	while(USART2_DMA_TX_FLAG);						//等待上一次TX发送完成，如果USART2_DMA_TX_FLAG=1说明还没发送完成
	USART2_DMA_TX_FLAG=1;							//开始发送USART2_DMA_TX_FLAG = 1
	DMA1_Channel7->CMAR  = (uint32_t)(buffer);	//设置要发送的数据地址
	DMA1_Channel7->CNDTR = size;    			//设置要发送的数据大小
	DMA_Cmd(DMA1_Channel7, ENABLE);				//DMA的发送开启
}

void DMA1_Channel7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC7)!= RESET)	//DMA传输完成标志
	{
		DMA_ClearITPendingBit(DMA1_IT_TC7); 	//清除传输完成标志
		USART_ClearFlag(USART2,USART_FLAG_TC);	//清除串口2的传输完成标志
		DMA_Cmd(DMA1_Channel7, DISABLE );   	//关闭DMA的发送通道
		USART2_DMA_TX_FLAG=0;						//USART2_DMA_TX_FLAG=0，说明一次TX的DMA的数据传输完成
	}
}

////void DMA1_Channel6_IRQHandler(void)
////{
////	u8 *p;
////	if(DMA_GetITStatus(DMA1_IT_TC6)!= RESET)		//DMA??????
////	{
////		DMA_ClearITPendingBit(DMA1_IT_TC6); 		//?????? 
////		USART_ClearFlag(USART2,USART_FLAG_TC);		//??USART2???
////		DMA_Cmd(DMA1_Channel6, DISABLE );   		//??USART2 TX DMA1 ??????
////		if(witchbuf)                        		//????u2rxbuf,???u1rxbuf
////		{
////			p=u2rxbuf;								//????????????????
////			DMA1_Channel6->CMAR=(u32)u1rxbuf;		//???u1rxbuf?????
////			witchbuf=0;                     		//??????u2rxbuf
////		}else                               		//????u1rxbuf,???u2rxbuf
////		{
////			p=u1rxbuf;								//????????????????
////			DMA1_Channel6->CMAR=(u32)u2rxbuf;		//???u2rxbuf?????
////			witchbuf=1;                     		//??????u1rxbuf
////		}
////		DMA1_Channel6->CNDTR = USART2_DMA_MAX_RX_LEN;	//DMA???DMA?????
////		DMA_Cmd(DMA1_Channel6, ENABLE);     		//??USART2 TX DMA1 ??????
////		
////		//******************?????????????????******************//
////		
////		usart2_dma_tx((u32)p,USART2_DMA_MAX_RX_LEN);
////		
////		//******************?????????????????******************//
////		
////	}
////}
void usart3_dma_tx_init()
{
	DMA_InitTypeDef DMA1_Init;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);					
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//一般RX的优先级高于TX的优先级			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);		//DMA中断分组请求，DMA的中断优先级要高于串口的中断优先级						

	DMA_DeInit(DMA1_Channel2);											//DMA对于TX而言内存--外设
	DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);			//DMA的内存地址	
	DMA1_Init.DMA_MemoryBaseAddr = (u32)USART3_DMA_TX_BUF;            			//DMA的外设地址
	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralDST;							//传输方向，内存--外设
	DMA1_Init.DMA_BufferSize = USART3_DMA_MAX_TX_LEN;						//DMA通道的缓存大小
	DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设的地址是否递增（看传输方向，终地址递增）		
	DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存的地址是否递增（看传输方向，终地址递增）	
	DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//设定内存数据宽度
	DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//设定外设数据宽度
	DMA1_Init.DMA_Mode = DMA_Mode_Normal;								//有两种模式，循环缓存模式和正常缓存模式
	DMA1_Init.DMA_Priority = DMA_Priority_High; 						//DMA的优先级
	DMA1_Init.DMA_M2M = DMA_M2M_Disable;								//使能或者关闭或者开启内存到内存的传输模式，（DMA1只能内存与外设）
	DMA_Init(DMA1_Channel2,&DMA1_Init); 	

	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);			//DMA的中断传输完成使能开启或者关闭			
//	DMA_Cmd(DMA1_Channel7,ENABLE);           					//使能或者失能DMA1的6通道		有发送函数的时候，关闭该使能，6.17
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);  //允许串口2使用DMA发送
}
void DMA_USART3_Tx_Data(u8* buffer, u32 size)
{
	while(USART3_DMA_TX_FLAG);						//等待上一次TX发送完成，如果USART2_DMA_TX_FLAG=1说明还没发送完成
	USART3_DMA_TX_FLAG=1;							//开始发送USART2_DMA_TX_FLAG = 1
	DMA1_Channel2->CMAR  = (uint32_t)(buffer);	//设置要发送的数据地址
	DMA1_Channel2->CNDTR = size;    			//设置要发送的数据大小
	DMA_Cmd(DMA1_Channel2, ENABLE);				//DMA的发送开启
}

void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC2)!= RESET)	//DMA传输完成标志
	{
		DMA_ClearITPendingBit(DMA1_IT_TC2); 	//清除传输完成标志
		USART_ClearFlag(USART3,USART_FLAG_TC);	//清除串口2的传输完成标志
		DMA_Cmd(DMA1_Channel2, DISABLE );   	//关闭DMA的发送通道
		USART3_DMA_TX_FLAG=0;						//USART2_DMA_TX_FLAG=0，说明一次TX的DMA的数据传输完成
	}
}