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
#define _head_1 0x23        //帧头'#'
#define _head_2 0x40        //帧头'@'

char send_str[256];
int RX_Value = 0;
extern char data[20];
char data1[20];


u8 USART2_RX_BUF[USART_REC_LEN1];//接收缓冲,最大USART_REC_LEN个字节.
u16 USART2_RX_STA = 0; //接收状态标记

u8 USART3_RX_BUF[USART_REC_LEN];//接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA = 0; //接收状态标?

//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos 使用
#endif


//////////////////////////////////////////////////////////////////////////////////
//串口发送缓存区
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; //发送缓冲,最大USART3_MAX_SEND_LEN字节
#ifdef USART3_RX_EN
#endif								   //如果使能了接收
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; //接收缓冲,最大USART3_MAX_RECV_LEN个字节.

#define channel_number 3
unsigned char DataScope_OutPut_Buffer[42] = {0}; //串口发送缓冲区
u8 pstr[channel_number * 4 + 1];				 //
int state;
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
void _ttywrch(int x)
{
	x = x;
}

//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while ((USART2->SR & 0X40) == 0)
		; //循环发送,直到发送完毕
	USART2->DR = (u8)ch;
	return ch;
}
#endif

#if EN_USART1_RX //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0; //接收状态标记

//初始化IO 串口1
//bound:波特率
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1时钟


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
        
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART1, &USART_InitStructure);										//初始化串口1

	USART_Cmd(USART1, ENABLE); //使能串口1

	//USART_ClearFlag(USART1, USART_FLAG_TC);

#if EN_USART1_RX
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器、

#endif

	//USART_ClearFlag(USART1, USART_FLAG_TC);
}
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率

void usart2_init(u32 bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟

	//USART1_TX   PA.2 PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;										//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 发模式
	USART_Init(USART2, &USART_InitStructure);										//初始化串口
	USART_Cmd(USART2, ENABLE);														//使能串口
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//开启相关中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器、
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

	USART_DeInit(USART3); //复位串口3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //初始化GPIOB11，和GPIOB10

	USART_InitStructure.USART_BaudRate = bound;										//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口3

	USART_Cmd(USART3, ENABLE); //使能串口

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器
}

void USART1_IRQHandler(void) //串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS //如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();
#endif
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1); //(USART1->DR);	//读取接收到的数据

		if ((USART_RX_STA & 0x8000) == 0) //接收未完成
		{
			if (USART_RX_STA & 0x4000) //接收到了0x0d
			{
				if (Res != 0x0a)
					USART_RX_STA = 0; //接收错误,重新开始
				else
					USART_RX_STA |= 0x8000; //接收完成了
			}
			else //还没收到0X0D
			{
				if (Res == 0x0d)
					USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
						USART_RX_STA = 0; //接收数据错误,重新开始接收
				}
			}
		}
	}
#if SYSTEM_SUPPORT_OS //如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
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


void USART2_IRQHandler(void) //串口1中断服务程序
{
u8 clear=clear;
u8 Res;
u16 len;	
clear = 0;
	
u16 USART2_RX_STA_FLAG;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART2); //(USART2->DR);	//读取接收到的数据
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
		
		
		
		
//////////		if ((USART2_RX_STA & 0x8000) == 0) //接收未完成
//////////		{
//////////			
//////////				
//////////				
//////////			if (USART2_RX_STA & 0x4000) //接收到了0x0d
//////////			{
//////////				if (Res != 0x0a)
//////////					USART2_RX_STA = 0; //接收错误,重新开始
//////////				else
//////////					USART2_RX_STA |= 0x8000; //接收完成了
//////////			}
//////////			
//////////			
//////////			
//////////			else //还没收到0X0D
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
//////////						USART2_RX_STA = 0; //接收数据错误,重新开始接收
//////////				}
//////////			}
//////////			
//////////		}//原始6.25




//////////		if( USART2_RX_STA<0x001e || (USART2_RX_STA & 0x4000) == 0x4000)
//////////		{
//////////				if ((USART2_RX_STA & 0x8000) == 0) //接收未完成
//////////		{
//////////			
//////////		

//////////				
//////////			if (USART2_RX_STA & 0x4000) //接收到了0x0d
//////////			{
//////////				if (Res != 0x0a)
//////////					USART2_RX_STA = 0; //接收错误,重新开始
//////////				else
//////////					USART2_RX_STA |= 0x8000; //接收完成了
//////////			}
//////////		
//////////			
//////////			
//////////			else //还没收到0X0D
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
//////////						USART2_RX_STA = 0; //接收数据错误,重新开始接收
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
//////////						USART2_RX_STA = 0; //接收数据错误,重新开始接收
//////////				}
//////////			


//////////				
//////////				
//////////		}


		if ((USART2_RX_STA & 0x8000) == 0) //接收未完成
		{
			
			if (USART2_RX_STA & 0x4000) //置了标志位0x4000，证明接收到了0x0d，然后判断连续的0x0a，是的话就接收正确，否则错误
			{
				if(USART2_RX_STA < 0x4014) //判断是否置了标志位0x4000后的数据还是否小于20(0x0014)，小于的话默认为是一个AT指令字符
				{
				if (Res != 0x0a)
					USART2_RX_STA = 0; //接收错误,重新开始
				else
					USART2_RX_STA |= 0x8000; //接收完成了
			}
				
			}

			
			
			else //还没收到0X0D
			{
			
				if (USART2_RX_STA < 0x0014 && Res == 0x0d)  //数据长度小于20(0x0014)而且Res等于0x0d时，这里的0x0d，是上位机通过串口调试助手发送过来的数据末尾总是跟着 0x0d（回车符） 0x0a（换行符)
				                                            //并且0x0d（回车符） 0x0a（换行符)总是连续，当然指的是从上位机发送来的数据
				{
					USART2_RX_STA |= 0x4000; //上述条件成立时置一个标志位
				}
//				else if ((USART2_RX_STA > 0x0014 ) && (USART2_RX_STA & 0xF000 == 0x3000))
				else if (USART2_RX_STA > 0x0014 && (USART2_RX_STA == 0x28DA))
//	else if (USART2_RX_STA > 0x0014 && USART2_RX_STA == 0x3887)				//数据长度大于20(0x0014)而且USART2_RX_STA == 0x38bf，0x38bf为Power Board传输完整性的标志位判断（0x38bf为该BIN文件的二进制长度）

				{
					USART2_RX_STA |= 0xC000; //上述条件成立时置一个标志位
				}
	
				else
				{
					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
					USART2_RX_STA++;
					if (USART2_RX_STA > (USART_REC_LEN1 - 1))
						USART2_RX_STA = 0; //接收数据错误,重新开始接收
				}
			}
			
		}//原始6.29
	








	}
	
	
	
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
    {
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);
			

			
if((USART2_RX_STA_FLAG = USART2_RX_STA & 0xC000)) //接收完成
	{
		if(USART2_RX_STA_FLAG == 0xC000 )//接收完成标志
		{
			len=USART2_RX_STA & 0x3fff;//
			USART2_RX_STA=0;	//USART2_RX_STA标志位置0	
			USART2_RX_BUF[len]=0;//长度len的位次置0
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
				USART_SendString(USART2,"USB HUB进入更新模式！！");
				SoftReset();
			}
			if(strcmp(USART2_RX_BUF,"imedis+HUB+UPDATA1")==0) //?????
			{
				extern u8 flag_bootload;
				USART_SendString(USART2,"USB HUB JUMP进入更新模式！！");
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
//}//AT指令处理字符串
    }  //???RX??BUFF????
	
	
	
	
}


//void USART2_IRQHandler(void)
//{
//	u8 res;
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
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
			; //循环发送,直到发送完毕
		USART1->DR = (u8)str[i];
	}
	while ((USART1->SR & 0X40) == 0)
		; //循环发送,直到发送完毕
	USART1->DR = (u8)str[size * cnt + 4];
}

