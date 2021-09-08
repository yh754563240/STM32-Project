#include "sys.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "stdarg.h"
#include "usart.h"
#include "receive_agreement.h"
#include "usart_dma.h"

#define USART3_MAX_RECV_LEN 200
#define USART3_MAX_SEND_LEN 200
#define _head_1 0x23        //Ö¡Í·'#'
#define _head_2 0x40        //Ö¡Í·'@'

char send_str[256];
int RX_Value = 0;
extern char data[20];
char data1[20];


u8 USART2_RX_BUF[USART_REC_LEN1];//½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
u16 USART2_RX_STA = 0; //½ÓÊÕ×´Ì¬±ê¼Ç

u8 USART3_RX_BUF[USART_REC_LEN];//½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
u16 USART3_RX_STA = 0; //½ÓÊÕ×´Ì¬±ê¼

//////////////////////////////////////////////////////////////////////////////////
//Èç¹ûÊ¹ÓÃucos,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼þ¼´¿É.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos Ê¹ÓÃ
#endif


//////////////////////////////////////////////////////////////////////////////////
//´®¿Ú·¢ËÍ»º´æÇø
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; //·¢ËÍ»º³å,×î´óUSART3_MAX_SEND_LEN×Ö½Ú
#ifdef USART3_RX_EN
#endif								   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; //½ÓÊÕ»º³å,×î´óUSART3_MAX_RECV_LEN¸ö×Ö½Ú.

#define channel_number 3
unsigned char DataScope_OutPut_Buffer[42] = {0}; //´®¿Ú·¢ËÍ»º³åÇø
u8 pstr[channel_number * 4 + 1];				 //
int state;
//////////////////////////////////////////////////////////////////
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êý,¶ø²»ÐèÒªÑ¡Ôñuse MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//±ê×¼¿âÐèÒªµÄÖ§³Öº¯Êý
struct __FILE
{
	int handle;
};

FILE __stdout;
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½
void _sys_exit(int x)
{
	x = x;
}
void _ttywrch(int x)
{
	x = x;
}

//ÖØ¶¨Òåfputcº¯Êý
int fputc(int ch, FILE *f)
{
	while ((USART2->SR & 0X40) == 0)
		; //Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï
	USART2->DR = (u8)ch;
	return ch;
}
#endif

#if EN_USART1_RX //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó
u8 USART_RX_BUF[USART_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓÐÐ§×Ö½ÚÊýÄ¿
u16 USART_RX_STA = 0; //½ÓÊÕ×´Ì¬±ê¼Ç

//³õÊ¼»¯IO ´®¿Ú1
//bound:²¨ÌØÂÊ
void uart_init(u32 bound)
{
	//GPIO¶Ë¿ÚÉèÖÃ
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //Ê¹ÄÜGPIOAÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //Ê¹ÄÜUSART1Ê±ÖÓ


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
        
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;										//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;								//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//ÊÕ·¢Ä£Ê½
	USART_Init(USART1, &USART_InitStructure);										//³õÊ¼»¯´®¿Ú1

	USART_Cmd(USART1, ENABLE); //Ê¹ÄÜ´®¿Ú1

	//USART_ClearFlag(USART1, USART_FLAG_TC);

#if EN_USART1_RX
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //¿ªÆôÏà¹ØÖÐ¶Ï

	//Usart1 NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //´®¿Ú1ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);							  //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢

#endif

	//USART_ClearFlag(USART1, USART_FLAG_TC);
}
//³õÊ¼»¯IO ´®¿Ú2
//pclk1:PCLK1Ê±ÖÓÆµÂÊ(Mhz)
//bound:²¨ÌØÂÊ

void usart2_init(u32 bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //Ê¹ÄÜGPIOAÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //Ê¹ÄÜUSART2Ê±ÖÓ

	//USART1_TX   PA.2 PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;										//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;								//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// ·¢Ä£Ê½
	USART_Init(USART2, &USART_InitStructure);										//³õÊ¼»¯´®¿Ú
	USART_Cmd(USART2, ENABLE);														//Ê¹ÄÜ´®¿Ú
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//¿ªÆôÏà¹ØÖÐ¶Ï
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	//Usart1 NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  //´®¿Ú1ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);							  //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢
}

void usart3_init(u32 bound)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);

	USART_DeInit(USART3); //¸´Î»´®¿Ú3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //³õÊ¼»¯GPIOB11£¬ºÍGPIOB10

	USART_InitStructure.USART_BaudRate = bound;										//²¨ÌØÂÊ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;								//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//ÊÕ·¢Ä£Ê½

	USART_Init(USART3, &USART_InitStructure); //³õÊ¼»¯´®¿Ú3

	USART_Cmd(USART3, ENABLE); //Ê¹ÄÜ´®¿Ú

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //¿ªÆôÖÐ¶Ï
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //ÇÀÕ¼ÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);							  //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
}

