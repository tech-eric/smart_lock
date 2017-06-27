
#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

#define  TIME_100mS	 100000
#define  TIME_1S   	1000000

extern void Delay(uint32_t time);  //对应时间是1us*time


#endif
