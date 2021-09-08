#include "sys.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "stdarg.h"
#include "usart.h"
#include "receive_agreement.h"
#define USART3_MAX_RECV_LEN 200
#define USART3_MAX_SEND_LEN 200
#define _head_1 0x23        //Ö¡Í·'#'
#define _head_2 0x40        //Ö¡Í·'@'

char send_str[256];
int RX_Value = 0;
extern char data[20];//½ÓÊÕUSART2ÅĞ¶¨Í¨ĞÅĞ­ÒéµÄ×Ô¶¨ÒåÊı×é

u8 USART2_RX_BUF[USART_REC_LEN1];//½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
u16 USART2_RX_STA = 0; //½ÓÊÕ×´Ì¬±ê¼Ç

u8 USART3_RX_BUF[USART_REC_LEN];//½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
u16 USART3_RX_STA = 0; //½ÓÊÕ×´Ì¬±ê¼

//////////////////////////////////////////////////////////////////////////////////
//Èç¹ûÊ¹ÓÃucos,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼ş¼´¿É.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos Ê¹ÓÃ
#endif


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
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êı,¶ø²»ĞèÒªÑ¡Ôñuse MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//±ê×¼¿âĞèÒªµÄÖ§³Öº¯Êı
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

//ÖØ¶¨Òåfputcº¯Êı
int fputc(int ch, FILE *f)
{
	while ((USART2->SR & 0X40) == 0)
		; //Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï
	USART2->DR = (u8)ch;
	return ch;
}
#endif

#if EN_USART1_RX //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó
u8 USART_RX_BUF[USART_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
u8 USART2_RX_BUF[USART_REC_LEN1];
u8 USART3_RX_BUF[USART_REC_LEN];
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓĞĞ§×Ö½ÚÊıÄ¿
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
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;								//ÎŞÆæÅ¼Ğ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//ÊÕ·¢Ä£Ê½
	USART_Init(USART1, &USART_InitStructure);										//³õÊ¼»¯´®¿Ú1

	USART_Cmd(USART1, ENABLE); //Ê¹ÄÜ´®¿Ú1

	//USART_ClearFlag(USART1, USART_FLAG_TC);

#if EN_USART1_RX
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //¿ªÆôÏà¹ØÖĞ¶Ï

	//Usart1 NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //´®¿Ú1ÖĞ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);							  //¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷¡¢

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
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;								//ÎŞÆæÅ¼Ğ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// ·¢Ä£Ê½
	USART_Init(USART2, &USART_InitStructure);										//³õÊ¼»¯´®¿Ú
	USART_Cmd(USART2, ENABLE);														//Ê¹ÄÜ´®¿Ú
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//¿ªÆôÏà¹ØÖĞ¶Ï
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	//Usart1 NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  //´®¿Ú1ÖĞ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);							  //¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷¡¢
}

void usart3_init(u32 bound)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //Ê¹ÄÜGPIOBÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //Ê¹ÄÜUSART3Ê±ÖÓ

	USART_DeInit(USART3); //¸´Î»´®¿Ú3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //³õÊ¼»¯GPIOB11£¬ºÍGPIOB10

	USART_InitStructure.USART_BaudRate = bound;										//²¨ÌØÂÊ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;								//ÎŞÆæÅ¼Ğ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//ÊÕ·¢Ä£Ê½

	USART_Init(USART3, &USART_InitStructure); //³õÊ¼»¯´®¿Ú3

	USART_Cmd(USART3, ENABLE); //Ê¹ÄÜ´®¿Ú

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //¿ªÆôÖĞ¶Ï

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //ÇÀÕ¼ÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);							  //¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷
}

void USART1_IRQHandler(void) //´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
{
	u8 Res;
#if SYSTEM_SUPPORT_OS //Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòĞèÒªÖ§³ÖOS.
	OSIntEnter();
#endif
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //½ÓÊÕÖĞ¶Ï(½ÓÊÕµ½µÄÊı¾İ±ØĞëÊÇ0x0d 0x0a½áÎ²)
	{
		Res = USART_ReceiveData(USART1); //(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊı¾İ

		if ((USART_RX_STA & 0x8000) == 0) //½ÓÊÕÎ´Íê³É
		{
			if (USART_RX_STA & 0x4000) //½ÓÊÕµ½ÁË0x0d
			{
				if (Res != 0x0a)
					USART_RX_STA = 0; //½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
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
						USART_RX_STA = 0; //½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ
				}
			}
		}
		
	}
#if SYSTEM_SUPPORT_OS //Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòĞèÒªÖ§³ÖOS.
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

void USART2_IRQHandler(void) //´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
{
u8 clear=clear;
u8 Res;
u8 len;	
clear = 0;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //½ÓÊÕÖĞ¶Ï(½ÓÊÕµ½µÄÊı¾İ±ØĞëÊÇ0x0d 0x0a½áÎ²)
	{
		Res = USART_ReceiveData(USART2); //(USART2->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊı¾İ
		Receive_Data(Res ,data);
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
			if ((USART2_RX_STA & 0x8000) == 0) //½ÓÊÕÎ´Íê³É
		{
			if (USART2_RX_STA & 0x4000) //½ÓÊÕµ½ÁË0x0d
			{
				if (Res != 0x0a)
					USART2_RX_STA = 0; //½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
				else
					USART2_RX_STA |= 0x8000; //½ÓÊÕÍê³ÉÁË
			}
			else //»¹Ã»ÊÕµ½0X0D
			{
				if (Res == 0x0d)
					USART2_RX_STA |= 0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
					USART2_RX_STA++;
					if (USART2_RX_STA > (USART_REC_LEN1 - 1))
						USART2_RX_STA = 0; //½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ
				}
			}
		}
//		if((USART2_RX_BUF[0]=='#')&&(USART2_RX_BUF[1]=='@'))
//		{
//		Receive_Data(Res ,data);
//		}
		
		}
	
		
