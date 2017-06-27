#ifndef __POWER_H
#define __POWER_H
#include "stm32f10x.h"

extern void power_config(void);
extern void powerd_dcdc(u8 state);

#endif
