#ifndef __RTC_H
#define __RTC_H
#include "stm32f10x.h"

void RTC_Config(void);
void WKUP_NVIC_Config(void);
void Sleep(uint32_t AlarmValue);

#endif
