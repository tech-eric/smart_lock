#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

extern void USART1_Config(void);
extern int fputc(int ch, FILE *f);

#endif /* __USART1_H */
