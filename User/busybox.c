#include "stm32f10x.h"
#include "lock.h"
#include "audio.h"
#include "busybox.h"
#include "stdio.h"
#include "string.h"
#include "common.h"
#include "nrf24l01.h"


/* Make a function pointer typedefine */
typedef int (*FUNC)(char *);

/*
* A busybox command
*  @name: the command name
*  @func: the pointer to function that have the same name with command.
*/
struct cmd{
	char name[10];
	FUNC func;
};

static struct cmd cmd_list[] = {
	{"lock", &lock},
	{"audio", &audio},
	{"nrf", &nrf},
};

#define cmd_num sizeof(cmd_list)/sizeof(cmd_list[0])

int busy_box(char *string)
{
	int i;
	char cmd_name[20];
	
	if(string[0] == '\0')
		return -1;
	
	string_first(string, cmd_name);
	
	for (i = 0; i < cmd_num; i++) {
		if((strcmp(cmd_name, cmd_list[i].name)) == 0)
					cmd_list[i].func(string);
	}
		
	return 0;
}
