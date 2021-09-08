#include "ctral.h"
#include "stdio.h"




	void ctral_init()
	{
			GPIO_InitTypeDef GPIO_InitStructure;	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			GPIO_InitStructure.GPIO_Pin = CTRAL_ALL;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
				
	}
	
	void ctral_cpctrl()
	{

			GPIO_SetBits(GPIOC,CP_12V);

	}
	//CPµÁ‘¥Õ®∂œ

		 