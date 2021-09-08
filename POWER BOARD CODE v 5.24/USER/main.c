#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "receive_agreement.h"
#include "batt.h"
#include "ctral.h"
#include "adc.h"
#include "pwm.h"
//#include "tim_delay.h"
#include "string.h"
#include "stdio.h"


u8 flag_bootload;
u8 flag_SoftReset;

char data[20];

float ADC_ConvertedValue; 
float ADC_ConvertedValueLocal;//ADC�ɼ���ѹ


u8 flag;
u16 temp;
u16 temp1;
u8 a;

u8 time_delay_ms;

int main(void)
{
	SCB->VTOR = FLASH_BASE | 0x3000;
	__enable_irq ();
	NVIC_Configuration();
  delay_init();
	usart2_init(115200);
  batt_init();
  batt_init_Read();//��ȡ��������źų�ʼ��
	ctral_init();
	Adc_Config();
	pwm_init();//PWM���������ʼ��
//	tim_Init();
	delay_ms(10);
	temp = GPIOB->IDR ;	
	
	Send_Data("%x", temp);
  while (1)
  {
//		Send_Data("%x", temp);
		
//WFI_SET();		//ִ��WFIָ��
		
		ctral_gpsctrl();//GPS��Դ
		ctral_cpctrl();//һ�����Դ
		
				if(ADC_ConvertedValueLocal>=2.8)
		{
			pwm_daty_updata(70);
		}
			else if(ADC_ConvertedValueLocal<=3.0)
			{
				pwm_daty_updata(20);
			}//����2������
		
			
			
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//����ת��	
	  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));  //�ȴ�ת�����
	  ADC_ConvertedValue=ADC_GetConversionValue(ADC1); //��ȡת�����*ADC_ConvertedValue*	
	  ADC_ConvertedValueLocal=(float)ADC_ConvertedValue*(3.3/4096);   //�����ʵ�ʵ�ѹֵ*ADC_ConvertedValueLocal*
		//�ɼ�PA1��ѹ
			
			
					if(strcmp(data,"#@ffdf")==0) //?????
			{
				GPIO_SetBits(GPIOC,EP_DOWN);
			}
		else if(strcmp(data,"#@ffef")==0) //?????
			{
				GPIO_SetBits(GPIOC,EP_UP);
			}
			else
			{
				GPIO_ResetBits(GPIOC,EP_UP|EP_DOWN);
			}
			//������Ϣ����
			
			
//				temp = GPIOB->IDR ;
//	Send_Data("%x", temp);
			
//		flag = 0;
//			if(flag == 0)
//			{

//				temp = GPIOB->IDR ;
////				delay_ms(10);
//					if(time_delay_ms==10)
//				{
//					time_delay_ms = 0;
//				temp1 = GPIOB->IDR ;
//				}
//				if(temp != temp1)
//				{
//					a=0;
//					Send_Data("%x", temp1);
//				}
//			else
//				{
//					flag = 1;
//				}
//			}
//			
//			if(flag == 1)
//			{
//				
//				if(temp == temp1)
//				{
//					a=1;
//				}
//			}
//			else
//			{
//				
//				flag = 0;
//			}
//			
//		//���͵����Ϣ��USB HUB��
		
			
			flag=0;
			temp = GPIOB->IDR ;
				delay_ms(10);
//					if(time_delay_ms==10)
//				{
//					time_delay_ms = 0;
				temp1 = GPIOB->IDR ;
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
			//		//���͵����Ϣ��USB HUB��
			
			
			if(flag_bootload == 6)
			{
				iap_load_bootload(0x08000000);
			}
			if(flag_SoftReset == 5)
			{
				SoftReset();
			}
			
			
  }
}

