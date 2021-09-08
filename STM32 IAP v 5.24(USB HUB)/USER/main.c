#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "uart.h"
#include "tim.h"
#include "sys.h"
#include "delay.h"
#include "iap.h"
#include "stmflash.h"
#include "tim.h"
#include "clock.h"
#include "stdio.h"

	u16 oldcount=0;	//�ϵĴ��ڽ�������ֵ
	u16 applenth=0;	//���յ���app���볤��
  u8 time;
	u32 temp = 0;

int main(void)
{
	__enable_irq ();
	NVIC_Configuration();
	clock_sys_init();
//	tim_Init();
	uart_init();
//	tim_Init();
	delay_init();
	
	printf("USB HUB�������ģʽ�ɹ�����4�������³����Զ�ִ�оɳ���");
	
	tim_Init();
  while (1)
  {
		delay_ms(100);

   if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		
		
		
		
		if(applenth>=1000)
			{
				printf("�̼�������...\r\n");
				temp = (*(vu32*)(0X20001000+4));
//				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX
 				if(((*(vu32*)(0X20001000+4))&0xFFFFF000)==0x08003000)//�ж��Ƿ�Ϊ0X08XXXXXX.//6.24�޸Ĺ�
				{	 
					
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   
					delay_ms(50);
					printf("USB HUB�̼��������!");
					delay_ms(50);
					printf("��ʼִ��USB HUB���¹̼�����!!");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
//				INTX_DISABLE();
//				__disable_irq();
					iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}
		}
			}
		
			
			if(time>=4)
			{
//			TIM_Cmd(TIM4, DISABLE);  //
//				TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
			time=0;
			printf("û�п��Ը��µĹ̼�,USB HUB��ʼִ�оɳ������");
				delay_ms(50);
								if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
//				INTX_DISABLE();
//				__disable_irq();
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}
			}
//		
  }
	
	
}

