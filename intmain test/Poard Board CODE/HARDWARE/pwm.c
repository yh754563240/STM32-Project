/*
 * @Author: Liuxo
 * @Date: 2021-01-08 18:14:56
 * @LastEditors: Liuxo
 * @LastEditTime: 2021-01-08 21:27:58
 * @Description: None
 */
#include "pwm.h"

static void pwm_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t gpio_pin = 0;
	(void)GPIO_InitStructure;
	(void)gpio_pin;
	
	#if(PWM_PA8_ENABLE || PWM_PA9_ENABLE || PWM_PA10_ENABLE ||\
    PWM_PA11_ENABLE || PWM_PA0_ENABLE || PWM_PA1_ENABLE ||\
    PWM_PA2_ENABLE || PWM_PA3_ENABLE || PWM_PA6_ENABLE ||\
    PWM_PA7_ENABLE) 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    gpio_pin = 0;
		#if(PWM_PA8_ENABLE)
			gpio_pin |= GPIO_Pin_8;
		#endif
		#if(PWM_PA9_ENABLE)
			gpio_pin |= GPIO_Pin_9;
		#endif
		#if(PWM_PA10_ENABLE)
			gpio_pin |= GPIO_Pin_10;
		#endif
		#if(PWM_PA11_ENABLE)
			gpio_pin |= GPIO_Pin_11;
		#endif 
		#if(PWM_PA0_ENABLE)
			gpio_pin |= GPIO_Pin_0;
		#endif
		#if(PWM_PA1_ENABLE)
			gpio_pin |= GPIO_Pin_1;
		#endif
		#if(PWM_PA2_ENABLE)
			gpio_pin |= GPIO_Pin_2;
		#endif
		#if(PWM_PA3_ENABLE)
			gpio_pin |= GPIO_Pin_3;
		#endif 
		#if(PWM_PA6_ENABLE)
			gpio_pin |= GPIO_Pin_6;
		#endif
		#if(PWM_PA7_ENABLE)
			gpio_pin |= GPIO_Pin_7;
		#endif
    GPIO_InitStructure.GPIO_Pin = gpio_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	#endif

	#if(PWM_PB0_ENABLE || PWM_PB1_ENABLE || PWM_PB6_ENABLE ||\
				PWM_PB7_ENABLE || PWM_PB8_ENABLE || PWM_PB9_ENABLE)    
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			gpio_pin = 0;
		#if(PWM_PB0_ENABLE)
			gpio_pin |= GPIO_Pin_0;
		#endif
		#if(PWM_PB1_ENABLE)
			gpio_pin |= GPIO_Pin_1;
		#endif 
		#if(PWM_PB6_ENABLE)
			gpio_pin |= GPIO_Pin_6;
		#endif
		#if(PWM_PB7_ENABLE)
			gpio_pin |= GPIO_Pin_7;
		#endif
		#if(PWM_PB8_ENABLE)
			gpio_pin |= GPIO_Pin_8;
		#endif
		#if(PWM_PB9_ENABLE)
			gpio_pin |= GPIO_Pin_9;
		#endif 
    GPIO_InitStructure.GPIO_Pin = gpio_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
}

