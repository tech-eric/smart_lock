/*********************************************************************************************************************

															定时器2配置成OC_PWM输出模式，用来调节电机转速


**********************************************************************************************************************/
#include "motor.h"
#include "Delay.h"

#define TIM_Period_Value  	1000;//定时器的自动装载值，即PWM波频率为200HZ,分辨率为1/100
#define TIM_Prescal_Value 	(uint16_t)(SystemCoreClock /200000) - 1//定时器的预先分频数，是定时器的时钟为200000HZ,T=5us


static void Timer2_GPIO_Conf(){
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  			 TIM_OCInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure; 
	
		//配置时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //打开定时器2的时钟
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE); //打开定时器输出IO口的时钟和复用时钟
	
	
		//配置IO模式
		
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(GPIOA, &GPIO_InitStructure);

	
		//配置定时器
		TIM_DeInit(TIM2);//初始化定时器
		
		
		TIM_TimeBaseStructure.TIM_Period = TIM_Period_Value;  //PWM调整分辨率为1/1000
		TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescal_Value;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	
		/* PWM1 Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC1Init(TIM2, &TIM_OCInitStructure);

		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

		/* PWM1 Mode configuration: Channel2 */
	
		TIM_OCInitStructure.TIM_Pulse = 0;

		TIM_OC2Init(TIM2, &TIM_OCInitStructure);

		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

		/* PWM1 Mode configuration: Channel3 */

		TIM_OCInitStructure.TIM_Pulse = 0;

		TIM_OC3Init(TIM2, &TIM_OCInitStructure);

		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

		/* PWM1 Mode configuration: Channel4 */
	
		TIM_OCInitStructure.TIM_Pulse = 0;

		TIM_OC4Init(TIM2, &TIM_OCInitStructure);

		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);


		/*定时器预装载使能*/
		TIM_ARRPreloadConfig(TIM2, ENABLE);

		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE);
		
}

/*********************************************************************************************************************


									调节电机速度，调节范围0~100。进入函数先检测输入值是否在允许范围，如果不在，则关闭电机

**********************************************************************************************************************/

void  Motor_SetDutycycle(uint16_t TIM_CCR_Value[]){
	

	TIM2->CCR1=(TIM_CCR_Value[0]>100)?300:(TIM_CCR_Value[0]+300);
	TIM2->CCR2=(TIM_CCR_Value[1]>100)?300:(TIM_CCR_Value[1]+300);
	TIM2->CCR3=(TIM_CCR_Value[2]>100)?300:(TIM_CCR_Value[2]+300);
	TIM2->CCR4=(TIM_CCR_Value[3]>100)?300:(TIM_CCR_Value[3]+300);
	
}

/*********************************************************************************************************************


							自动油门行程匹配

							匹配方式：油门开到最大等待2S,		哔-哔-，表示最高点确认
												油门开到最小等待1S,		N声短鸣声表示电池节数，哔-长鸣声表示最低点确认
												匹配结束，可以起飞了

**********************************************************************************************************************/

void Motor_AdaptRoute(void){
	
	uint16_t TIM_CCR_Value_Max[4]={100,100,100,100};
	uint16_t TIM_CCR_Value_Min[4]={0,0,0,0};

	Motor_SetDutycycle(TIM_CCR_Value_Max);

	Delay( TIME_1S );
	Delay( TIME_1S );
	Delay( TIME_1S );

 	
	Motor_SetDutycycle(TIM_CCR_Value_Min);

	Delay( TIME_1S );
	
	
}

/*********************************************************************************************************************


									启动电机，使之能正常工作

									启动顺序：将油门拉低，等待3s,发声123，然后N声短鸣声表示电池节数，一声长鸣声表示启动成功，可以起飞

**********************************************************************************************************************/

void Motor_Start(void){
	
	uint16_t TIM_CCR_Value_Min[4]={  0,  0,  0,  0};

	Motor_SetDutycycle(TIM_CCR_Value_Min);

	Delay( TIME_1S );
	Delay( TIME_1S );
	Delay( TIME_1S );
	
	
}

/*********************************************************************************************************************


							定时器初始化函数

**********************************************************************************************************************/

void Motor_Init(void){

	Timer2_GPIO_Conf();
	
	//Motor_Start();
	
}

/*********************************************************************************************************************




**********************************************************************************************************************/

