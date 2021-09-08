#include "ctral.h"

void ctral_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = CTRAL_ALL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = VDD_USB;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
				
}

//读到PA1脚的VDD_USB的高电平信号，GPS_12V|GPS_5V的继电器控制脚输出高电平，
//否则GPS_12V|GPS_5V的继电器控制脚输出低电平，达到检测到USB插拔信号后CPS
//5V与12V的电压的通断。
void ctral_gpsctrl()	
{
	if(GPIO_ReadInputDataBit(GPIOA,VDD_USB)==1)
	{
		GPIO_SetBits(GPIOC,GPS_12V|GPS_5V);
	}
	else 
	{
		GPIO_ResetBits(GPIOC,GPS_12V|GPS_5V);
	}
}

//条件是有无18V_IN然后CP_12V给电的通断，思路:18V_IN在原理图中通过DC1的转换，
//得到5V0，5V0通过LM1117转3V3，输出一个3V3给芯片供电得VDD电压，当3V3没有时
//STM32的引脚CP_12V无法输出高电平给予继电器通断，因此给予该引脚CP_12V常高电
//平输出即可，当18V_IN没有时，CP_12V断，当18V_IN有时，CP_12V通，功能实现
void ctral_cpctrl()
{
	GPIO_SetBits(GPIOC,CP_12V);
}

//控制电推升降，检测到VBUS，持续输出高电平在 EP_DOWN和EP_UP 引脚，当按下电推升或者降时
//升降开关工作
void ctral_epctrl()
{
	if(GPIO_ReadInputDataBit(GPIOA,VDD_USB)==1)
	{
		GPIO_SetBits(GPIOC,EP_DOWN|EP_UP);
	}
	else
	{
		{
			GPIO_ResetBits(GPIOC,EP_DOWN|EP_UP);
		}
	}
}


	


