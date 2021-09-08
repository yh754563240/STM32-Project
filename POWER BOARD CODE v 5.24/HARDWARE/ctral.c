#include "ctral.h"
#include "stdio.h"

iapfun jump2app;

	void ctral_init()
	{
			GPIO_InitTypeDef GPIO_InitStructure;	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			GPIO_InitStructure.GPIO_Pin = CTRAL_ALL;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
		
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_Init(GPIOA, &GPIO_InitStructure);		
	}
	
	
void ctral_gpsctrl()	
	{
		extern float ADC_ConvertedValueLocal;

		if(ADC_ConvertedValueLocal>=2.8)
		{
			GPIO_SetBits(GPIOC,GPS_12V|GPS_5V);
		}
			else if(ADC_ConvertedValueLocal<=3.0)
			{
				GPIO_ResetBits(GPIOC,GPS_12V|GPS_5V);
			}
		
	}
	//GPS电源通断
	
	void ctral_cpctrl()
	{

			GPIO_SetBits(GPIOC,CP_12V);

	}
	//CP电源通断

	
void iap_load_bootload(u32 appxaddr)
{
__disable_irq(); //关闭所有中断
	
	TIM_DeInit(TIM4);
	TIM_DeInit(TIM3);
	USART_DeInit(USART2);
	ADC_DeInit(ADC1);
	
	if(((*(vu32*)appxaddr)&0x2FFB0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);			//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到BootLoad.
	}
}		 