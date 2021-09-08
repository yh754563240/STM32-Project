#include "tim_delay.h"
#include "sys.h"
#include "stdio.h"

void tim_Init()
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	
    TIM_TimeBaseStructure.TIM_Period = 10-1; //ÉèÖÃÁËÔÚÏÂÒ»¸ö¸üĞÂÊÂ¼ş×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ ·¶Î§ÊÇ0x0000-0xFFFF    //10000¾ÍÊÇ1Ãë£¬1000¾ÍÊÇ100ºÁÃë,100¾ÍÊÇ10ºÁÃë£¬10¾ÍÊÇ1ºÁÃë
    TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //ÉèÖÃÁËÓÃÀ´×÷Îª TIMx Ê±ÖÓÆµÂÊ³ıÊıµÄÔ¤·ÖÆµÖµ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //????
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//Ê¹ÄÜ»òÕßÊ§ÄÜ TIM µÄÖĞ¶Ï£¬Ïê¼û¸½Â¼Í¼1
    TIM_Cmd(TIM4, ENABLE);
	
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //Ö¸¶¨IRQÍ¨µÀ
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //Ö¸¶¨ÏÈÕ¼ÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;        //´ÓÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //¶¨ÒåµÄIRQÊÇ±»Ê¹ÄÜ»¹ÊÇÊ§ÄÜ
    NVIC_Init(&NVIC_InitStructure);  
}

void TIM4_IRQHandler(void)   
{

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //»ñÈ¡ÖĞ¶Ï×´Ì¬
    {
		   TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			time_delay_ms++;
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//??????

    }

}//Ò»¸öTIM¶¨Ê±Æ÷Ò»Ãë£¬Period = 10000£¬timeÓÃÀ´¼ÆÊ±N¸öTIM¶¨Ê±Æ÷£¬ÀıÈç time = 5£¬ÔòÑÓÊ±5Ãë¡
//ÓÃtime¼ÆÊı¿ÉÒÔÀíÂÛÉÏ¿ÉÒÔÊµÏÖÎŞÏŞ¼ÆÊı£¬Ò²¿ÉÖ±½ÓÓÃÒ»´ÎÖĞ¶Ï£¬Í¨¹ıPeriodÊµÏÖÒ»´ÎĞÔµÄ¼ÆÊı¡£



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
////ÓÃTIM4¶¨Ê±Æ÷ÊµÏÖÁË5ÃëÑÓÊ±
