#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f10x.h"

#define	ON	1
#define OFF	0
#define CMD_RECEIVED 1
#define CMD_CLEAR 0

extern char cmd_buf[];
extern char cmd_status;
extern int  buf_index;


extern int string_divide(char *str, int *record);
extern void string_first(char *str, char *buf);
extern int reboot(u8 state);

#endif