//			if((USART2_RX_BUF[0]=='#')&&(USART2_RX_BUF[1]=='@'))
//		{
//		Receive_Data(Res ,data);
//		}
		
		if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
    {
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);
			

			
if((USART2_RX_STA |= 0x8000)) //½ÓÊÕÍê³É
	{
			len=USART2_RX_STA & 0x3fff;//½ÓÊÕµ½µÄ³¤¶È
			USART2_RX_STA=0;	//½ÓÊÕ±ê¼ÇÇåÁã
			USART2_RX_BUF[len]=0;//½ÓÊÕ»º´æÇøºóÃæÇåÁã
	}
	
        clear = USART2->SR; // clear
        clear = USART2->DR; // clear
	
//			if	((USART2_RX_BUF[0]!='#')&&(USART2_RX_BUF[1]!='@'))
//			{

			if(strcmp(USART2_RX_BUF,"imedis+PB+V")==0) //?????
			{
//				USART_SendString(USART2, "Power Board v1.0.1");
				USART_SendString(USART2, "PBÕı³£¸üĞÂ²âÊÔÍê³É");
//				printf("Power Board v1.0");
			}
//		}
	if(strcmp(USART2_RX_BUF,"imedis+PB+UPDATA")==0) //?????
			{
//				USART_SendString(USART2, "PB½øÈëÁË¸üĞÂÄ£Ê½£¡£¡");
				extern u8 flag_SoftReset;
				flag_SoftReset = 5;
//				printf("Power Board v1.0");
			}
			if(strcmp(USART2_RX_BUF,"imedis+PB+UPDATA1")==0) //?????
			{
				extern u8 flag_bootload;
				flag_bootload = 6;
			}
			
//			if	((USART2_RX_BUF[0]=='y')&&(USART2_RX_BUF[1]=='h'))
//{
//	printf("%s",USART2_RX_BUF);
//}//ATÖ¸Áî´¦Àí×Ö·û´®
    }  //???RX??BUFF????
		
		
}
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

//Í¨¹ıÅĞ¶Ï½ÓÊÕÁ¬Ğø2¸ö×Ö·ûÖ®¼äµÄÊ±¼ä²î²»´óÓÚ100msÀ´¾ö¶¨ÊÇ²»ÊÇÒ»´ÎÁ¬ĞøµÄÊı¾İ.
//Èç¹û2¸ö×Ö·û½ÓÊÕ¼ä¸ô³¬¹ı100ms,ÔòÈÏÎª²»ÊÇ1´ÎÁ¬ĞøÊı¾İ.Ò²¾ÍÊÇ³¬¹ı100msÃ»ÓĞ½ÓÊÕµ½
//ÈÎºÎÊı¾İ,Ôò±íÊ¾´Ë´Î½ÓÊÕÍê±Ï.
//½ÓÊÕµ½µÄÊı¾İ×´Ì¬
//[15]:0,Ã»ÓĞ½ÓÊÕµ½Êı¾İ;1,½ÓÊÕµ½ÁËÒ»ÅúÊı¾İ.
//[14:0]:½ÓÊÕµ½µÄÊı¾İ³¤¶È

void USART3_IRQHandler(void)
{
	u8 res;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //½ÓÊÕµ½Êı¾İ
	{
		res = USART_ReceiveData(USART3);
   
  // USART_SendData(USART1, res);
		Receive_Data(res ,data);	
     	
	}
}
//³õÊ¼»¯IO ´®¿Ú3
//bound:²¨ÌØÂÊ

//´®¿Ú3,printf º¯Êı
//È·±£Ò»´Î·¢ËÍÊı¾İ²»³¬¹ıUSART3_MAX_SEND_LEN×Ö½Ú
void u3_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART3_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART3_TX_BUF); //´Ë´Î·¢ËÍÊı¾İµÄ³¤¶È
	for (j = 0; j < i; j++)					 //Ñ­»··¢ËÍÊı¾İ
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);											   //µÈ´ıÉÏ´Î´«ÊäÍê³É
		USART_SendData(USART3, (uint8_t)USART3_TX_BUF[j]); //·¢ËÍÊı¾İµ½´®¿Ú3
	}
}

/*

stm32´®¿Ú·¢ËÍ×Ö·ûº¯Êı

*/
void SendChar(uint8_t Data)

{

USART_SendData (USART2 ,Data);

while(USART_GetFlagStatus (USART2 ,USART_FLAG_TC )!=SET );

}


/*
stm32´®¿Ú·¢ËÍ×Ö·û´®º¯Êı
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
    va_list ap; //³õÊ¼»¯Ö¸Ïò¿É±ä²ÎÊıÁĞ±íµÄÖ¸Õë
    int len=0;          //¼ÆËãÊı¾İ³¤¶È
    va_start(ap, fmt);         
    vsprintf(send_str, fmt, ap);
    len=strlen(send_str);
    char _data[len+4]; //¶¨ÒåÊı×éÀ´°´ÕÕĞ­Òé´æ·ÅÊı¾İ£»£¨ËãÉÏÖ¡Í·Ö¡Î²µÄ³¤¶È+Êı¾İĞ£ÑéÎ»£©
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