void USART1_IRQHandler(void) //´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
	u8 Res;
#if SYSTEM_SUPPORT_OS //Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòÐèÒªÖ§³ÖOS.
	OSIntEnter();
#endif
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{
		Res = USART_ReceiveData(USART1); //(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý

		if ((USART_RX_STA & 0x8000) == 0) //½ÓÊÕÎ´Íê³É
		{
			if (USART_RX_STA & 0x4000) //½ÓÊÕµ½ÁË0x0d
			{
				if (Res != 0x0a)
					USART_RX_STA = 0; //½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else
					USART_RX_STA |= 0x8000; //½ÓÊÕÍê³ÉÁË
			}
			else //»¹Ã»ÊÕµ½0X0D
			{
				if (Res == 0x0d)
					USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
						USART_RX_STA = 0; //½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ
				}
			}
		}
	}
#if SYSTEM_SUPPORT_OS //Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòÐèÒªÖ§³ÖOS.
	OSIntExit();
#endif
}

int UART_Value(void)
{
	int reg;
	if (RX_Value)
	{
		reg = RX_Value;
		RX_Value = 0;
	}
	return reg;
}


////////void USART2_IRQHandler(void)                	
////////{
////////	extern u8 u1rxbuf[USART2_DMA_MAX_RX_LEN];			
////////  extern u8 u2rxbuf[USART2_DMA_MAX_RX_LEN];			
////////  extern u8 witchbuf;
////////	
////////	u8 *p;
////////	u8 USART2_RX_LEN = 0;		
//////////	u8 clear = 0;
////////	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)			//´®¿Ú2µÄ¿ÕÏÐÖÐ¶Ï
////////	{
//////////		clear = USART2->SR; // clear
//////////    clear = USART2->DR; // clear
////////		
////////		
////////		USART_ReceiveData(USART2); 									//´®¿Ú2½ÓÊÕÊý¾Ý
////////		USART_ClearFlag(USART2,USART_FLAG_TC);
////////		USART_ClearFlag(USART2,USART_FLAG_IDLE);//Çå³ý±êÖ¾Î»
////////		DMA_Cmd(DMA1_Channel6, DISABLE );   						//¹Ø±Õ´®¿Ú2µÄDMAµÄRXÍ¨µÀ
////////		USART2_RX_LEN = USART2_DMA_MAX_RX_LEN - DMA1_Channel6->CNDTR;	//×Ü»º´æÇø´óÐ¡ - Ê£ÏÂµÄ»º´æÇø´óÐ¡£¨DMA1_Channel6->CNDTR£© = Ê¹ÓÃµôµÄ»º´æÇø´óÐ¡
////////		if(witchbuf)                        						//????u2rxbuf,???u1rxbuf
////////		{
////////			p=u2rxbuf;												//ÏÈ±£´æÇ°Ò»´ÎµÄÊý¾ÝµØÖ·ÔÙÇÐ»»ÎªÁíÒ»¸öÊý¾ÝµØÖ·
////////			DMA1_Channel6->CMAR=(u32)u1rxbuf;						//Ê¹ÓÃu1rxbuf×÷ÎªDMAµÄ½ÓÊÕÊý¾ÝµØÖ·
////////			witchbuf=0;                     						//ÏÂ´ÎÇÐ»»Îªu2rxbuf
////////		}else                               						//Ö®Ç°ÓÃµÄu1rxbuf,ÏÖÔÚÇÐ»»Îªu2rxbuf
////////		{
////////			p=u1rxbuf;												//ÏÈ±£´æÇ°Ò»´ÎµÄÊý¾ÝµØÖ·ÔÙÇÐ»»ÎªÁíÒ»¸öÊý¾ÝµØÖ·
////////			DMA1_Channel6->CMAR=(u32)u2rxbuf;						//Ê¹ÓÃu2rxbuf×÷ÎªDMAµÄ½ÓÊÕÊý¾ÝµØÖ·
////////			witchbuf=1;			//ÏÂ´ÎÇÐ»»Îªu1rxbuf
////////			
////////		}
////////		DMA1_Channel6->CNDTR = USART2_DMA_MAX_RX_LEN;					//DMAÍ¨µÀµÄDMA»º´æ´óÐ¡
////////		DMA_Cmd(DMA1_Channel6, ENABLE);     						//¿ªÆô´®¿Ú2µÄDMA½ÓÊÕÍ¨µÀ
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
////////			}//DMAµÄÖÐ¶Ï·þÎñº¯Êý£¬×Ö·û´®½âÎöÊä³ö¶ÔÓ¦µÄÖµ
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
//////////DMAµÄ½ÓÊÕÒÔ¼°´¦Àíº¯Êý£¬Æ¹ÅÒ½á¹¹


