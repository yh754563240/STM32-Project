#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "sys.h" 
 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define USART_REC_LEN1  		  15*1024  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u8  USART2_RX_BUF[USART_REC_LEN1]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	
extern u8  USART3_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         		//����״̬���


//����봮���жϽ��գ��벻Ҫע�����º궨��
////void uart_init(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);
void send(u8 channel, int cnt, ...);
void u3_printf(char *fmt, ...);
void SendChar(uint8_t Data);
void SendString(char *str);
void Send_Data(char *fmt, ...);

void USART_SendString(USART_TypeDef* USARTx, char *DataString);
void Send_data1(USART_TypeDef * USARTx,u8 *s);
#endif


