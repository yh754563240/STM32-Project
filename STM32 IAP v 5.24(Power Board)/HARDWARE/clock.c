#include "clock.h"
#include "stdio.h"

void clock_sys_init(void)
{
	ErrorStatus HSE_Status;//枚举一个外部高速晶振8mhz的起震状态
	RCC_DeInit();//复位RCC寄存器为初始状态
	RCC_HSEConfig(RCC_HSE_ON);//打开外部的高速晶振，8mhz的
	
	HSE_Status = RCC_WaitForHSEStartUp();//等待外部的高速晶振8mhz起震
	
	if(HSE_Status == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//预取指缓存使能
		FLASH_SetLatency(FLASH_Latency_2);//代码延时值，延时周期为2
		
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//用外部快速时钟的8mhz*9作为PLL的时钟频率
		RCC_PLLCmd(ENABLE);//使能PLL时钟
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)//等待PLL就绪
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//PLL作为系统时钟
		
		while(RCC_GetSYSCLKSource() != 0x08)//0x00:HSI作为系统时钟。0x04:HSE作为系统时钟。0x08:PLL作为系统时钟
		{
		}
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHB的时钟就是系统时钟72Mhz
		
		RCC_PCLK1Config(RCC_HCLK_Div2);//APB1的时钟为36Mhz
		
		RCC_PCLK2Config(RCC_HCLK_Div1);//APB2的时钟为72Mhz
		
		RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);//USB的时钟为系统时钟PLL 72Mhz/1.5=48Mhz
		
		RCC_ADCCLKConfig(RCC_PCLK2_Div2);//ADC的时钟等于AP2 36Mhz/2=18Mhz
		
		
	}
	
	
	
	
}