void USART2_IRQHandler(void) //´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
u8 clear=clear;
u8 Res;
u16 len;	
clear = 0;
	
u16 USART2_RX_STA_FLAG;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{
		Res = USART_ReceiveData(USART2); //(USART2->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
//		USART_SendData (USART2,Res+1);/////////
//		Receive_Data(Res ,data);
//		if (Res == '$')
//		{
//			state = channel_number * 4 + 1;
//			state--;
//		}
//		else
//		{
//			if (state > 0)
//				pstr[(channel_number * 4 + 1) - state] = Res;
//			state--;
//		}
		
		
//////////		if(count<USART_REC_LEN1)
//////////		{
//////////		USART2_RX_BUF [count] = Res;
//////////		count++;
//////////		}
		
		
		
		
//////////		if ((USART2_RX_STA & 0x8000) == 0) //½ÓÊÕÎ´Íê³É
//////////		{
//////////			
//////////				
//////////				
//////////			if (USART2_RX_STA & 0x4000) //½ÓÊÕµ½ÁË0x0d
//////////			{
//////////				if (Res != 0x0a)
//////////					USART2_RX_STA = 0; //½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
//////////				else
//////////					USART2_RX_STA |= 0x8000; //½ÓÊÕÍê³ÉÁË
//////////			}
//////////			
//////////			
//////////			
//////////			else //»¹Ã»ÊÕµ½0X0D
//////////			{
//////////				
////////////if(USART2_RX_STA == 0x38bf)
////////////{
////////////	USART2_RX_STA |= 0xC000;

//////////	
//////////				if (Res == 0x0d)
//////////					USART2_RX_STA |= 0x4000;

//////////			
//////////	
//////////				else
//////////				{
//////////					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
//////////					USART2_RX_STA++;
//////////					if (USART2_RX_STA > (USART_REC_LEN1 - 1))
//////////						USART2_RX_STA = 0; //½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ
//////////				}
//////////			}
//////////			
//////////		}//Ô­Ê¼6.25




//////////		if( USART2_RX_STA<0x001e || (USART2_RX_STA & 0x4000) == 0x4000)
//////////		{
//////////				if ((USART2_RX_STA & 0x8000) == 0) //½ÓÊÕÎ´Íê³É
//////////		{
//////////			
//////////		

//////////				
//////////			if (USART2_RX_STA & 0x4000) //½ÓÊÕµ½ÁË0x0d
//////////			{
//////////				if (Res != 0x0a)
//////////					USART2_RX_STA = 0; //½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
//////////				else
//////////					USART2_RX_STA |= 0x8000; //½ÓÊÕÍê³ÉÁË
//////////			}
//////////		
//////////			
//////////			
//////////			else //»¹Ã»ÊÕµ½0X0D
//////////			{
//////////				
////////////if(USART2_RX_STA == 0x38bf)
////////////{
////////////	USART2_RX_STA |= 0xC000;
//////////// if(USART2_RX_STA < 0x001e)
//////////// {
////////////				if(USART2_RX_STA<0x001e)
////////////				{
//////////				if (Res == 0x0d)
//////////				{
//////////					USART2_RX_STA |= 0x4000;
//////////				}
////////////			}
////////////			}
////////////			else if(USART2_RX_STA>0x001e)
////////////			{
////////////				
////////////				if(USART2_RX_STA == 0x38bf)
////////////				{
////////////					USART2_RX_STA |= 0xC000;
////////////		      USART2_RX_STA &= 0xC000;
////////////				}
////////////			}

////////////else if(USART2_RX_STA > 0x001e)
////////////	
////////////{
////////////	if(USART2_RX_STA == 0x38bf)
////////////	{
////////////		USART2_RX_STA |= 0xC000;
////////////		USART2_RX_STA &= 0xC000;
////////////	}
////////////}
//////////				else 
//////////				{
//////////					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
//////////					USART2_RX_STA++;
//////////					
//////////					if (USART2_RX_STA > (USART_REC_LEN1 - 1))
//////////						USART2_RX_STA = 0; //½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ
//////////					
//////////				}
//////////				
//////////				
//////////			}
//////////			
//////////		}
//////////	}
//////////		
//////////		else if(USART2_RX_STA>0x001e)
//////////		{
//////////			
//////////			
//////////			
//////////			if(USART2_RX_STA == 0x38bf)
//////////			{
//////////					USART2_RX_STA |= 0xC000;
//////////		      USART2_RX_STA &= 0xC000;
//////////			}
//////////			else
//////////			{
//////////				USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
//////////					USART2_RX_STA++;
//////////					
//////////					if (USART2_RX_STA > (USART_REC_LEN1 - 1))
//////////						USART2_RX_STA = 0; //½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ
//////////				}
//////////			


//////////				
//////////				
//////////		}


		if ((USART2_RX_STA & 0x8000) == 0) //½ÓÊÕÎ´Íê³É
		{
			
			if (USART2_RX_STA & 0x4000) //ÖÃÁË±êÖ¾Î»0x4000£¬Ö¤Ã÷½ÓÊÕµ½ÁË0x0d£¬È»ºóÅÐ¶ÏÁ¬ÐøµÄ0x0a£¬ÊÇµÄ»°¾Í½ÓÊÕÕýÈ·£¬·ñÔò´íÎó
			{
				if(USART2_RX_STA < 0x4014) //ÅÐ¶ÏÊÇ·ñÖÃÁË±êÖ¾Î»0x4000ºóµÄÊý¾Ý»¹ÊÇ·ñÐ¡ÓÚ20(0x0014)£¬Ð¡ÓÚµÄ»°Ä¬ÈÏÎªÊÇÒ»¸öATÖ¸Áî×Ö·û
				{
				if (Res != 0x0a)
					USART2_RX_STA = 0; //½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else
					USART2_RX_STA |= 0x8000; //½ÓÊÕÍê³ÉÁË
			}
				
			}

			
			
			else //»¹Ã»ÊÕµ½0X0D
			{
			
				if (USART2_RX_STA < 0x0014 && Res == 0x0d)  //Êý¾Ý³¤¶ÈÐ¡ÓÚ20(0x0014)¶øÇÒResµÈÓÚ0x0dÊ±£¬ÕâÀïµÄ0x0d£¬ÊÇÉÏÎ»»úÍ¨¹ý´®¿Úµ÷ÊÔÖúÊÖ·¢ËÍ¹ýÀ´µÄÊý¾ÝÄ©Î²×ÜÊÇ¸ú×Å 0x0d£¨»Ø³µ·û£© 0x0a£¨»»ÐÐ·û)
				                                            //²¢ÇÒ0x0d£¨»Ø³µ·û£© 0x0a£¨»»ÐÐ·û)×ÜÊÇÁ¬Ðø£¬µ±È»Ö¸µÄÊÇ´ÓÉÏÎ»»ú·¢ËÍÀ´µÄÊý¾Ý
				{
					USART2_RX_STA |= 0x4000; //ÉÏÊöÌõ¼þ³ÉÁ¢Ê±ÖÃÒ»¸ö±êÖ¾Î»
				}
//				else if ((USART2_RX_STA > 0x0014 ) && (USART2_RX_STA & 0xF000 == 0x3000))
				else if (USART2_RX_STA > 0x0014 && (USART2_RX_STA == 0x28DA))
//	else if (USART2_RX_STA > 0x0014 && USART2_RX_STA == 0x3887)				//Êý¾Ý³¤¶È´óÓÚ20(0x0014)¶øÇÒUSART2_RX_STA == 0x38bf£¬0x38bfÎªPower Board´«ÊäÍêÕûÐÔµÄ±êÖ¾Î»ÅÐ¶Ï£¨0x38bfÎª¸ÃBINÎÄ¼þµÄ¶þ½øÖÆ³¤¶È£©

				{
					USART2_RX_STA |= 0xC000; //ÉÏÊöÌõ¼þ³ÉÁ¢Ê±ÖÃÒ»¸ö±êÖ¾Î»
				}
	
				else
				{
					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
					USART2_RX_STA++;
					if (USART2_RX_STA > (USART_REC_LEN1 - 1))
						USART2_RX_STA = 0; //½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ
				}
			}
			
		}//Ô­Ê¼6.29
	








	}
	
	
	
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
    {
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);
			

			
if((USART2_RX_STA_FLAG = USART2_RX_STA & 0xC000)) //½ÓÊÕÍê³É
	{
		if(USART2_RX_STA_FLAG == 0xC000 )//½ÓÊÕÍê³É±êÖ¾
		{
			len=USART2_RX_STA & 0x3fff;//
			USART2_RX_STA=0;	//USART2_RX_STA±êÖ¾Î»ÖÃ0	
			USART2_RX_BUF[len]=0;//³¤¶ÈlenµÄÎ»´ÎÖÃ0
//		memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));
		}
	}
        clear = USART2->SR; // clear
        clear = USART2->DR; // clear
	
			if(strcmp(USART2_RX_BUF,"imedis+HUB+V")==0) //?????
			{
				USART_SendString(USART2, "USB HUB v1.0.1");
			}
			if(strcmp(USART2_RX_BUF,"imedis+HUB+UPDATA")==0) //?????
			{
				USART_SendString(USART2,"USB HUB½øÈë¸üÐÂÄ£Ê½£¡£¡");
				SoftReset();
			}
			if(strcmp(USART2_RX_BUF,"imedis+HUB+UPDATA1")==0) //?????
			{
				extern u8 flag_bootload;
				USART_SendString(USART2,"USB HUB JUMP½øÈë¸üÐÂÄ£Ê½£¡£¡");
				flag_bootload = 6;
			}
				if(strcmp(USART2_RX_BUF,"imedis+PB+UPDATA")==0) //?????
			{
				USART_SendString(USART3,"imedis+PB+UPDATA");
			}
			if(strcmp(USART2_RX_BUF,"imedis+PB+UPDATA1")==0) //?????
			{
				USART_SendString(USART3,"imedis+PB+UPDATA1");
			}
			if(strcmp(USART2_RX_BUF,"imedis+PB+V")==0) //?????
			{
				USART_SendString(USART3, "imedis+PB+V");
			}
		 if	((USART2_RX_BUF[0]=='U')&&(USART2_RX_BUF[1]=='S'))
			{
//				USART_SendString(USART2,USART2_RX_BUF);
				char sen1[] = {"11"};
				DMA_USART2_Tx_Data(sen1,sizeof(sen1));
//				DMA_USART2_Tx_Data(USART2_RX_BUF,sizeof(USART2_RX_BUF));
			}
			
			if	((USART2_RX_BUF[0]==0x10)&&(USART2_RX_BUF[1]==0x47))
			{
				char sen2[] = {"DMA33"};
//				USART_SendString(USART2, USART2_RX_BUF);
//				USART_SendString(USART3, sen2);
//				u3_printf(sen2);
////				DMA_USART2_Tx_Data(USART2_RX_BUF,len);
//				DMA_USART3_Tx_Data(sen2,sizeof(sen2));
				DMA_USART3_Tx_Data(USART2_RX_BUF,len);
//				Send_Data("%s", sen2);
//				USART_SendString(USART2 ,USART2_RX_BUF);
//				USART_SendData (USART3 ,USART2_RX_BUF);
//				Send_data1(USART2,USART2_RX_BUF);
//				printf("%c",USART2_RX_BUF);
//				Send_Data("%c", USART2_RX_BUF);
			}
			
			
			
			


