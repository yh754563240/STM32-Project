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
#include "ctral.h"
#include "stdio.h"

	u16 oldcount=0;	//老的串口接收数据值
	u16 applenth=0;	//接收到的app代码长度
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
	ctral_init();
	
	printf("PW进入更新模式成功！！5秒内无新程序自动执行旧程序");
	
	tim_Init();
  while (1)
  {
		ctral_cpctrl();
		delay_ms(100);

   if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("PW 用户代码接收完成!");
				printf("代码长度:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		
		
		
		
		if(applenth>=1000)
			{
				printf("固件更新中...\r\n");
				temp = (*(vu32*)(0X20001000+4));
//				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX
 				if(((*(vu32*)(0X20001000+4))&0xFFFFF000)==0x08003000)//判断是否为0X08XXXXXX.//6.24修改过
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
					delay_ms(50);
					printf("PB固件更新完成!!");
					
						delay_ms(50);
					printf("开始执行PB更新固件代码!!");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
//				INTX_DISABLE();
//				__disable_irq();
					iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}
		}
			}
		
			
			if(time>=5)
			{
//			TIM_Cmd(TIM4, DISABLE);  //
//				TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
			time=0;
			printf("没有可以更新的固件,PB开始执行旧程序代码");
				delay_ms(50);
								if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
//				INTX_DISABLE();
//				__disable_irq();
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}
			}
//		
  }
	
	
}

