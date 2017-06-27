#include <stm32f10x.h>


void RTC_Config(void)
{
/* Enable the PWR clock */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
/* Allow access to RTC */
PWR_BackupAccessCmd(ENABLE);
BKP_DeInit();
RCC_LSEConfig(RCC_LSE_OFF);
RCC_LSICmd(ENABLE);

while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
{ } /* Wait till LSi is ready */
/* Select the RTC Clock Source */
RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);//??LSI??RTC???
/* Enable the RTC Clock */
RCC_RTCCLKCmd(ENABLE);//??RTC
RTC_WaitForSynchro();
RTC_SetPrescaler(310); //?RTC WAKE UP TIMER????
RTC_WaitForLastTask();
RTC_ITConfig(RTC_IT_ALR, ENABLE);  
RTC_WaitForLastTask();
}
/********RTC wake up interrupt NVIC ********/
void WKUP_NVIC_Config(void)
{
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

/* Configure EXTI line 20 (connected to the RTC wakeup event) */
EXTI_ClearITPendingBit(EXTI_Line17);
EXTI_InitStructure.EXTI_Line = EXTI_Line17;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);
/* NVIC configuration */
NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

/* Enable peirodic wakeup interrupt */
RTC_WaitForLastTask();
RTC_ITConfig(RTC_IT_ALR, ENABLE); //
/* Clear the wakeup Pending Bit */
RTC_WaitForLastTask();
RTC_ClearITPendingBit(RTC_IT_ALR);
}


void Sleep(uint32_t AlarmValue)
{
  /* Wait till RTC Second event occurs */
  RTC_ClearFlag(RTC_FLAG_SEC);
  while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);
  

  RTC_SetAlarm(RTC_GetCounter()+ AlarmValue);    //????+????
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
  /* Request to enter STOP mode with regulator in low power mode*/
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
  //PWR_EnterSTANDBYMode();
  /* Configures system clock after wake-up from STOP: enable HSE, PLL and select 
  PLL as system clock source (HSE and PLL are disabled in STOP mode) */
  //SYSCLKConfig_STOP();
}