//			if	((USART2_RX_BUF[0]=='y')&&(USART2_RX_BUF[1]=='h'))
//{
//	printf("%s",USART2_RX_BUF);
//}//ATÖ¸Áî´¦Àí×Ö·û´®
    }  //???RX??BUFF????
	
	
	
	
}


//void USART2_IRQHandler(void)
//{
//	u8 res;
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //½ÓÊÕµ½Êý¾Ý
//	{
//		res = USART_ReceiveData(USART2);
//   USART_SendData(USART3 , res+2);
////		Receive_Data(res ,data);	
//     	
//	}
//}


int receive(int data[])
{
	union {
		unsigned char float_byte[sizeof(int)];

		int value;
	} f_b;
	int i;
	int cnt = channel_number;
	if (state == 0)
	{
		for (i = 0; i < cnt; ++i)
		{
			f_b.float_byte[0] = pstr[4 * i + 1];
			f_b.float_byte[1] = pstr[4 * i + 2];
			f_b.float_byte[2] = pstr[4 * i + 3];
			f_b.float_byte[3] = pstr[4 * i + 4];
			data[i] = f_b.value;
		}
		state = channel_number * 4 + 1;
		return 1;
	}
	else
		for (i = 0; i < cnt; ++i)
			data[i] = 0;
	return 0;
}

void send(u8 channel, int cnt, ...)
{
	union {
		u8 float_byte[sizeof(u16)];
		u16 value;
	} f_b;
	int i;
	const int size = 2;
	u8 str[size * cnt + 5];
	va_list ap;
	va_start(ap, cnt);
	str[0] = 0xAA;
	str[1] = 0xAA;
	str[2] = channel;
	str[3] = cnt * size;
	str[size * cnt + 4] = 0;
	for (i = 0; i < cnt; ++i)
	{
		f_b.value = va_arg(ap, int);
		str[size * i + 4] = f_b.float_byte[1];
		str[size * i + 5] = f_b.float_byte[0];
	}
	va_end(ap);
	for (i = 0; i < size * cnt + 4; i++)
	{
		str[size * cnt + 4] = (str[size * cnt + 4] + str[i]) & 0xff; //?????
		while ((USART1->SR & 0X40) == 0)
			; //Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï
		USART1->DR = (u8)str[i];
	}
	while ((USART1->SR & 0X40) == 0)
		; //Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï
	USART1->DR = (u8)str[size * cnt + 4];
}

//Í¨¹ýÅÐ¶Ï½ÓÊÕÁ¬Ðø2¸ö×Ö·ûÖ®¼äµÄÊ±¼ä²î²»´óÓÚ100msÀ´¾ö¶¨ÊÇ²»ÊÇÒ»´ÎÁ¬ÐøµÄÊý¾Ý.
//Èç¹û2¸ö×Ö·û½ÓÊÕ¼ä¸ô³¬¹ý100ms,ÔòÈÏÎª²»ÊÇ1´ÎÁ¬ÐøÊý¾Ý.Ò²¾ÍÊÇ³¬¹ý100msÃ»ÓÐ½ÓÊÕµ½
//ÈÎºÎÊý¾Ý,Ôò±íÊ¾´Ë´Î½ÓÊÕÍê±Ï.
//½ÓÊÕµ½µÄÊý¾Ý×´Ì¬
//[15]:0,Ã»ÓÐ½ÓÊÕµ½Êý¾Ý;1,½ÓÊÕµ½ÁËÒ»ÅúÊý¾Ý.
//[14:0]:½ÓÊÕµ½µÄÊý¾Ý³¤¶È
void USART3_IRQHandler(void)
{
	u8 clear=clear;
u8 res;
u8 len;	
clear = 0;
	

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //½ÓÊÕµ½Êý¾Ý
	{
		res = USART_ReceiveData(USART3);
		Receive_Data(res ,data);

//   USART_SendData(USART2 , res+2);
//		Receive_Data(res ,data);
		if ((USART3_RX_STA & 0x8000) == 0) //½ÓÊÕÎ´Íê³É
		{
			if (USART3_RX_STA & 0x4000) //½ÓÊÕµ½ÁË0x0d
			{
				if (res != 0x0a)
					USART3_RX_STA = 0; //½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else
					USART3_RX_STA |= 0x8000; //½ÓÊÕÍê³ÉÁË
			}
			else //»¹Ã»ÊÕµ½0X0D
			{
				if (res == 0x0d)
					USART3_RX_STA |= 0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = res;
					USART3_RX_STA++;
					if (USART3_RX_STA > (USART_REC_LEN - 1))
						USART3_RX_STA = 0; //½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ
				}
			}
		}

//				Receive_Data(res ,data);	
	}
	

		
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) 
    {
			USART_ClearITPendingBit(USART3, USART_IT_IDLE);
			

			
if((USART3_RX_STA |= 0x8000)) //½ÓÊÕÍê³É
	{
			len=USART3_RX_STA & 0x3fff;//½ÓÊÕµ½µÄ³¤¶È
			USART3_RX_STA=0;	//½ÓÊÕ±ê¼ÇÇåÁã			
			USART3_RX_BUF[len] = 0;//½ÓÊÕ»º´æÇøºóÃæÇåÁã
//			memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
	}
	
        clear = USART3->SR; // clear
        clear = USART3->DR; // clear
	
//			if	((USART3_RX_BUF[0]!='#')&&(USART3_RX_BUF[1]!='@'))
//			{

			if(strcmp(USART3_RX_BUF,"Power Board v1.0.1")==0) //?????
			{
				USART_SendString(USART2, "Power Board v1.0.1");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"PW½øÈë¸üÐÂÄ£Ê½³É¹¦£¡£¡5ÃëÄÚÎÞÐÂ³ÌÐò×Ô¶¯Ö´ÐÐ¾É³ÌÐò")==0) //?????
			{
				USART_SendString(USART2, "PW½øÈë¸üÐÂÄ£Ê½³É¹¦£¡£¡5ÃëÄÚÎÞÐÂ³ÌÐò×Ô¶¯Ö´ÐÐ¾É³ÌÐò");
//				printf("Power Board v1.0");
			}
				if(strcmp(USART3_RX_BUF,"PB¹Ì¼þ¸üÐÂÍê³É!")==0) //?????
			{
				USART_SendString(USART2, "PB¹Ì¼þ¸üÐÂÍê³É£¡");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"¿ªÊ¼Ö´ÐÐPB¸üÐÂ¹Ì¼þ´úÂë!!")==0) //?????
			{
				USART_SendString(USART2, "¿ªÊ¼Ö´ÐÐPB¸üÐÂ¹Ì¼þ´úÂë!!");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"Ã»ÓÐ¿ÉÒÔ¸üÐÂµÄ¹Ì¼þ,PB¿ªÊ¼Ö´ÐÐ¾É³ÌÐò´úÂë")==0) //?????
			{
				USART_SendString(USART2, "Ã»ÓÐ¿ÉÒÔ¸üÐÂµÄ¹Ì¼þ,PB¿ªÊ¼Ö´ÐÐ¾É³ÌÐò´úÂë");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"PBÕý³£¸üÐÂ²âÊÔÍê³É")==0) //?????
			{
				USART_SendString(USART2, "PBÕý³£¸üÐÂ²âÊÔÍê³É");
//				printf("Power Board v1.0");
			}
