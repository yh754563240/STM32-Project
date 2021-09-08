#include "usarttest.h"
void usarttest_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//void USART2_IRQHandler(void)
//{
//	uint8_t data;
//    (void)data;

//	if(USART_GetITStatus(USART2,USART_IT_RXNE)) 
//	{
//		data= USART_ReceiveData(USART2); 
//        USART_SendData(USART2,data);
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    }
//	}

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


		
