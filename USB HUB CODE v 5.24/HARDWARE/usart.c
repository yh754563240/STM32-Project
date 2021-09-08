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
#define _head_1 0x23        //֡ͷ'#'
#define _head_2 0x40        //֡ͷ'@'

char send_str[256];
int RX_Value = 0;
extern char data[20];
char data1[20];


u8 USART2_RX_BUF[USART_REC_LEN1];//���ջ���,���USART_REC_LEN���ֽ�.
u16 USART2_RX_STA = 0; //����״̬���

u8 USART3_RX_BUF[USART_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.
u16 USART3_RX_STA = 0; //����״̬��

//////////////////////////////////////////////////////////////////////////////////
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos ʹ��
#endif


//////////////////////////////////////////////////////////////////////////////////
//���ڷ��ͻ�����
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; //���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
#ifdef USART3_RX_EN
#endif								   //���ʹ���˽���
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; //���ջ���,���USART3_MAX_RECV_LEN���ֽ�.

#define channel_number 3
unsigned char DataScope_OutPut_Buffer[42] = {0}; //���ڷ��ͻ�����
u8 pstr[channel_number * 4 + 1];				 //
int state;
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
void _ttywrch(int x)
{
	x = x;
}

//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while ((USART2->SR & 0X40) == 0)
		; //ѭ������,ֱ���������
	USART2->DR = (u8)ch;
	return ch;
}
#endif

#if EN_USART1_RX //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA = 0; //����״̬���

//��ʼ��IO ����1
//bound:������
void uart_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USART1ʱ��


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
        
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);										//��ʼ������1

	USART_Cmd(USART1, ENABLE); //ʹ�ܴ���1

	//USART_ClearFlag(USART1, USART_FLAG_TC);

#if EN_USART1_RX
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif

	//USART_ClearFlag(USART1, USART_FLAG_TC);
}
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������

void usart2_init(u32 bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ��USART2ʱ��

	//USART1_TX   PA.2 PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;										//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// ��ģʽ
	USART_Init(USART2, &USART_InitStructure);										//��ʼ������
	USART_Cmd(USART2, ENABLE);														//ʹ�ܴ���
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//��������ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
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

	USART_DeInit(USART3); //��λ����3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //��ʼ��GPIOB11����GPIOB10

	USART_InitStructure.USART_BaudRate = bound;										//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������3

	USART_Cmd(USART3, ENABLE); //ʹ�ܴ���

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //�����ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

void USART1_IRQHandler(void) //����1�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS //���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();
#endif
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART1); //(USART1->DR);	//��ȡ���յ�������

		if ((USART_RX_STA & 0x8000) == 0) //����δ���
		{
			if (USART_RX_STA & 0x4000) //���յ���0x0d
			{
				if (Res != 0x0a)
					USART_RX_STA = 0; //���մ���,���¿�ʼ
				else
					USART_RX_STA |= 0x8000; //���������
			}
			else //��û�յ�0X0D
			{
				if (Res == 0x0d)
					USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
						USART_RX_STA = 0; //�������ݴ���,���¿�ʼ����
				}
			}
		}
	}
#if SYSTEM_SUPPORT_OS //���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
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
//////////DMA�Ľ����Լ�����������ƹ�ҽṹ


void USART2_IRQHandler(void) //����1�жϷ������
{
u8 clear=clear;
u8 Res;
u16 len;	
clear = 0;
	
u16 USART2_RX_STA_FLAG;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART2); //(USART2->DR);	//��ȡ���յ�������
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
		
		
		
		
//////////		if ((USART2_RX_STA & 0x8000) == 0) //����δ���
//////////		{
//////////			
//////////				
//////////				
//////////			if (USART2_RX_STA & 0x4000) //���յ���0x0d
//////////			{
//////////				if (Res != 0x0a)
//////////					USART2_RX_STA = 0; //���մ���,���¿�ʼ
//////////				else
//////////					USART2_RX_STA |= 0x8000; //���������
//////////			}
//////////			
//////////			
//////////			
//////////			else //��û�յ�0X0D
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
//////////						USART2_RX_STA = 0; //�������ݴ���,���¿�ʼ����
//////////				}
//////////			}
//////////			
//////////		}//ԭʼ6.25




