#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "receive_agreement.h"
#include "ep.h"
#include "bsp_ws281x.h"
//#include "tim_delay.h"
#include "clock.h"
#include "string.h"
#include "stdio.h"
#include "usart_dma.h"
//extern u8  USART2_RX_BUF[USART_REC_LEN1];
char data[20];//接收数组

u8 flag_bootload;


u8 flag;
u16 temp;
u16 temp1;


u8 time_delay_ms;

//u8 USB_HUB_2;
//u8 USB_HUB_2_1;
u8 USB_HUB_2_2;

//// u8 send_buf[] = {"IMEDIS 6000 TEST!!"}; 
 int y;

int main(void)
{
	SCB->VTOR = FLASH_BASE | 0x3000;//先中断向量偏移再打开所有中断，然后中断分组
	__enable_irq ();
//	SCB->VTOR = FLASH_BASE | 0x3000;
	NVIC_Configuration();
	clock_sys_init();
//	tim_Init();
//  NVIC_Configuration();
    delay_init();
	usart2_init(115200);
	usart3_init(115200);
	ep_init();
	ws281x_init();
//	rgbled_void();//电池初始化灭灯

	RGB_flag_color();
	USB_HUB_init();
//	usart2_dma_rx_init();
	usart2_dma_tx_init();
	usart3_dma_tx_init();
////	DMA_USART2_Tx_Data(send_buf,sizeof(send_buf));
	
	USART_SendString(USART2, "IMEDIS 6000 TEST!!1");
  while (1)
  {
		if(flag_bootload == 6)
			{

				iap_load_bootload(0x08000000);
			}
//		DMA_USART2_Tx_Data(send_buf,sizeof(send_buf - DMA1_Channel7->CNDTR));
//		if	((USART2_RX_BUF[0]==0x28)&&(USART2_RX_BUF[1]==0x0c))
//			{
//				USART_SendString(USART2, "1111111111111");
//			}
		
//		ws281x_setPixelColor(2,0x0F0000);//点亮单个任意色彩的灯，加show（）函数
//		ws281x_rainbowCycle(30);//彩虹测试灯
//		ws281x_show();
//		ws281x_theaterChase(ws281x_color(0x01, 0x05, 0x06),50);//交替闪
		ws281x_colorWipe(ws281x_color(0x00, 0x00, 0x0F),1);//追逐灯（红）(RGB灯)
//				ws281x_colorWipe(ws281x_color(0x00, 0x00, 0x0F),10);//追逐灯（蓝）
//		ws281x_theaterChaseRainbow(50);//追逐的彩虹灯，颜色交替
//						 tim_delay_ms(100);
//		if(time_delay_ms==100)
//		{
//			time_delay_ms=0;
//			printf("YHGOOD");
//		}
		
//		USB_HUB = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);
//			USB_HUB_1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
//			USB_HUB_2_2= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);
//			GPS_rgbled();
			RGB_flag_color();

		
		

			
			flag=0;
			temp = GPIOC->IDR ;
				delay_ms(10);
//					if(time_delay_ms==10)
//				{
//					time_delay_ms = 0;
				temp1 = GPIOC->IDR ;
//				}
			if(temp==temp1)
			{
				flag=1;
			}
			else if(temp!=temp1)
			{
				flag=2;
			}
			
			if(flag==1)
			{
				flag=0;
			}
			else if(flag==2)
			{
				Send_Data("%x", temp1);
			}
			
			
			

//		 RGB_flag_color();
			//电池信号指示灯
		
  }
}
