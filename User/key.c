#include "stm32f10x.h"
#include "key.h"

void Key_GPIO_Conf(void)
{
   	GPIO_InitTypeDef myStructure; //GPIO初始化结构体
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//启动GPIO时钟和映射管脚时钟
		//初始化LCD引脚
	  myStructure.GPIO_Pin=PIN5|PIN6;
	  myStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  myStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOE,&myStructure);
	
		
}



void Key_Init(void){

	Key_GPIO_Conf();
	
	
}