//		}

			
//			if	((USART2_RX_BUF[0]=='y')&&(USART2_RX_BUF[1]=='h'))
//{
//	printf("%s",USART2_RX_BUF);
//}//ATÖ¸Áî´¦Àí×Ö·û´®
    }  //???RX??BUFF????
//				memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
	
	
}
//³õÊ¼»¯IO ´®¿Ú3
//bound:²¨ÌØÂÊ

//´®¿Ú3,printf º¯Êý
//È·±£Ò»´Î·¢ËÍÊý¾Ý²»³¬¹ýUSART3_MAX_SEND_LEN×Ö½Ú
void u3_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART3_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART3_TX_BUF); //´Ë´Î·¢ËÍÊý¾ÝµÄ³¤¶È
	for (j = 0; j < i; j++)					 //Ñ­»··¢ËÍÊý¾Ý
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);											   //µÈ´ýÉÏ´Î´«ÊäÍê³É
		USART_SendData(USART3, (uint8_t)USART3_TX_BUF[j]); //·¢ËÍÊý¾Ýµ½´®¿Ú3
	}
}

/*

stm32´®¿Ú·¢ËÍ×Ö·ûº¯Êý

*/
void SendChar(uint8_t Data)

{

USART_SendData (USART3 ,Data);

while(USART_GetFlagStatus (USART3 ,USART_FLAG_TC )!=SET );

}


