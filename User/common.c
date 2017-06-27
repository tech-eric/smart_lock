#include "common.h"
#include "stm32f10x.h"
#include "string.h"

#define SPACE 0x20

/* Find the next space character */
static int next_space(char *str, int i)
{
		if (str[i] == '\0')
			return -1;
		if (str[i] == SPACE)
			return i;
		else 
			return next_space(str, ++i);
}
/* Find the next normal character */
static int next_char(char *str, int i)
{
		if (str[i] == '\0')
			return -1;
		else if (str[i] != SPACE )
			return i;
		else 
			return next_char(str, ++i);
}

/*
*  string_divide: divide string to several section according space label
*  @str: the input string.
*  @record: a pointer to array that record the string section start address.
*  @return: the section number 
*/
int string_divide(char *str, int *record)
{
		int i = 0;//index for str
		int j = 0;//index for record.
		int ret = 0;
		int cnt = 0;//divide number
			
		for ( ; ; ) {
				
				if((ret = next_char(str, i)) >= 0) {
						cnt++;
						record[j++] = ret;
						i = ret +1;
				} else
						break;
				
				if((ret = next_space(str, i)) >= 0) {
						str[ret] = '\0';
						i = ret + 1;
				}else
						break;
		}
		return cnt;
}
/*
* 	string_first: get the first string from cmd line, usally, it's the cmd's name
*   @str: pointer to cmd line string
*   @buf: pointer to the string buf that will store the first string from cmd line.
*/
void string_first(char *str, char *buf)
{
	int start,stop;
	
	start = next_char(str, 0);
	stop = next_space(str, start);
	memcpy(buf, str, stop - start);
	buf[stop]= '\0';
}
