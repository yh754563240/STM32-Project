#include "adc.h"
#include "stdio.h"

        

void  Adc_Config(void)
{ 	
    /*����������ʼ��Ҫ�õĽṹ�壬�����ÿ���ṹ���Ա��ֵ*/
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*
	  ʹ��GPIOA��ADC1ͨ��ʱ��
	  ע�⣺����RCC_APB2PeriphClockCmd����RCC_APB1PeriphClockCmd����ô�����ѡ��
      APB2������ʱ�ӣ����72MHz����Ҫ����AD���룬I/O������1���߼���ʱ��TIM
      APB1������ʱ�ӣ����36MHz����Ҫ����DA���������2��3��4��5����ͨ��ʱ��TIM,USB,IIC,CAN��SPI
  	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //72M/6=12, ADC�Ĳ���ʱ�����14MHz  
      
    /*���������ѹ���õ�PA0����*/         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //GPIO_Mode_AIN��ģ�����루��������ʲôģʽ���뿴����ĸ�¼ͼ1��
	GPIO_Init(GPIOA, &GPIO_InitStructure);	


//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  // �����ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


	
	ADC_DeInit(ADC1); //��λ����ADC1��صļĴ�����ΪĬ��ֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ��ADC1��ADC2��������ģʽ  ����������ʲôģʽ���뿴����ĸ�¼ͼ2��
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//��ģת��������ɨ�裨��ͨ����ģʽ=ENABLE�����Σ���ͨ����ģʽ=DISABLE
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//��ģת������������=ENABLE������=DISABLE
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADCת��������������� ����������ʲôģʽ���뿴����ĸ�¼ͼ3��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���   �����Ҿ�����ADC_DataAlign_Left
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ   ��Χ��1-16
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADC1�ļĴ���
 	
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	  	  		    
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC  ע�⣺����ADC_Cmdֻ��������ADC���ú���֮�󱻵���
	ADC_ResetCalibration(ADC1);	//����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���һ���������
	ADC_StartCalibration(ADC1);	//��ʼָ��ADC��У׼״̬	
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ���һ����������		
 }


//void ADC1_IRQHandler(void)
//{	
//	
//	extern float ADC_ConvertedValue; 
//	if (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )) 
//	{
//		// ??ADC????
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//����ת��	
//		ADC_ConvertedValue = ADC_GetConversionValue(ADC1);
//	}
//	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
//}



