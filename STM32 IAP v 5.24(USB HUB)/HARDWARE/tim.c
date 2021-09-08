#include "tim.h"
#include "stdio.h"
#include "iap.h"
#include "sys.h"

void tim_Init()
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	
    TIM_TimeBaseStructure.TIM_Period = 10000-1; //ÉèÖÃÁËÔÚÏÂÒ»¸ö¸üĞÂÊÂ¼ş×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ ·¶Î§ÊÇ0x0000-0xFFFF
    TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //ÉèÖÃÁËÓÃÀ´×÷Îª TIMx Ê±ÖÓÆµÂÊ³ıÊıµÄÔ¤·ÖÆµÖµ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //????
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//Ê¹ÄÜ»òÕßÊ§ÄÜ TIM µÄÖĞ¶Ï£¬Ïê¼û¸½Â¼Í¼1
    TIM_Cmd(TIM4, ENABLE);
	
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //Ö¸¶¨IRQÍ¨µÀ
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //Ö¸¶¨ÏÈÕ¼ÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;        //´ÓÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //¶¨ÒåµÄIRQÊÇ±»Ê¹ÄÜ»¹ÊÇÊ§ÄÜ
    NVIC_Init(&NVIC_InitStructure);  
}

void TIM4_IRQHandler(void)   
{
		extern u8 time;
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //»ñÈ¡ÖĞ¶Ï×´Ì¬
    {
		   TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//			printf("Ã»ÓĞ¿ÉÒÔ¸üĞÂµÄ¹Ì¼ş!\r\n");
//			printf("¿ªÊ¼Ö´ĞĞFLASHÓÃ»§´úÂë!!\r\n");
			time++;
//				if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//ÅĞ¶ÏÊÇ·ñÎª0X08XXXXXX.
//			{	 
//				iap_load_app(FLASH_APP1_ADDR);//Ö´ĞĞFLASH APP´úÂë
//			}
TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//??????

//TIM_Cmd(TIM4, DISABLE);  //??(?????)
    }

}//Ò»¸öTIM¶¨Ê±Æ÷Ò»Ãë£¬Period = 10000£¬timeÓÃÀ´¼ÆÊ±N¸öTIM¶¨Ê±Æ÷£¬ÀıÈç time = 5£¬ÔòÑÓÊ±5Ãë¡
//ÓÃtime¼ÆÊı¿ÉÒÔÀíÂÛÉÏ¿ÉÒÔÊµÏÖÎŞÏŞ¼ÆÊı£¬Ò²¿ÉÖ±½ÓÓÃÒ»´ÎÖĞ¶Ï£¬Í¨¹ıPeriodÊµÏÖÒ»´ÎĞÔµÄ¼ÆÊı¡£


