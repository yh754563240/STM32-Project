#ifndef __CLOCK_H
#define __CLOCK_H

#include "stm32f10x.h"

void clock_sys_init(void);//系统时钟设置，包括PLL，系统时钟，AHB，APB1 , APB2，ADC时钟，USB时钟

#endif
