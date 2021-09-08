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
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//启动转换	
//	  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));  //等待转换完成
//	  ADC_ConvertedValue=ADC_GetConversionValue(ADC1); //获取转换结果*ADC_ConvertedValue*	
//	  ADC_ConvertedValueLocal=(float)ADC_ConvertedValue*(3.3/4096);   //计算出实际电压值*ADC_ConvertedValueLocal*
//	  }
//}


//ADC例程在其他项目主函数中应加入以上函数
