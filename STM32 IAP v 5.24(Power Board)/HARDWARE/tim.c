#include "tim.h"
#include "stdio.h"
#include "iap.h"
#include "sys.h"

void tim_Init()
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	
    TIM_TimeBaseStructure.TIM_Period = 10000-1; //����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ ��Χ��0x0000-0xFFFF
    TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //������������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //????
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//ʹ�ܻ���ʧ�� TIM ���жϣ������¼ͼ1
    TIM_Cmd(TIM4, ENABLE);
	
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //ָ��IRQͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //ָ����ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;        //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //�����IRQ�Ǳ�ʹ�ܻ���ʧ��
    NVIC_Init(&NVIC_InitStructure);  
}

void TIM4_IRQHandler(void)   
{
		extern u8 time;
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //��ȡ�ж�״̬
    {
		   TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//			printf("û�п��Ը��µĹ̼�!\r\n");
//			printf("��ʼִ��FLASH�û�����!!\r\n");
			time++;
//				if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
//			{	 
//				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
//			}
TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//??????

//TIM_Cmd(TIM4, DISABLE);  //??(?????)
    }

}//һ��TIM��ʱ��һ�룬Period = 10000��time������ʱN��TIM��ʱ�������� time = 5������ʱ5��
//��time�������������Ͽ���ʵ�����޼�����Ҳ��ֱ����һ���жϣ�ͨ��Periodʵ��һ���Եļ�����


