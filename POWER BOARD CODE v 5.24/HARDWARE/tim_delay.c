#include "tim_delay.h"
#include "sys.h"
#include "stdio.h"

void tim_Init()
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	
    TIM_TimeBaseStructure.TIM_Period = 10-1; //设置了在下一个更新事件装入活动的自动重装载寄存器周期的值 范围是0x0000-0xFFFF    //10000就是1秒，1000就是100毫秒,100就是10毫秒，10就是1毫秒
    TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置了用来作为 TIMx 时钟频率除数的预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //????
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能或者失能 TIM 的中断，详见附录图1
    TIM_Cmd(TIM4, ENABLE);
	
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //指定IRQ通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //指定先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;        //从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //定义的IRQ是被使能还是失能
    NVIC_Init(&NVIC_InitStructure);  
}

void TIM4_IRQHandler(void)   
{

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //获取中断状态
    {
		   TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			time_delay_ms++;
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//??????

    }

}//一个TIM定时器一秒，Period = 10000，time用来计时N个TIM定时器，例如 time = 5，则延时5秒?
//用time计数可以理论上可以实现无限计数，也可直接用一次中断，通过Period实现一次性的计数。



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
////用TIM4定时器实现了5秒延时
