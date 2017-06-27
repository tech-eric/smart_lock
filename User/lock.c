#include "lock.h"
#include "stm32f10x.h" 
#include <stdio.h>
#include "common.h"
#include <string.h>

/**测似代码 初始化函数****/
void lock_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* config  clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   
}


int lock(char *cmd)
{
		int record[20];
		string_divide(cmd, record);
	
		if(strcmp(&cmd[record[1]], "on") == 0)
		{
				GPIO_SetBits(GPIOB, GPIO_Pin_3);
		}	
		else if(strcmp(&cmd[record[1]], "off") == 0)
		{ 
				GPIO_ResetBits(GPIOB, GPIO_Pin_3);
		}
		
		return 0;
}


