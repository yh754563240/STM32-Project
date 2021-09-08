#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "sys.h" 
	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define USART_REC_LEN1  			15*1024  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern u8  USART2_RX_BUF[USART_REC_LEN1]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	
extern u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);
void send(u8 channel, int cnt, ...);
void u3_printf(char *fmt, ...);
void SendChar(uint8_t Data);
void SendString(char *str);
void Send_Data(char *fmt, ...);

void USART_SendString(USART_TypeDef* USARTx, char *DataString);
#endif


