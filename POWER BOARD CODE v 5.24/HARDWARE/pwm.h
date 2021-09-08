#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
// enable pwm pin for stm32f103c8t6
// 注意！！！：以下引脚部分有冲突，使用前请根据手册来避免引脚功能冲突，否则有可能造成不可预料的错误
#define PWM_PA8_ENABLE          (0)         // TIM1_CH1
#define PWM_PA9_ENABLE          (0)         // TIM1_CH2
#define PWM_PA10_ENABLE         (0)         // TIM1_CH3
#define PWM_PA11_ENABLE         (0)         // TIM1_CH4
#define PWM_PA0_ENABLE          (0)         // TIM2_CH1
#define PWM_PA1_ENABLE          (0)         // TIM2_CH2
#define PWM_PA2_ENABLE          (0)         // TIM2_CH3
#define PWM_PA3_ENABLE          (0)         // TIM2_CH4
#define PWM_PA6_ENABLE          (1)         // TIM3_CH1
#define PWM_PA7_ENABLE          (0)         // TIM3_CH2
#define PWM_PB0_ENABLE          (0)         // TIM3_CH3
#define PWM_PB1_ENABLE          (0)         // TIM3_CH4
#define PWM_PB6_ENABLE          (0)         // TIM4_CH1
#define PWM_PB7_ENABLE          (0)         // TIM4_CH2
#define PWM_PB8_ENABLE          (0)         // TIM4_CH3
#define PWM_PB9_ENABLE          (0)         // TIM4_CH4

#if(PWM_PA8_ENABLE || PWM_PA9_ENABLE || PWM_PA10_ENABLE || PWM_PA11_ENABLE)
#define TIM1_ENABLE             (1)     // do not edit
#define TIM1_FREQUENCY          (20000)  // set pwm frequency for tim1
#define TIM1_DUTY               (50)    // set pwm duty-cycle for tim1
#else
#define TIM1_ENABLE             (0)     // do not edit
#endif

#if(PWM_PA0_ENABLE || PWM_PA1_ENABLE || PWM_PA2_ENABLE || PWM_PA3_ENABLE)
#define TIM2_ENABLE             (1)     // do not edit
#define TIM2_FREQUENCY          (500)   // set pwm frequency for tim2
#define TIM2_DUTY               (50)    // set pwm duty-cycle for tim2
#else
#define TIM2_ENABLE             (0)     // do not edit
#endif

#if(PWM_PA6_ENABLE || PWM_PA7_ENABLE || PWM_PB0_ENABLE || PWM_PB1_ENABLE)

#define TIM3_ENABLE             (1)     // do not edit
#define TIM3_FREQUENCY          (20000)  // set pwm frequency for tim3
#define TIM3_DUTY               (30)    // set pwm duty-cycle for tim3
#else
#define TIM3_ENABLE             (0)     // do not edit
#endif

#if(PWM_PB6_ENABLE || PWM_PB7_ENABLE || PWM_PB8_ENABLE || PWM_PB9_ENABLE)
#define TIM4_ENABLE             (1)     // do not edit
#define TIM4_FREQUENCY          (1000)  // set pwm frequency for tim4
#define TIM4_DUTY               (50)    // set pwm duty-cycle for tim4
#else
#define TIM4_ENABLE             (0)     // do not edit
#endif

#if(TIM1_ENABLE)
#define TIM1_Prescaler          (72-1)                      // 预分频器(0~65535)
#define TIM1_Period             (1000000/TIM1_FREQUENCY)    // 自动重装载计数器(0~65535)
#define TIM1_ClockDivision      (TIM_CKD_DIV1)              // 时钟分频
#define TIM1_TIM_CounterMode    (TIM_CounterMode_Up)        // 计数模式
#define TIM1_RepetitionCounter  (0)                         // 高级计数器用于控制PWM个数

#define TIM1_OCMode             (TIM_OCMode_PWM1)           // PWM1模式
#define TIM1_OutputState        (TIM_OutputState_Enable)    // 输出使能
#define TIM1_OCPolarity         (TIM_OCPolarity_High)       // 输出极性
#define TIM1_Pulse              (TIM1_DUTY*TIM1_Period/100) // 占空比计数
#endif

#if(TIM2_ENABLE)
#define TIM2_Prescaler          (72-1)                      // 预分频器(0~65535)
#define TIM2_Period             (1000000/TIM2_FREQUENCY)    // 自动重装载计数器(0~65535)
#define TIM2_ClockDivision      (TIM_CKD_DIV1)              // 时钟分频
#define TIM2_TIM_CounterMode    (TIM_CounterMode_Up)        // 计数模式
#define TIM2_RepetitionCounter  (0)                         // 高级计数器用于控制PWM个数

#define TIM2_OCMode             (TIM_OCMode_PWM1)           // PWM1模式
#define TIM2_OutputState        (TIM_OutputState_Enable)    // 输出使能
#define TIM2_OCPolarity         (TIM_OCPolarity_High)       // 输出极性
#define TIM2_Pulse              (TIM2_DUTY*TIM2_Period/100) // 占空比计数
#endif

#if(TIM3_ENABLE)
#define TIM3_Prescaler          (72-1)                      // 预分频器(0~65535)
#define TIM3_Period             (1000000/TIM3_FREQUENCY)    // 自动重装载计数器(0~65535)
#define TIM3_ClockDivision      (TIM_CKD_DIV1)              // 时钟分频
#define TIM3_TIM_CounterMode    (TIM_CounterMode_Up)        // 计数模式
#define TIM3_RepetitionCounter  (0)                         // 高级计数器用于控制PWM个数

#define TIM3_OCMode             (TIM_OCMode_PWM1)           // PWM1模式
#define TIM3_OutputState        (TIM_OutputState_Enable)    // 输出使能
#define TIM3_OCPolarity         (TIM_OCPolarity_High)       // 输出极性
#define TIM3_Pulse              (TIM3_DUTY*TIM3_Period/100) // 占空比计数
#endif

#if(TIM4_ENABLE)
#define TIM4_Prescaler          (72-1)                      // 预分频器(0~65535)
#define TIM4_Period             (1000000/TIM4_FREQUENCY)    // 自动重装载计数器(0~65535)
#define TIM4_ClockDivision      (TIM_CKD_DIV1)              // 时钟分频
#define TIM4_TIM_CounterMode    (TIM_CounterMode_Up)        // 计数模式
#define TIM4_RepetitionCounter  (0)                         // 高级计数器用于控制PWM个数

#define TIM4_OCMode             (TIM_OCMode_PWM1)           // PWM1模式
#define TIM4_OutputState        (TIM_OutputState_Enable)    // 输出使能
#define TIM4_OCPolarity         (TIM_OCPolarity_High)       // 输出极性
#define TIM4_Pulse              (TIM4_DUTY*TIM4_Period/100) // 占空比计数
#endif

void pwm_init(void);
void pwm_daty_updata(u16 FAN_DATY);//更新占空比实时函数
#endif /* __PWM_H */ 
