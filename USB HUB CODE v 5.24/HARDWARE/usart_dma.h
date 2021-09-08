#ifndef __USART_DMA_H
#define __USART_DMA_H

#define USART2_DMA_MAX_RX_LEN 200
#define USART2_DMA_MAX_TX_LEN 200
#define USART3_DMA_MAX_TX_LEN 200
//u16 USART2_DMA_MAX_TX_LEN = 15*1024;
//u16  USART2_DMA_MAX_RX_LEN = 15*1024;
#include "stm32f10x.h"

void usart2_dma_rx_init(void);//目前使用
void usart2_dma_tx(u32 MemoryBaseAddr,u16 DMA_len);
void usart2_dma_tx_init(void);//目前使用
void DMA_USART2_Tx_Data(u8 *buffer, u32 size);//DMA的发送函数，优势相对于上面的TX发送函数不用每次发送都初始化
void usart3_dma_tx_init(void);
void DMA_USART3_Tx_Data(u8* buffer, u32 size);
#endif

////////void USART2_IRQHandler(void)                	
////////{
////////	extern u8 u1rxbuf[USART2_DMA_MAX_RX_LEN];			
////////  extern u8 u2rxbuf[USART2_DMA_MAX_RX_LEN];			
////////  extern u8 witchbuf;
////////	
////////	u8 *p;
////////	u8 USART2_RX_LEN = 0;		
//////////	u8 clear = 0;
////////	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)			//串口2的空闲中断
////////	{
//////////		clear = USART2->SR; // clear
//////////    clear = USART2->DR; // clear
////////		
////////		
////////		USART_ReceiveData(USART2); 									//串口2接收数据
////////		USART_ClearFlag(USART2,USART_FLAG_TC);
////////		USART_ClearFlag(USART2,USART_FLAG_IDLE);//清除标志位
////////		DMA_Cmd(DMA1_Channel6, DISABLE );   						//关闭串口2的DMA的RX通道
////////		USART2_RX_LEN = USART2_DMA_MAX_RX_LEN - DMA1_Channel6->CNDTR;	//总缓存区大小 - 剩下的缓存区大小（DMA1_Channel6->CNDTR） = 使用掉的缓存区大小
////////		if(witchbuf)                        						//????u2rxbuf,???u1rxbuf
////////		{
////////			p=u2rxbuf;												//先保存前一次的数据地址再切换为另一个数据地址
////////			DMA1_Channel6->CMAR=(u32)u1rxbuf;						//使用u1rxbuf作为DMA的接收数据地址
////////			witchbuf=0;                     						//下次切换为u2rxbuf
////////		}else                               						//之前用的u1rxbuf,现在切换为u2rxbuf
////////		{
////////			p=u1rxbuf;												//先保存前一次的数据地址再切换为另一个数据地址
////////			DMA1_Channel6->CMAR=(u32)u2rxbuf;						//使用u2rxbuf作为DMA的接收数据地址
////////			witchbuf=1;			//下次切换为u1rxbuf
////////			
////////		}
////////		DMA1_Channel6->CNDTR = USART2_DMA_MAX_RX_LEN;					//DMA通道的DMA缓存大小
////////		DMA_Cmd(DMA1_Channel6, ENABLE);     						//开启串口2的DMA接收通道
////////		
////////		//******************?????????????????******************//
////////		
//////////		usart2_dma_tx((u32)p,USART2_RX_LEN);
//////////		DMA_USART2_Tx_Data(p,USART2_RX_LEN);
////////		if	((p[0]=='y')&&(p[1]=='h'))
////////		{
//////////				USART_SendString(USART3, "imedis+Power Board");
//////////			char yh1[]={"GOOD"};
//////////		DMA_USART2_Tx_Data(yh1,sizeof(yh1));
////////			DMA_USART2_Tx_Data(p,USART2_RX_LEN);
////////			}//DMA的中断服务函数，字符串解析输出对应的值
//////////				if	((u1rxbuf[0]=='6')&&(u1rxbuf[1]=='6'))
//////////		{
////////////				USART_SendString(USART3, "imedis+Power Board");
//////////			char yh2[]={"111111"};
//////////		DMA_USART2_Tx_Data(yh2,sizeof(yh2));
//////////			}
//////////		if(strcmp(u1rxbuf,"US")==0)
//////////		{
//////////			char yh2[]={"GOOD"};
//////////			usart2_dma_tx((u32)&yh2,sizeof(yh2));
//////////		}
//////////		usart2_dma_tx((u32)p,USART2_RX_LEN);
////////		//******************?????????????????******************//
//////////			if(strcmp(p,"US")==0)
//////////		{
//////////			char yh2[]={"GOOD"};
//////////			usart2_dma_tx((u32)&yh2,sizeof(yh2));
//////////		}
//////////		
////////		
////////		
////////  }
////////}
//////////DMA的接收以及处理函数，乒乓结构

//中断服务函数例子，配套该DMA的tx和rx

