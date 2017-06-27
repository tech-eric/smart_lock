#include "stm32f10x.h"
#include "busybox.h"
#include "usart1.h"
#include "audio.h"
#include "lock.h"
#include "Delay.h"
#include "common.h"
#include "rs485.h"
#include "nrf24l01.h"
#include "stm32f10x_pwr.h"
#include "power.h"
#include "rtc.h"
#include "string.h"


/* buf for common from uart */
#define CMD_BUF_LEN 20
char buf_debug[CMD_BUF_LEN];
char buf_cmd[CMD_BUF_LEN];
char buf_debug_status = DEBUG_CLEAR;
char buf_cmd_status = DEBUG_CLEAR;
int  buf_debug_index = 0;
int  buf_cmd_index = 0;


void RCC_Configuration(void)  
{  
    RCC_DeInit();//??? RCC?????????  
  
    RCC_HSICmd(ENABLE);//??HSI    
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//??HSI????  
  
    //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  
    //FLASH_SetLatency(FLASH_Latency_2);  
     
    RCC_HCLKConfig(RCC_SYSCLK_Div1);     
    RCC_PCLK1Config(RCC_HCLK_Div2);  
    RCC_PCLK2Config(RCC_HCLK_Div1);  
      
    //?? PLL ????????  
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2);//?????? PLL,???????:ENABLE??DISABLE   
    RCC_PLLCmd(ENABLE);//??PLL???????,????????  
    //????? RCC ??????? ??PLL?????  
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  
  
    //??????(SYSCLK) ??PLL??????  
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//?????????   
    //??PLL?????????????  
    //  0x00:HSI ??????   
    //  0x04:HSE??????   
    //  0x08:PLL??????    
    while(RCC_GetSYSCLKSource() != 0x08);//??????????????,RCC_SYSCLKSource_PLL  
  
  
}  

int main(void)
{
		int lock_status = OFF;
		//int work_status = SLEEP;
	
	  RCC_Configuration();
	
  	USART1_Config();
	  audio_config();
	  lock_config();
		rs485_config();
		nrf_config();
		power_config();
		RTC_Config();
		WKUP_NVIC_Config();

		//rs485();
		
		//audio(ON);
		//Delay(10000);
		//audio(OFF);
		
		printf("\r\n**WELCOM TO ENTER INTO BUSYBOX****\r\n");
	
		//nrf_clear_all();
		//power_off();
		//powerd_dcdc(OFF);
		/*
		//power_off();
		printf("Goodbaby!\r\n");
		Sleep(500);
		power_on();
		nrf_config();
		nrf_clear_all();
		buf_cmd_status = CMD_CLEAR;
		*/

		//power_on();

		//buf_cmd_index = 0;
		//Delay(100000);
		//Delay(5000000);
		while (1) {
	
				//printf("Hello World\r\n");
				if(buf_debug_status == DEBUG_RECEIVED) {
					busy_box(buf_debug);
					buf_debug_status = DEBUG_CLEAR;
				}
				
				if(buf_cmd_status == CMD_RECEIVED) {	
					buf_cmd_status = CMD_CLEAR;
					DEBUG("%s\r\n", buf_cmd);
					busy_box(buf_cmd);
				}
				
				if(work_status == SLEEP) {
					power_off();
					//printf("Goodbaby!\r\n");
					Sleep(1000);
					//power_off();
					//printf("Goodbaby!\r\n");
					//Sleep(1000);
					power_on();
					//ifnnrf_rx_mode();
					//nrf_clear_all();
					Delay(10000);
				}
		 }
}
