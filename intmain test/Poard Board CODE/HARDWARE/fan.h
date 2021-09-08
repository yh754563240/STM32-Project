#ifndef __FSN_H
#define __FSN_H

#include "stm32f10x.h"

#define FAN_CTRAL GPIO_Pin_6

void fan_pwm(u16 arr,u16 psc,u16 ple);
void fan_gpio(void);

#endif

