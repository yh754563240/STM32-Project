#include "clock.h"
#include "stdio.h"

void clock_sys_init(void)
{
	ErrorStatus HSE_Status;//ö��һ���ⲿ���پ���8mhz������״̬
	RCC_DeInit();//��λRCC�Ĵ���Ϊ��ʼ״̬
	RCC_HSEConfig(RCC_HSE_ON);//���ⲿ�ĸ��پ���8mhz��
	
	HSE_Status = RCC_WaitForHSEStartUp();//�ȴ��ⲿ�ĸ��پ���8mhz����
	
	if(HSE_Status == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//Ԥȡָ����ʹ��
		FLASH_SetLatency(FLASH_Latency_2);//������ʱֵ����ʱ����Ϊ2
		
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//���ⲿ����ʱ�ӵ�8mhz*9��ΪPLL��ʱ��Ƶ��
		RCC_PLLCmd(ENABLE);//ʹ��PLLʱ��
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)//�ȴ�PLL����
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//PLL��Ϊϵͳʱ��
		
		while(RCC_GetSYSCLKSource() != 0x08)//0x00:HSI��Ϊϵͳʱ�ӡ�0x04:HSE��Ϊϵͳʱ�ӡ�0x08:PLL��Ϊϵͳʱ��
		{
		}
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHB��ʱ�Ӿ���ϵͳʱ��72Mhz
		
		RCC_PCLK1Config(RCC_HCLK_Div2);//APB1��ʱ��Ϊ36Mhz
		
		RCC_PCLK2Config(RCC_HCLK_Div1);//APB2��ʱ��Ϊ72Mhz
		
		RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);//USB��ʱ��Ϊϵͳʱ��PLL 72Mhz/1.5=48Mhz
		
		RCC_ADCCLKConfig(RCC_PCLK2_Div2);//ADC��ʱ�ӵ���AP2 36Mhz/2=18Mhz
		
		
	}
	
	
}


