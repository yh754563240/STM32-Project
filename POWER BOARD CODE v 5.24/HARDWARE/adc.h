#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"


void  Adc_Config(void);

#endif

//float ADC_ConvertedValue; 
//float ADC_ConvertedValueLocal;
//int main(void)
//{
//while(1)
//		{
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//����ת��	
//	  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));  //�ȴ�ת�����
//	  ADC_ConvertedValue=ADC_GetConversionValue(ADC1); //��ȡת�����*ADC_ConvertedValue*	
//	  ADC_ConvertedValueLocal=(float)ADC_ConvertedValue*(3.3/4096);   //�����ʵ�ʵ�ѹֵ*ADC_ConvertedValueLocal*
//	  }
//}


//ADC������������Ŀ��������Ӧ�������Ϻ���
