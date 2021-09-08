#ifndef __USART_DMA_H
#define __USART_DMA_H

#define USART2_DMA_MAX_RX_LEN 200
#define USART2_DMA_MAX_TX_LEN 200
#define USART3_DMA_MAX_TX_LEN 200
//u16 USART2_DMA_MAX_TX_LEN = 15*1024;
//u16  USART2_DMA_MAX_RX_LEN = 15*1024;
#include "stm32f10x.h"

void usart2_dma_rx_init(void);//Ŀǰʹ��
void usart2_dma_tx(u32 MemoryBaseAddr,u16 DMA_len);
void usart2_dma_tx_init(void);//Ŀǰʹ��
void DMA_USART2_Tx_Data(u8 *buffer, u32 size);//DMA�ķ��ͺ�������������������TX���ͺ�������ÿ�η��Ͷ���ʼ��
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
////////	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)			//����2�Ŀ����ж�
////////	{
//////////		clear = USART2->SR; // clear
//////////    clear = USART2->DR; // clear
////////		
////////		
////////		USART_ReceiveData(USART2); 									//����2��������
////////		USART_ClearFlag(USART2,USART_FLAG_TC);
////////		USART_ClearFlag(USART2,USART_FLAG_IDLE);//�����־λ
////////		DMA_Cmd(DMA1_Channel6, DISABLE );   						//�رմ���2��DMA��RXͨ��
////////		USART2_RX_LEN = USART2_DMA_MAX_RX_LEN - DMA1_Channel6->CNDTR;	//�ܻ�������С - ʣ�µĻ�������С��DMA1_Channel6->CNDTR�� = ʹ�õ��Ļ�������С
////////		if(witchbuf)                        						//????u2rxbuf,???u1rxbuf
////////		{
////////			p=u2rxbuf;												//�ȱ���ǰһ�ε����ݵ�ַ���л�Ϊ��һ�����ݵ�ַ
////////			DMA1_Channel6->CMAR=(u32)u1rxbuf;						//ʹ��u1rxbuf��ΪDMA�Ľ������ݵ�ַ
////////			witchbuf=0;                     						//�´��л�Ϊu2rxbuf
////////		}else                               						//֮ǰ�õ�u1rxbuf,�����л�Ϊu2rxbuf
////////		{
////////			p=u1rxbuf;												//�ȱ���ǰһ�ε����ݵ�ַ���л�Ϊ��һ�����ݵ�ַ
////////			DMA1_Channel6->CMAR=(u32)u2rxbuf;						//ʹ��u2rxbuf��ΪDMA�Ľ������ݵ�ַ
////////			witchbuf=1;			//�´��л�Ϊu1rxbuf
////////			
////////		}
////////		DMA1_Channel6->CNDTR = USART2_DMA_MAX_RX_LEN;					//DMAͨ����DMA�����С
////////		DMA_Cmd(DMA1_Channel6, ENABLE);     						//��������2��DMA����ͨ��
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
////////			}//DMA���жϷ��������ַ������������Ӧ��ֵ
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
//////////DMA�Ľ����Լ���������ƹ�ҽṹ

//�жϷ��������ӣ����׸�DMA��tx��rx

