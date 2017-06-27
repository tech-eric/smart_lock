#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f10x.h"

#define	ON	1
#define OFF	0
#define DEBUG_RECEIVED 1
#define DEBUG_CLEAR 0
#define CMD_RECEIVED 1 
#define CMD_CLEAR 0
#define WAKEUP 0
#define SLEEP 1

#define __DEBUG__

#ifdef	__DEBUG__
	#define	DEBUG(fmt, args...)  printf( "DEBUG: %s()@%d: " ## fmt, __func__, __LINE__, ##args)
#else
	#define	DEBUG(fmt, args...) 
#endif

extern char buf_debug[];
extern char buf_cmd[];
extern char buf_debug_status;
extern char buf_cmd_status;
extern int  buf_debug_index;
extern int  buf_cmd_index;

extern int string_divide(char *str, int *record);
extern void string_first(char *str, char *buf);
extern int reboot(u8 state);
extern int wakeup(char *str);
extern int sleep(char *str);


extern int work_status;

#endif
