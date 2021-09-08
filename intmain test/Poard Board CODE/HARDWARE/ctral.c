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

//����PA1�ŵ�VDD_USB�ĸߵ�ƽ�źţ�GPS_12V|GPS_5V�ļ̵������ƽ�����ߵ�ƽ��
//����GPS_12V|GPS_5V�ļ̵������ƽ�����͵�ƽ���ﵽ��⵽USB����źź�CPS
//5V��12V�ĵ�ѹ��ͨ�ϡ�
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

//����������18V_INȻ��CP_12V�����ͨ�ϣ�˼·:18V_IN��ԭ��ͼ��ͨ��DC1��ת����
//�õ�5V0��5V0ͨ��LM1117ת3V3�����һ��3V3��оƬ�����VDD��ѹ����3V3û��ʱ��
//STM32������CP_12V�޷�����ߵ�ƽ����̵���ͨ�ϣ���˸��������CP_12V���ߵ�
//ƽ������ɣ���18V_INû��ʱ��CP_12V�ϣ���18V_IN��ʱ��CP_12Vͨ������ʵ��
void ctral_cpctrl()
{
	GPIO_SetBits(GPIOC,CP_12V);
}

//���Ƶ�����������⵽VBUS����������ߵ�ƽ�� EP_DOWN��EP_UP ���ţ������µ��������߽�ʱ
//�������ع���
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


	


