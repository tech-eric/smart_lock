#include "stm32f10x.h"
#include "busybox.h"
#include "usart1.h"
#include "audio.h"
#include "lock.h"
#include "Delay.h"
#include "common.h"
#include "rs485.h"
#include "nrf24l01.h"
#include "string.h"


/* buf for common from uart */
#define CMD_BUF_LEN 32
char cmd_buf[CMD_BUF_LEN];
char cmd_status = CMD_CLEAR;
int  buf_index=0;


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
		BYTE status;
	
	  RCC_Configuration();
	
  	USART1_Config();
	  audio_config();
	  lock_config();
		rs485_config();
		nrf_config();
	
		rs485();
		
		//audio(ON);
		//Delay(10000);
		//audio(OFF);
		
		printf("\r\n**WELCOM TO ENTER INTO BUSYBOX****\r\n");
	
		//nrf_send("Hello");
		while (1) {
	
				//printf("Hello World\r\n");
				if(cmd_status == CMD_RECEIVED) {
					cmd_status  = CMD_CLEAR;
					//busy_box(cmd_buf);
					if(strlen(cmd_buf) <= 2) 
						continue;
					while((nrf_send(cmd_buf)) == -1);
				}
				
				//nrf_send("wakeup");
				/*
				while((nrf_send("wakeup now")) == -1);
				Delay(1000);
				while((nrf_send("lock on")) == -1);
				Delay(1000);
				while((nrf_send("lock off")) == -1);
				Delay(1000);
				while((nrf_send("sleep now")) == -1);
				Delay(1000);
				*/
		}
		
}
