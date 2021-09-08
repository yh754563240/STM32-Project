#ifndef __UNUSED_H
#define __UNUSED_H

#include "stm32f10x.h"

#define UNUSED_A (GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15)
#define UNUSED_B (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)
#define UNUSED_C (GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13)
#define UNUSED_D GPIO_Pin_2

void unused_init(void);

#endif