/*
stm32´®¿Ú·¢ËÍ×Ö·û´®º¯Êý
*/

void SendString(char *str)
{
 while(*str)
{
  SendChar(*str);
  str++;
}
}

void Send_Data(char *fmt, ...) 
{
		int i;
    va_list ap; //³õÊ¼»¯Ö¸Ïò¿É±ä²ÎÊýÁÐ±íµÄÖ¸Õë
    int len=0;          //¼ÆËãÊý¾Ý³¤¶È
    va_start(ap, fmt);         
    vsprintf(send_str, fmt, ap);
    len=strlen(send_str);
    char _data[len+4]; //¶¨ÒåÊý×éÀ´°´ÕÕÐ­Òé´æ·ÅÊý¾Ý£»£¨ËãÉÏÖ¡Í·Ö¡Î²µÄ³¤¶È+Êý¾ÝÐ£ÑéÎ»£©
    _data[0] = _head_1;
    _data[1] =_head_2;
    _data[2] =len;
    for (i = 0; i < len; i++)
    {
        _data[i+3]=send_str[i];

    }
    _data[len+3]='\0';
    SendString(_data);  //ÕâÀïµ÷ÓÃsendstring£»
    va_end(ap);                
}
#endif

void USART_SendString(USART_TypeDef* USARTx, char *DataString)
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//??????????(?????????????)
	while(DataString[i] != '\0')												//??????
	{
		USART_SendData(USARTx,DataString[i]);									//????????????
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//????????
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//??????????
		i++;
	}
}
//????????


void Send_data1(USART_TypeDef * USARTx,u8 *s)
{
 while(*s!=' ')
 { 
  while(USART_GetFlagStatus(USARTx,USART_FLAG_TC )==RESET); 
  USART_SendData(USARTx,*s);
  s++;
 }
}
