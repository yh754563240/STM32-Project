
#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"


/* enable usart */
#define USART1_ENABLE            (0)
#define USART2_ENABLE            (1)
#define USART3_ENABLE            (0)

/* enable fputc */
#if(USART1_ENABLE ||  USART2_ENABLE || USART3_ENABLE)
#define USART_FPUTC_ENABLE       (1)
#define USART_FPUTC              (USART2)
#endif

/* usart1 setting */
#if(USART1_ENABLE)
#define USART1_BOUND             (9600)
#define USART1_IT_RXNE_ENABLE    (1)
#define USART1_IT_IDLE_ENABLE    (0)
#define USART1_IT_TC_ENABLE      (0)
#endif

/* usart2 setting */
#if(USART2_ENABLE)
#define USART2_BOUND             (115200)
#define USART2_IT_RXNE_ENABLE    (1)
#define USART2_IT_IDLE_ENABLE    (1)
#define USART2_IT_TC_ENABLE      (0)
#endif

/* usart3 setting */
#if(USART3_ENABLE)
#define USART3_BOUND             (115200)
#define USART3_IT_RXNE_ENABLE    (1)
#define USART3_IT_IDLE_ENABLE    (0)
#define USART3_IT_TC_ENABLE      (0)
#endif


// uart init
void uart_init(void);
void USART_SendString(USART_TypeDef* USARTx, char *DataString);

#endif /* __UART_H */
