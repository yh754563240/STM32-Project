#ifndef __CTRAL_H
#define __CTRAL_H

#include "stm32f10x.h"

#define CTRAL_ALL (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4)
#define	GPS_12V GPIO_Pin_0
#define	GPS_5V GPIO_Pin_1
#define	CP_12V GPIO_Pin_2
#define	EP_DOWN GPIO_Pin_3
#define	EP_UP GPIO_Pin_4
#define	VDD_USB GPIO_Pin_1

void ctral_init(void);
void ctral_gpsctrl(void);
void ctral_cpctrl(void);
void ctral_epctrl(void);

#endif


