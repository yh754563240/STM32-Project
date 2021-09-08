/*
 * @Descripttion: 
 * @version: 
 * @Author: liuxo
 * @Date: 2020-12-08 08:44:25
 * @LastEditors: Liuxo
 * @LastEditTime: 2020-12-11 20:21:35
 */

#include "uart.h"
#include "stdio.h"

/*****************************************************************************************************
 *                                  Drivers Init
******************************************************************************************************/
static void uart_gpio_init(void)
{
#if(USART1_ENABLE ||  USART2_ENABLE || USART3_ENABLE)
	GPIO_InitTypeDef GPIO_InitStructure;
#if(USART1_ENABLE)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#if(USART2_ENABLE)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#if(USART3_ENABLE)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
#endif	 
#endif	   
}

static void uart_nvic_init(void)
{
#if(USART1_ENABLE ||  USART2_ENABLE || USART3_ENABLE)
  NVIC_InitTypeDef NVIC_InitStructure;
#if(USART1_ENABLE)
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif

#if(USART2_ENABLE)
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
#endif

#if(USART3_ENABLE)	
  /* Enable the USARTz Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif	
#endif		
}

void uart_init(void)
{
#if(USART1_ENABLE ||  USART2_ENABLE || USART3_ENABLE)
	USART_InitTypeDef USART_InitStructure;
#if(USART1_ENABLE)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitStructure.USART_BaudRate = USART1_BOUND;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
#if(USART1_IT_RXNE_ENABLE)
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
#if(USART1_IT_IDLE_ENABLE)
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
#endif
#if(USART1_IT_TC_ENABLE)
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
#endif
    USART_Cmd(USART1, ENABLE);
    USART1->SR&=~(1<<7);
#endif

#if(USART2_ENABLE)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_InitStructure.USART_BaudRate = USART2_BOUND;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
#if(USART2_IT_RXNE_ENABLE)
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif
#if(USART2_IT_IDLE_ENABLE)
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
#endif
#if(USART2_IT_TC_ENABLE)
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
#endif
    USART_Cmd(USART2, ENABLE);
    USART2->SR&=~(1<<7);
#endif

#if(USART3_ENABLE)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    USART_InitStructure.USART_BaudRate = USART3_BOUND;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
#if(USART3_IT_RXNE_ENABLE)
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
#endif
#if(USART3_IT_IDLE_ENABLE)
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
#endif
#if(USART3_IT_TC_ENABLE)
    USART_ITConfig(USART3, USART_IT_TC, ENABLE);
#endif
    USART_Cmd(USART3, ENABLE);
    USART3->SR&=~(1<<7);
#endif

#if(USART1_ENABLE ||  USART2_ENABLE || USART3_ENABLE)
    uart_gpio_init();
    uart_nvic_init();
#endif
#endif
}

#if(USART_FPUTC_ENABLE)
int fputc(int ch,FILE *f)
{
	USART_SendData(USART_FPUTC,ch);
	while (USART_GetFlagStatus(USART_FPUTC, USART_FLAG_TC) == RESET);
	return ch;
}
#endif

/****************************************************************************************
 *                                  中断处理
 ****************************************************************************************/
#if(USART1_ENABLE)
void USART1_IRQHandler(void)
{
	uint8_t data;
    (void)data;
#if(USART1_IT_RXNE_ENABLE)
	if(USART_GetITStatus(USART1,USART_IT_RXNE)) 
	{
		data= USART_ReceiveData(USART1); 
        USART_SendData(USART1,data);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
#endif

#if(USART1_IT_IDLE_ENABLE)
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) 
    {
        data = USART1->SR; // clear
        data = USART1->DR; // clear
    }  
#endif

#if(USART1_IT_TC_ENABLE)
    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC);       // clear
    }
#endif
}
#endif

#if(USART2_ENABLE)
//void USART2_IRQHandler(void)
//{
//	uint8_t data;
//    (void)data;
//#if(USART2_IT_RXNE_ENABLE)
//	if(USART_GetITStatus(USART2,USART_IT_RXNE)) 
//	{
//		data= USART_ReceiveData(USART2); 
//        USART_SendData(USART2,data);
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    }
//#endif

//#if(USART2_IT_IDLE_ENABLE)
//    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
//    {
//        data = USART2->SR; // clear
//        data = USART2->DR; // clear
//    }  
//#endif

//#if(USART2_IT_TC_ENABLE)
//    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
//    {
//        USART_ClearITPendingBit(USART2, USART_IT_TC);       // clear
//    }
//#endif
//}
u8 USART_RX_BUF[USART_REC_LEN] __attribute__ ((at(0X20001000)));//接收缓冲,最大USART_REC_LEN个字节,起始地址为0X20001000.    

u16 USART_RX_STA=0;       	  
u16 USART_RX_CNT=0;			  
void USART2_IRQHandler(void)
{
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//
	{	 
		res=USART_ReceiveData(USART2);
//		USART_SendData(USART2,res);
		if(USART_RX_CNT<USART_REC_LEN)
		{
			USART_RX_BUF[USART_RX_CNT]=res;
			USART_RX_CNT++;
//			printf("0xf38ab = %x",USART_RX_BUF[0x38ab]);
		}
	}
#ifdef OS_CRITICAL_METHOD 	//
	OSIntExit();  											 
#endif
} 
#endif

#if(USART3_ENABLE)
void USART3_IRQHandler(void)
{
	uint8_t data;
    (void)data;
#if(USART3_IT_RXNE_ENABLE)
	if(USART_GetITStatus(USART3,USART_IT_RXNE)) 
	{
		data= USART_ReceiveData(USART3); 
        USART_SendData(USART3,data);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
#endif

#if(USART3_IT_IDLE_ENABLE)
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) 
    {
        data = USART3->SR; // clear
        data = USART3->DR; // clear
    }  
#endif

#if(USART3_IT_TC_ENABLE)
    if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_TC);       // clear
    }
#endif
}
#endif

