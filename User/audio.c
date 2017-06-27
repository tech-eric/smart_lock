#include "stm32f10x.h"
#include "common.h"
#include "audio.h"
#include "Delay.h"
#include "stdio.h"
#include <string.h>

void audio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* config  clock */
	RCC_APB2PeriphClockCmd(AUDIO_RCCGROUP, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = AUDIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(AUDIO_PINGROUP, &GPIO_InitStructure);   
}


int audio(char *cmd)
{
	int record[20];
	string_divide(cmd, record);
	
	if (strcmp(&cmd[record[1]], "on") == 0){
		GPIO_SetBits(AUDIO_PINGROUP, AUDIO_PIN);
		Delay(10000);
		Delay(1000);
		GPIO_ResetBits(AUDIO_PINGROUP, AUDIO_PIN);
	}
	else if(strcmp(&cmd[record[1]], "off") == 0)
		GPIO_ResetBits(AUDIO_PINGROUP, AUDIO_PIN);
	
	return 0;
}
