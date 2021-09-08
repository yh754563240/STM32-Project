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
	NVIC_Init(&NVIC_InitStructure);		//DMA�жϷ�������DMA���ж����ȼ�Ҫ���ڴ��ڵ��ж����ȼ�						

	DMA_DeInit(DMA1_Channel6);											//DMA����RX���ԣ�����--�ڴ�
	DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);			//DMA���ڴ��ַ	
	DMA1_Init.DMA_MemoryBaseAddr = (u32)u1rxbuf;            			//DMA�������ַ
	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralSRC;							//���䷽������--�ڴ�
	DMA1_Init.DMA_BufferSize = USART2_DMA_MAX_RX_LEN;						//DMAͨ���Ļ����С
	DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����ĵ�ַ�Ƿ�����������䷽���յ�ַ������		
	DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ�ĵ�ַ�Ƿ�����������䷽���յ�ַ������	
	DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�趨�ڴ����ݿ��
	DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//�趨�������ݿ��
	DMA1_Init.DMA_Mode = DMA_Mode_Normal;								//������ģʽ��ѭ������ģʽ����������ģʽ
	DMA1_Init.DMA_Priority = DMA_Priority_High; 						//DMA�����ȼ�
	DMA1_Init.DMA_M2M = DMA_M2M_Disable;								//ʹ�ܻ��߹رջ��߿����ڴ浽�ڴ�Ĵ���ģʽ����DMA1ֻ���ڴ������裩
	DMA_Init(DMA1_Channel6,&DMA1_Init); 	

	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);			//DMA���жϴ������ʹ�ܿ������߹ر�			
	DMA_Cmd(DMA1_Channel6,ENABLE);           					//ʹ�ܻ���ʧ��DMA1��6ͨ��		
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);  //������2ʹ��DMA����
}
void usart2_dma_tx(u32 MemoryBaseAddr,u16 DMA_len)
{
	DMA_InitTypeDef DMA1_Init;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);					
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//һ��RX�����ȼ�����TX�����ȼ�			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);		//DMA�жϷ�������DMA���ж����ȼ�Ҫ���ڴ��ڵ��ж����ȼ�						

	DMA_DeInit(DMA1_Channel7);											//DMA����TX�����ڴ�--����
	DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);			//DMA���ڴ��ַ	
	DMA1_Init.DMA_MemoryBaseAddr = MemoryBaseAddr;            			//DMA�������ַ
	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralDST;							//���䷽���ڴ�--����
	DMA1_Init.DMA_BufferSize = DMA_len;						//DMAͨ���Ļ����С
	DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����ĵ�ַ�Ƿ�����������䷽���յ�ַ������		
	DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ�ĵ�ַ�Ƿ�����������䷽���յ�ַ������	
	DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�趨�ڴ����ݿ��
	DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//�趨�������ݿ��
	DMA1_Init.DMA_Mode = DMA_Mode_Normal;								//������ģʽ��ѭ������ģʽ����������ģʽ
	DMA1_Init.DMA_Priority = DMA_Priority_High; 						//DMA�����ȼ�
	DMA1_Init.DMA_M2M = DMA_M2M_Disable;								//ʹ�ܻ��߹رջ��߿����ڴ浽�ڴ�Ĵ���ģʽ����DMA1ֻ���ڴ������裩
	DMA_Init(DMA1_Channel7,&DMA1_Init); 	

	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);			//DMA���жϴ������ʹ�ܿ������߹ر�			
	DMA_Cmd(DMA1_Channel7,ENABLE);           					//ʹ�ܻ���ʧ��DMA1��6ͨ��		
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  //������2ʹ��DMA����
}

void usart2_dma_tx_init()
{
	DMA_InitTypeDef DMA1_Init;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);					
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//һ��RX�����ȼ�����TX�����ȼ�			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);		//DMA�жϷ�������DMA���ж����ȼ�Ҫ���ڴ��ڵ��ж����ȼ�						

	DMA_DeInit(DMA1_Channel7);											//DMA����TX�����ڴ�--����
	DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);			//DMA���ڴ��ַ	
	DMA1_Init.DMA_MemoryBaseAddr = (u32)USART2_DMA_TX_BUF;            			//DMA�������ַ
	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralDST;							//���䷽���ڴ�--����
	DMA1_Init.DMA_BufferSize = USART2_DMA_MAX_TX_LEN;						//DMAͨ���Ļ����С
	DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����ĵ�ַ�Ƿ�����������䷽���յ�ַ������		
	DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ�ĵ�ַ�Ƿ�����������䷽���յ�ַ������	
	DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�趨�ڴ����ݿ��
	DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//�趨�������ݿ��
	DMA1_Init.DMA_Mode = DMA_Mode_Normal;								//������ģʽ��ѭ������ģʽ����������ģʽ
	DMA1_Init.DMA_Priority = DMA_Priority_High; 						//DMA�����ȼ�
	DMA1_Init.DMA_M2M = DMA_M2M_Disable;								//ʹ�ܻ��߹رջ��߿����ڴ浽�ڴ�Ĵ���ģʽ����DMA1ֻ���ڴ������裩
	DMA_Init(DMA1_Channel7,&DMA1_Init); 	

	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);			//DMA���жϴ������ʹ�ܿ������߹ر�			
//	DMA_Cmd(DMA1_Channel7,ENABLE);           					//ʹ�ܻ���ʧ��DMA1��6ͨ��		�з��ͺ�����ʱ�򣬹رո�ʹ�ܣ�6.17
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  //������2ʹ��DMA����
}

void DMA_USART2_Tx_Data(u8* buffer, u32 size)
{
	while(USART2_DMA_TX_FLAG);						//�ȴ���һ��TX������ɣ����USART2_DMA_TX_FLAG=1˵����û�������
	USART2_DMA_TX_FLAG=1;							//��ʼ����USART2_DMA_TX_FLAG = 1
	DMA1_Channel7->CMAR  = (uint32_t)(buffer);	//����Ҫ���͵����ݵ�ַ
	DMA1_Channel7->CNDTR = size;    			//����Ҫ���͵����ݴ�С
	DMA_Cmd(DMA1_Channel7, ENABLE);				//DMA�ķ��Ϳ���
}

void DMA1_Channel7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC7)!= RESET)	//DMA������ɱ�־
	{
		DMA_ClearITPendingBit(DMA1_IT_TC7); 	//���������ɱ�־
		USART_ClearFlag(USART2,USART_FLAG_TC);	//�������2�Ĵ�����ɱ�־
		DMA_Cmd(DMA1_Channel7, DISABLE );   	//�ر�DMA�ķ���ͨ��
		USART2_DMA_TX_FLAG=0;						//USART2_DMA_TX_FLAG=0��˵��һ��TX��DMA�����ݴ������
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
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//һ��RX�����ȼ�����TX�����ȼ�			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);		//DMA�жϷ�������DMA���ж����ȼ�Ҫ���ڴ��ڵ��ж����ȼ�						

	DMA_DeInit(DMA1_Channel2);											//DMA����TX�����ڴ�--����
	DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);			//DMA���ڴ��ַ	
	DMA1_Init.DMA_MemoryBaseAddr = (u32)USART3_DMA_TX_BUF;            			//DMA�������ַ
	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralDST;							//���䷽���ڴ�--����
	DMA1_Init.DMA_BufferSize = USART3_DMA_MAX_TX_LEN;						//DMAͨ���Ļ����С
	DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����ĵ�ַ�Ƿ�����������䷽���յ�ַ������		
	DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ�ĵ�ַ�Ƿ�����������䷽���յ�ַ������	
	DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�趨�ڴ����ݿ��
	DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//�趨�������ݿ��
	DMA1_Init.DMA_Mode = DMA_Mode_Normal;								//������ģʽ��ѭ������ģʽ����������ģʽ
	DMA1_Init.DMA_Priority = DMA_Priority_High; 						//DMA�����ȼ�
	DMA1_Init.DMA_M2M = DMA_M2M_Disable;								//ʹ�ܻ��߹رջ��߿����ڴ浽�ڴ�Ĵ���ģʽ����DMA1ֻ���ڴ������裩
	DMA_Init(DMA1_Channel2,&DMA1_Init); 	

	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);			//DMA���жϴ������ʹ�ܿ������߹ر�			
//	DMA_Cmd(DMA1_Channel7,ENABLE);           					//ʹ�ܻ���ʧ��DMA1��6ͨ��		�з��ͺ�����ʱ�򣬹رո�ʹ�ܣ�6.17
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);  //������2ʹ��DMA����
}
void DMA_USART3_Tx_Data(u8* buffer, u32 size)
{
	while(USART3_DMA_TX_FLAG);						//�ȴ���һ��TX������ɣ����USART2_DMA_TX_FLAG=1˵����û�������
	USART3_DMA_TX_FLAG=1;							//��ʼ����USART2_DMA_TX_FLAG = 1
	DMA1_Channel2->CMAR  = (uint32_t)(buffer);	//����Ҫ���͵����ݵ�ַ
	DMA1_Channel2->CNDTR = size;    			//����Ҫ���͵����ݴ�С
	DMA_Cmd(DMA1_Channel2, ENABLE);				//DMA�ķ��Ϳ���
}

void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC2)!= RESET)	//DMA������ɱ�־
	{
		DMA_ClearITPendingBit(DMA1_IT_TC2); 	//���������ɱ�־
		USART_ClearFlag(USART3,USART_FLAG_TC);	//�������2�Ĵ�����ɱ�־
		DMA_Cmd(DMA1_Channel2, DISABLE );   	//�ر�DMA�ķ���ͨ��
		USART3_DMA_TX_FLAG=0;						//USART2_DMA_TX_FLAG=0��˵��һ��TX��DMA�����ݴ������
	}
}