static void pwm_tim_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	(void)TIM_TimeBaseStructure;
	(void)TIM_OCInitStructure;
	#if(TIM1_ENABLE)
    // tim config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseStructure.TIM_Period             = TIM1_Period;
    TIM_TimeBaseStructure.TIM_Prescaler          = TIM1_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision      = TIM1_ClockDivision;
    TIM_TimeBaseStructure.TIM_CounterMode        = TIM1_TIM_CounterMode;
    TIM_TimeBaseStructure.TIM_RepetitionCounter  = TIM1_RepetitionCounter;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    // output complete mode config
    TIM_OCInitStructure.TIM_OCMode               = TIM1_OCMode;
    TIM_OCInitStructure.TIM_OutputState          = TIM1_OutputState;
    TIM_OCInitStructure.TIM_OCPolarity           = TIM1_OCPolarity;
    TIM_OCInitStructure.TIM_Pulse                = TIM1_Pulse;
		#if(PWM_PA8_ENABLE)  
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		#endif  
		#if(PWM_PA9_ENABLE)  
			TIM_OC2Init(TIM1, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
		#endif 
		#if(PWM_PA10_ENABLE)  
			TIM_OC3Init(TIM1, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		#endif 
		#if(PWM_PA11_ENABLE)  
			TIM_OC4Init(TIM1, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
		#endif 
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
	#endif

	#if(TIM2_ENABLE)
    // tim config
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period             = TIM2_Period;
    TIM_TimeBaseStructure.TIM_Prescaler          = TIM2_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision      = TIM2_ClockDivision;
    TIM_TimeBaseStructure.TIM_CounterMode        = TIM2_TIM_CounterMode;
    TIM_TimeBaseStructure.TIM_RepetitionCounter  = TIM2_RepetitionCounter;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    // output complete mode config
    TIM_OCInitStructure.TIM_OCMode               = TIM2_OCMode;
    TIM_OCInitStructure.TIM_OutputState          = TIM2_OutputState;
    TIM_OCInitStructure.TIM_OCPolarity           = TIM2_OCPolarity;
    TIM_OCInitStructure.TIM_Pulse                = TIM2_Pulse;
		#if(PWM_PA0_ENABLE)  
			TIM_OC1Init(TIM2, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
		#endif  
		#if(PWM_PA1_ENABLE)  
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
		#endif 
		#if(PWM_PA2_ENABLE)  
			TIM_OC3Init(TIM2, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
		#endif 
		#if(PWM_PA3_ENABLE)  
			TIM_OC4Init(TIM2, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		#endif 
    TIM_Cmd(TIM2, ENABLE);
	#endif

	#if(TIM3_ENABLE)
    // tim config
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period             = TIM3_Period;
    TIM_TimeBaseStructure.TIM_Prescaler          = TIM3_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision      = TIM3_ClockDivision;
    TIM_TimeBaseStructure.TIM_CounterMode        = TIM3_TIM_CounterMode;
    TIM_TimeBaseStructure.TIM_RepetitionCounter  = TIM3_RepetitionCounter;
    // output complete mode config
    TIM_OCInitStructure.TIM_OCMode               = TIM3_OCMode;
    TIM_OCInitStructure.TIM_OutputState          = TIM3_OutputState;
    TIM_OCInitStructure.TIM_OCPolarity           = TIM3_OCPolarity;
    TIM_OCInitStructure.TIM_Pulse                = TIM3_Pulse;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		#if(PWM_PA6_ENABLE)  
			TIM_OC1Init(TIM3, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		#endif  
		#if(PWM_PA7_ENABLE)  
			TIM_OC2Init(TIM3, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		#endif 
		#if(PWM_PB0_ENABLE)  
			TIM_OC3Init(TIM3, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		#endif 
		#if(PWM_PB1_ENABLE)  
			TIM_OC4Init(TIM3, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		#endif 
    TIM_Cmd(TIM3, ENABLE);
	#endif

	#if(TIM4_ENABLE)
    // tim config
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseStructure.TIM_Period             = TIM4_Period;
    TIM_TimeBaseStructure.TIM_Prescaler          = TIM4_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision      = TIM4_ClockDivision;
    TIM_TimeBaseStructure.TIM_CounterMode        = TIM4_TIM_CounterMode;
    TIM_TimeBaseStructure.TIM_RepetitionCounter  = TIM4_RepetitionCounter;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    // output complete mode config
    TIM_OCInitStructure.TIM_OCMode               = TIM4_OCMode;
    TIM_OCInitStructure.TIM_OutputState          = TIM4_OutputState;
    TIM_OCInitStructure.TIM_OCPolarity           = TIM4_OCPolarity;
    TIM_OCInitStructure.TIM_Pulse                = TIM4_Pulse;
		#if(PWM_PB6_ENABLE)  
			TIM_OC1Init(TIM4, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		#endif  
		#if(PWM_PB7_ENABLE)  
			TIM_OC2Init(TIM4, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		#endif 
		#if(PWM_PB8_ENABLE)  
			TIM_OC3Init(TIM4, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		#endif 
		#if(PWM_PB9_ENABLE)  
			TIM_OC4Init(TIM4, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		#endif 
			TIM_Cmd(TIM4, ENABLE);
	#endif
}

void pwm_init(void)
{
	pwm_gpio_init();
	pwm_tim_init();
}
