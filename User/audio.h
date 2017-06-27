#ifndef __AUDIO_H
#define __AUDIO_H

#define AUDIO_PINGROUP	GPIOB
#define AUDIO_PIN				GPIO_Pin_2
#define AUDIO_RCCGROUP  RCC_APB2Periph_GPIOB

extern void audio_config(void);
extern int audio(char *cmd);

#endif
