 
#ifndef __MOTOR_H
#define __MOTOR_H
 
#include "stm32f10x.h"

extern	void		Motor_Init(void);//初始化,启动电机

extern	void 		Motor_AdaptRoute(void);//匹配量常，启动电机

extern 	void 		Motor_Start(void);

extern 	void  	Motor_SetDutycycle(uint16_t TIM_CCR_Value[]);//调节电机速度

#endif
