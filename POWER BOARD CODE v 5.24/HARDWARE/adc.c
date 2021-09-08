#include "adc.h"
#include "stdio.h"

        

void  Adc_Config(void)
{ 	
    /*定义两个初始化要用的结构体，下面给每个结构体成员赋值*/
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*
	  使能GPIOA和ADC1通道时钟
	  注意：除了RCC_APB2PeriphClockCmd还有RCC_APB1PeriphClockCmd，那么该如何选择？
      APB2：高速时钟，最高72MHz，主要负责AD输入，I/O，串口1，高级定时器TIM
      APB1：低速时钟，最高36MHz，主要负责DA输出，串口2、3、4、5，普通定时器TIM,USB,IIC,CAN，SPI
  	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //72M/6=12, ADC的采样时钟最快14MHz  
      
    /*配置输入电压所用的PA0引脚*/         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //GPIO_Mode_AIN：模拟输入（还有其他什么模式？请看下面的附录图1）
	GPIO_Init(GPIOA, &GPIO_InitStructure);	


//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  // 配置中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


	
	ADC_DeInit(ADC1); //复位，将ADC1相关的寄存器设为默认值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//工作模式：ADC1和ADC2独立工作模式  （还有其他什么模式？请看下面的附录图2）
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//数模转换工作：扫描（多通道）模式=ENABLE、单次（单通道）模式=DISABLE
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//数模转换工作：连续=ENABLE、单次=DISABLE
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC转换由软件触发启动 （还有其他什么模式？请看下面的附录图3）
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐   除了右就是左：ADC_DataAlign_Left
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目   范围是1-16
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADC1的寄存器
 	
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	  	  		    
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC  注意：函数ADC_Cmd只能在其他ADC设置函数之后被调用
	ADC_ResetCalibration(ADC1);	//重置指定的ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1)); //等待上一步操作完成
	ADC_StartCalibration(ADC1);	//开始指定ADC的校准状态	
	while(ADC_GetCalibrationStatus(ADC1));//等待上一步操作按成		
 }


//void ADC1_IRQHandler(void)
//{	
//	
//	extern float ADC_ConvertedValue; 
//	if (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )) 
//	{
//		// ??ADC????
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//启动转换	
//		ADC_ConvertedValue = ADC_GetConversionValue(ADC1);
//	}
//	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
//}



