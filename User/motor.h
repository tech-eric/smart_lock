 
#ifndef __MOTOR_H
#define __MOTOR_H
 
#include "stm32f10x.h"

extern	void		Motor_Init(void);//��ʼ��,�������

extern	void 		Motor_AdaptRoute(void);//ƥ���������������

extern 	void 		Motor_Start(void);

extern 	void  	Motor_SetDutycycle(uint16_t TIM_CCR_Value[]);//���ڵ���ٶ�

#endif