//////////		if( USART2_RX_STA<0x001e || (USART2_RX_STA & 0x4000) == 0x4000)
//////////		{
//////////				if ((USART2_RX_STA & 0x8000) == 0) //����δ���
//////////		{
//////////			
//////////		

//////////				
//////////			if (USART2_RX_STA & 0x4000) //���յ���0x0d
//////////			{
//////////				if (Res != 0x0a)
//////////					USART2_RX_STA = 0; //���մ���,���¿�ʼ
//////////				else
//////////					USART2_RX_STA |= 0x8000; //���������
//////////			}
//////////		
//////////			
//////////			
//////////			else //��û�յ�0X0D
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
//////////						USART2_RX_STA = 0; //�������ݴ���,���¿�ʼ����
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
//////////						USART2_RX_STA = 0; //�������ݴ���,���¿�ʼ����
//////////				}
//////////			


//////////				
//////////				
//////////		}


		if ((USART2_RX_STA & 0x8000) == 0) //����δ���
		{
			
			if (USART2_RX_STA & 0x4000) //���˱�־λ0x4000��֤�����յ���0x0d��Ȼ���ж�������0x0a���ǵĻ��ͽ�����ȷ���������
			{
				if(USART2_RX_STA < 0x4014) //�ж��Ƿ����˱�־λ0x4000������ݻ��Ƿ�С��20(0x0014)��С�ڵĻ�Ĭ��Ϊ��һ��ATָ���ַ�
				{
				if (Res != 0x0a)
					USART2_RX_STA = 0; //���մ���,���¿�ʼ
				else
					USART2_RX_STA |= 0x8000; //���������
			}
				
			}

			
			
			else //��û�յ�0X0D
			{
			
				if (USART2_RX_STA < 0x0014 && Res == 0x0d)  //���ݳ���С��20(0x0014)����Res����0x0dʱ�������0x0d������λ��ͨ�����ڵ������ַ��͹���������ĩβ���Ǹ��� 0x0d���س����� 0x0a�����з�)
				                                            //����0x0d���س����� 0x0a�����з�)������������Ȼָ���Ǵ���λ��������������
				{
					USART2_RX_STA |= 0x4000; //������������ʱ��һ����־λ
				}
//				else if ((USART2_RX_STA > 0x0014 ) && (USART2_RX_STA & 0xF000 == 0x3000))
				else if (USART2_RX_STA > 0x0014 && (USART2_RX_STA == 0x28DA))
//	else if (USART2_RX_STA > 0x0014 && USART2_RX_STA == 0x3887)				//���ݳ��ȴ���20(0x0014)����USART2_RX_STA == 0x38bf��0x38bfΪPower Board���������Եı�־λ�жϣ�0x38bfΪ��BIN�ļ��Ķ����Ƴ��ȣ�

				{
					USART2_RX_STA |= 0xC000; //������������ʱ��һ����־λ
				}
	
				else
				{
					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
					USART2_RX_STA++;
					if (USART2_RX_STA > (USART_REC_LEN1 - 1))
						USART2_RX_STA = 0; //�������ݴ���,���¿�ʼ����
				}
			}
			
		}//ԭʼ6.29
	








	}
	
	
	
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
    {
			USART_ClearITPendingBit(USART2, USART_IT_IDLE);
			

			
if((USART2_RX_STA_FLAG = USART2_RX_STA & 0xC000)) //�������
	{
		if(USART2_RX_STA_FLAG == 0xC000 )//������ɱ�־
		{
			len=USART2_RX_STA & 0x3fff;//
			USART2_RX_STA=0;	//USART2_RX_STA��־λ��0	
			USART2_RX_BUF[len]=0;//����len��λ����0
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
				USART_SendString(USART2,"USB HUB�������ģʽ����");
				SoftReset();
			}
			if(strcmp(USART2_RX_BUF,"imedis+HUB+UPDATA1")==0) //?????
			{
				extern u8 flag_bootload;
				USART_SendString(USART2,"USB HUB JUMP�������ģʽ����");
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
//}//ATָ����ַ���
    }  //???RX??BUFF????
	
	
	
	
}


//void USART2_IRQHandler(void)
//{
//	u8 res;
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
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
			; //ѭ������,ֱ���������
		USART1->DR = (u8)str[i];
	}
	while ((USART1->SR & 0X40) == 0)
		; //ѭ������,ֱ���������
	USART1->DR = (u8)str[size * cnt + 4];
}

//ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
//���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
void USART3_IRQHandler(void)
{
	u8 clear=clear;
u8 res;
u8 len;	
clear = 0;
	

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{
		res = USART_ReceiveData(USART3);
		Receive_Data(res ,data);

//   USART_SendData(USART2 , res+2);
//		Receive_Data(res ,data);
		if ((USART3_RX_STA & 0x8000) == 0) //����δ���
		{
			if (USART3_RX_STA & 0x4000) //���յ���0x0d
			{
				if (res != 0x0a)
					USART3_RX_STA = 0; //���մ���,���¿�ʼ
				else
					USART3_RX_STA |= 0x8000; //���������
			}
			else //��û�յ�0X0D
			{
				if (res == 0x0d)
					USART3_RX_STA |= 0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = res;
					USART3_RX_STA++;
					if (USART3_RX_STA > (USART_REC_LEN - 1))
						USART3_RX_STA = 0; //�������ݴ���,���¿�ʼ����
				}
			}
		}

//				Receive_Data(res ,data);	
	}
	

		
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) 
    {
			USART_ClearITPendingBit(USART3, USART_IT_IDLE);
			

			
if((USART3_RX_STA |= 0x8000)) //�������
	{
			len=USART3_RX_STA & 0x3fff;//���յ��ĳ���
			USART3_RX_STA=0;	//���ձ������			
			USART3_RX_BUF[len] = 0;//���ջ�������������
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
			if(strcmp(USART3_RX_BUF,"PW�������ģʽ�ɹ�����5�������³����Զ�ִ�оɳ���")==0) //?????
			{
				USART_SendString(USART2, "PW�������ģʽ�ɹ�����5�������³����Զ�ִ�оɳ���");
//				printf("Power Board v1.0");
			}
				if(strcmp(USART3_RX_BUF,"PB�̼��������!")==0) //?????
			{
				USART_SendString(USART2, "PB�̼�������ɣ�");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"��ʼִ��PB���¹̼�����!!")==0) //?????
			{
				USART_SendString(USART2, "��ʼִ��PB���¹̼�����!!");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"û�п��Ը��µĹ̼�,PB��ʼִ�оɳ������")==0) //?????
			{
				USART_SendString(USART2, "û�п��Ը��µĹ̼�,PB��ʼִ�оɳ������");
//				printf("Power Board v1.0");
			}
			if(strcmp(USART3_RX_BUF,"PB�������²������")==0) //?????
			{
				USART_SendString(USART2, "PB�������²������");
//				printf("Power Board v1.0");
			}
//		}

			
//			if	((USART2_RX_BUF[0]=='y')&&(USART2_RX_BUF[1]=='h'))
//{
//	printf("%s",USART2_RX_BUF);
//}//ATָ����ַ���
    }  //???RX??BUFF????
//				memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
	
	
}
//��ʼ��IO ����3
//bound:������

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART3_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART3_TX_BUF); //�˴η������ݵĳ���
	for (j = 0; j < i; j++)					 //ѭ����������
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);											   //�ȴ��ϴδ������
		USART_SendData(USART3, (uint8_t)USART3_TX_BUF[j]); //�������ݵ�����3
	}
}

/*

stm32���ڷ����ַ�����

*/
void SendChar(uint8_t Data)

{

USART_SendData (USART3 ,Data);

while(USART_GetFlagStatus (USART3 ,USART_FLAG_TC )!=SET );

}


/*
stm32���ڷ����ַ�������
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
    va_list ap; //��ʼ��ָ��ɱ�����б���ָ��
    int len=0;          //�������ݳ���
    va_start(ap, fmt);         
    vsprintf(send_str, fmt, ap);
    len=strlen(send_str);
    char _data[len+4]; //��������������Э�������ݣ�������֡ͷ֡β�ĳ���+����У��λ��
    _data[0] = _head_1;
    _data[1] =_head_2;
    _data[2] =len;
    for (i = 0; i < len; i++)
    {
        _data[i+3]=send_str[i];

    }
    _data[len+3]='\0';
    SendString(_data);  //�������sendstring��
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