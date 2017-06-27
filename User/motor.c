/*********************************************************************************************************************

															��ʱ��2���ó�OC_PWM���ģʽ���������ڵ��ת��


**********************************************************************************************************************/
#include "motor.h"
#include "Delay.h"

#define TIM_Period_Value  	1000;//��ʱ�����Զ�װ��ֵ����PWM��Ƶ��Ϊ200HZ,�ֱ���Ϊ1/100
#define TIM_Prescal_Value 	(uint16_t)(SystemCoreClock /200000) - 1//��ʱ����Ԥ�ȷ�Ƶ�����Ƕ�ʱ����ʱ��Ϊ200000HZ,T=5us


static void Timer2_GPIO_Conf(){
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  			 TIM_OCInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure; 
	
		//����ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //�򿪶�ʱ��2��ʱ��
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE); //�򿪶�ʱ�����IO�ڵ�ʱ�Ӻ͸���ʱ��
	
	
		//����IOģʽ
		
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(GPIOA, &GPIO_InitStructure);

	
		//���ö�ʱ��
		TIM_DeInit(TIM2);//��ʼ����ʱ��
		
		
		TIM_TimeBaseStructure.TIM_Period = TIM_Period_Value;  //PWM�����ֱ���Ϊ1/1000
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


		/*��ʱ��Ԥװ��ʹ��*/
		TIM_ARRPreloadConfig(TIM2, ENABLE);

		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE);
		
}

/*********************************************************************************************************************


									���ڵ���ٶȣ����ڷ�Χ0~100�����뺯���ȼ������ֵ�Ƿ�������Χ��������ڣ���رյ��

**********************************************************************************************************************/

void  Motor_SetDutycycle(uint16_t TIM_CCR_Value[]){
	

	TIM2->CCR1=(TIM_CCR_Value[0]>100)?300:(TIM_CCR_Value[0]+300);
	TIM2->CCR2=(TIM_CCR_Value[1]>100)?300:(TIM_CCR_Value[1]+300);
	TIM2->CCR3=(TIM_CCR_Value[2]>100)?300:(TIM_CCR_Value[2]+300);
	TIM2->CCR4=(TIM_CCR_Value[3]>100)?300:(TIM_CCR_Value[3]+300);
	
}

/*********************************************************************************************************************


							�Զ������г�ƥ��

							ƥ�䷽ʽ�����ſ������ȴ�2S,		��-��-����ʾ��ߵ�ȷ��
												���ſ�����С�ȴ�1S,		N����������ʾ��ؽ�������-��������ʾ��͵�ȷ��
												ƥ����������������

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


									���������ʹ֮����������

									����˳�򣺽��������ͣ��ȴ�3s,����123��Ȼ��N����������ʾ��ؽ�����һ����������ʾ�����ɹ����������

**********************************************************************************************************************/

void Motor_Start(void){
	
	uint16_t TIM_CCR_Value_Min[4]={  0,  0,  0,  0};

	Motor_SetDutycycle(TIM_CCR_Value_Min);

	Delay( TIME_1S );
	Delay( TIME_1S );
	Delay( TIME_1S );
	
	
}

/*********************************************************************************************************************


							��ʱ����ʼ������

**********************************************************************************************************************/

void Motor_Init(void){

	Timer2_GPIO_Conf();
	
	//Motor_Start();
	
}

/*********************************************************************************************************************




**********************************************************************************************************************/

