#ifndef __BATT_H
#define __BATT_H

#include "stm32f10x.h"

#define BATT_READ_ALL (GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11)
#define BATT_OUT_ALL (GPIO_Pin_9|GPIO_Pin_10)
#define BATT_FUL_IND GPIO_Pin_4
#define BATT_NRM_IND GPIO_Pin_5
#define BATT_UVP_IND GPIO_Pin_6
#define BATT_PWR_IND GPIO_Pin_7
#define BATT_AC_IND GPIO_Pin_8
#define BATT_ENB GPIO_Pin_9
#define BATT_SCL GPIO_Pin_10
#define BATT_SDA GPIO_Pin_11

void batt_init(void);
void batt_init_Read(void);

#endif

