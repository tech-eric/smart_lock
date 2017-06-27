/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stdio.h"
#include "common.h"
#include "nrf24l01.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */



/****** RTC periodic wake up interrupt service routine*******/
void RTCAlarm_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{
		if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
		{
			/* Clear EXTI line17 pending bit */
			EXTI_ClearITPendingBit(EXTI_Line17);  // ?EXTI_Line17??? 
			/* Check if the Wake-Up flag is set */
			if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)// ?????????? 
			{
				/* Clear Wake Up flag */
				PWR_ClearFlag(PWR_FLAG_WU);
			}
    
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();   
			/* Clear RTC Alarm interrupt pending bit */
			RTC_ClearITPendingBit(RTC_IT_ALR);
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
			//printf("Hi, I am come back!\r\n");
		}
	}
}
/*
*  PA0 interrupt used for NRF24L01 communation
*
*/
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		nrf_rd_buf(RD_RX_PLOAD, (uint8_t *)buf_cmd, 32);
		buf_cmd_status = CMD_RECEIVED;
		nrf_clear_all();
	}
}


/**
  * @}
  */
void EXTI9_5_IRQHandler(void)
{ 
	
}

char RxData;

void  USART1_IRQHandler(void){
		
	if(USART_GetITStatus(USART1, USART_IT_RXNE)){
			
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
			RxData = USART_ReceiveData(USART1);
		
		  switch (RxData) {
				/*  Receive enter key, start a new lane and print a '#' and a label */
				case 0x0d:
					putchar(0x0a);
				case 0x0a:
					putchar(0x0d);
					putchar('#');
				  /* And '\0' to show that we reveiced a new cmd string */
				  buf_debug[buf_debug_index] = '\0';
				  buf_debug_status = DEBUG_RECEIVED;
					buf_debug_index = 0;
					break;
				case 0x7F:
					/* If we received a backspace key, delet the last reveice data and move the buf_index back */
					if(buf_debug_index > 0) {
						putchar(RxData);
						buf_debug_index--;
					}
					break;
				default: 
					/* Stroe the received cmd string */
					putchar(RxData);
				  buf_debug[buf_debug_index++] = RxData;
					if(buf_debug_index >= 32) {
						buf_debug_index = 0;
						buf_debug_status = DEBUG_RECEIVED;
					}
			}	
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
