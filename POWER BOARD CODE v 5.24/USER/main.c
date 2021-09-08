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
float ADC_ConvertedValueLocal;//ADC采集电压


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
  batt_init_Read();//读取动力电池信号初始化
	ctral_init();
	Adc_Config();
	pwm_init();//PWM风扇输出初始化
//	tim_Init();
	delay_ms(10);
	temp = GPIOB->IDR ;	
	
	Send_Data("%x", temp);
  while (1)
  {
//		Send_Data("%x", temp);
		
//WFI_SET();		//执行WFI指令
		
		ctral_gpsctrl();//GPS电源
		ctral_cpctrl();//一体机电源
		
				if(ADC_ConvertedValueLocal>=2.8)
		{
			pwm_daty_updata(70);
		}
			else if(ADC_ConvertedValueLocal<=3.0)
			{
				pwm_daty_updata(20);
			}//风扇2档变速
		
			
			
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//启动转换	
	  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));  //等待转换完成
	  ADC_ConvertedValue=ADC_GetConversionValue(ADC1); //获取转换结果*ADC_ConvertedValue*	
	  ADC_ConvertedValueLocal=(float)ADC_ConvertedValue*(3.3/4096);   //计算出实际电压值*ADC_ConvertedValueLocal*
		//采集PA1电压
			
			
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
			//升降信息处理
			
			
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
//		//发送电池信息到USB HUB板
		
			
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
			//		//发送电池信息到USB HUB板
			
			
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

