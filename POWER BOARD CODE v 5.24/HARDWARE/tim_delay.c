#include "tim_delay.h"
#include "sys.h"
#include "stdio.h"

void tim_Init()
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	
    TIM_TimeBaseStructure.TIM_Period = 10-1; //����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ ��Χ��0x0000-0xFFFF    //10000����1�룬1000����100����,100����10���룬10����1����
    TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //������������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //????
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//ʹ�ܻ���ʧ�� TIM ���жϣ������¼ͼ1
    TIM_Cmd(TIM4, ENABLE);
	
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //ָ��IRQͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //ָ����ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;        //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //�����IRQ�Ǳ�ʹ�ܻ���ʧ��
    NVIC_Init(&NVIC_InitStructure);  
}

void TIM4_IRQHandler(void)   
{

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //��ȡ�ж�״̬
    {
		   TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			time_delay_ms++;
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//??????

    }

}//һ��TIM��ʱ��һ�룬Period = 10000��time������ʱN��TIM��ʱ�������� time = 5������ʱ5��
//��time�������������Ͽ���ʵ�����޼�����Ҳ��ֱ����һ���жϣ�ͨ��Periodʵ��һ���Եļ�����



//u8 time;
// int main()
// {
//	 while(1)
//	 {
//		 if(time==5)
//		 {
//			 time=0;
//		 }
//	 }
// }
////��TIM4��ʱ��ʵ����5����ʱ