//通过判断接收连续2个字符之间的时间差不大于100ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过100ms,则认为不是1次连续数据.也就是超过100ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
void USART3_IRQHandler(void)
{
	u8 clear=clear;
u8 res;
u8 len;	
clear = 0;
	

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{
		res = USART_ReceiveData(USART3);
		Receive_Data(res ,data);

//   USART_SendData(USART2 , res+2);
//		Receive_Data(res ,data);
		if ((USART3_RX_STA & 0x8000) == 0) //接收未完成
		{
			if (USART3_RX_STA & 0x4000) //接收到了0x0d
			{
				if (res != 0x0a)
					USART3_RX_STA = 0; //接收错误,重新开始
				else
					USART3_RX_STA |= 0x8000; //接收完成了
			}
			else //还没收到0X0D
			{
				if (res == 0x0d)
					USART3_RX_STA |= 0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = res;
					USART3_RX_STA++;
					if (USART3_RX_STA > (USART_REC_LEN - 1))
						USART3_RX_STA = 0; //接收数据错误,重新开始接收
				}
			}
		}

//				Receive_Data(res ,data);	
	}
	

		
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) 
    {
			USART_ClearITPendingBit(USART3, USART_IT_IDLE);
			

			
if((USART3_RX_STA |= 0x8000)) //接收完成
	{
			len=USART3_RX_STA & 0x3fff;//接收到的长度
			USART3_RX_STA=0;	//接收标记清零			
			USART3_RX_BUF[len] = 0;//接收缓存区后面清零
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
			if(strcmp(USART3_RX_BUF,"PW进入更新模式成功！！5秒内无新程序自动执行旧程序")==0) //?????
			{
				USART_SendString(USART2, "PW进入更新模式成功！！5秒内无新程序自动执行旧程序");
//				printf("Power Board v1.0");
			}
				if(strcmp(USART3_RX_BUF,"PB固件更新完成!")==0) //?????
			{
				USART_SendString(USART2, "PB固件更新完成！");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"开始执行PB更新固件代码!!")==0) //?????
			{
				USART_SendString(USART2, "开始执行PB更新固件代码!!");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"没有可以更新的固件,PB开始执行旧程序代码")==0) //?????
			{
				USART_SendString(USART2, "没有可以更新的固件,PB开始执行旧程序代码");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"PB正常更新测试完成")==0) //?????
			{
				USART_SendString(USART2, "PB正常更新测试完成");
//				printf("Power Board v1.0");
			}
//		}

			
//			if	((USART2_RX_BUF[0]=='y')&&(USART2_RX_BUF[1]=='h'))
//{
//	printf("%s",USART2_RX_BUF);
//}//AT指令处理字符串
    }  //???RX??BUFF????
//				memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
	
	
}
//初始化IO 串口3
//bound:波特率

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART3_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART3_TX_BUF); //此次发送数据的长度
	for (j = 0; j < i; j++)					 //循环发送数据
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);											   //等待上次传输完成
		USART_SendData(USART3, (uint8_t)USART3_TX_BUF[j]); //发送数据到串口3
	}
}

/*

stm32串口发送字符函数

*/
void SendChar(uint8_t Data)

{

USART_SendData (USART3 ,Data);

while(USART_GetFlagStatus (USART3 ,USART_FLAG_TC )!=SET );

}


/*
stm32串口发送字符串函数
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
    va_list ap; //初始化指向可变参数列表的指针
    int len=0;          //计算数据长度
    va_start(ap, fmt);         
    vsprintf(send_str, fmt, ap);
    len=strlen(send_str);
    char _data[len+4]; //定义数组来按照协议存放数据；（算上帧头帧尾的长度+数据校验位）
    _data[0] = _head_1;
    _data[1] =_head_2;
    _data[2] =len;
    for (i = 0; i < len; i++)
    {
        _data[i+3]=send_str[i];

    }
    _data[len+3]='\0';
    SendString(_data);  //这里调用sendstring；
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
