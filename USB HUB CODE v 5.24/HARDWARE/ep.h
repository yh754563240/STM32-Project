#ifndef __EP_H
#define __EP_H

#include "stm32f10x.h"

typedef  void (*iapfun)(void);

#define EP_ALL GPIO_Pin_4|GPIO_Pin_5
#define EP_DOWN GPIO_Pin_4
#define EP_UP GPIO_Pin_5

void ep_init(void);
void USB_HUB_init(void);
void iap_load_bootload(u32 appxaddr);
#endif

