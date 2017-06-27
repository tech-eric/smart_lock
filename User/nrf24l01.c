#include "stm32f10x.h" 
#include "stm32f10x_spi.h"
#include <stdio.h>
#include "common.h"
#include "nrf24l01.h"
#include <string.h>
#include "Delay.h"

BYTE  TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address

BYTE rx_buf[TX_PLOAD_WIDTH];

BYTE tx_buf[TX_PLOAD_WIDTH];


void nrf_spi_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;    

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
  
  /*!< Configure SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure MISO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure MOSI */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure CS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*!< Configure CE */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/*!< Configure IRQ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* SPI work mode setting up*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);
}


/*
*  nrf_rw: nrf write one byte and receive one byte
*  @data: data will be write
*  @return: the received byte
*/
static uint8_t nrf_rw(uint8_t data)
{
	uint8_t ret;
	
	while( SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);//waite for spi tx empoty
	SPI_SEND(data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//waite for nrf give back a byte
	ret = SPI_RECV;
	
	return ret;
}

/*
*  write the reg of nrf24l01
*/
uint8_t nrf_wr_reg(u8 reg, u8 dat)
{
	uint8_t status;
	
	NRF_SPI_CE_L;
  NRF_SPI_CS_L;
  status = nrf_rw(reg);  //select register
  nrf_rw(dat);  //write value to the reg
  NRF_SPI_CS_H;
  NRF_SPI_CE_H;
	
	return status;
}

/*
*  nrf_rd_reg: read one byte from nrf24l01
*  @reg: reg address that want to read
*  @return: the read out data
*/
uint8_t nrf_rd_reg(u8 reg)
{
	uint8_t ret;
	
	NRF_SPI_CE_L;
  NRF_SPI_CS_L;
  nrf_rw(reg);  //select register
  ret = nrf_rw(NOP);  //read reg
  NRF_SPI_CS_H;
  NRF_SPI_CE_H;
	
	return ret;
}

/*
*  nrf_rw_buf: write data to slave 
*  @cmd: command
*  @buf: pointer to buffer that store data to write.
*  @cnt: data number will be write.
*/

static uint8_t nrf_wr_buf(uint8_t cmd, uint8_t *buf, uint8_t cnt)
{
	uint8_t status;
	uint8_t i;
	
	NRF_SPI_CE_L;
  NRF_SPI_CS_L;
	/* reg reg cmd and receive status*/
	status = nrf_rw(cmd);
	/* write buf dat to slave or read data from slave to buf*/
	for (i = 0; i < cnt; i++) {
			nrf_rw(buf[i]);
	}
	
	NRF_SPI_CS_H;
  NRF_SPI_CE_H;

	return status;
}

/*
*  nrf_rw_buf: read data from slave 
*  @cmd: command
*  @buf: pointer to buffer that store received data.
*  @cnt: data number will be read.
*/

uint8_t nrf_rd_buf(uint8_t cmd, uint8_t *buf, uint8_t cnt)
{
	uint8_t status;
	uint8_t i;
	
	//NRF_SPI_CE_L;
  NRF_SPI_CS_L;
	/* reg reg cmd and receive status*/
	status = nrf_rw(cmd);
	/* write NOP to slave or read data from slave to buf*/
	for (i = 0; i < cnt; i++) {
			buf[i] = nrf_rw(NOP);
	}
	
	NRF_SPI_CS_H;
  //NRF_SPI_CE_H;

	return status;
}


void power_off(void)
{
    NRF_SPI_CE_L;
    nrf_wr_reg(NRF_WRITE_REG + CONFIG, 0x0D); 
    NRF_SPI_CE_H;
    Delay(10000);
}

void power_on(void)
{
    NRF_SPI_CE_L;
    nrf_wr_reg(NRF_WRITE_REG + CONFIG, 0x0f); 
    NRF_SPI_CE_H;
    Delay(10000);
}

void ifnnrf_rx_mode(void)
{
    power_off();
		NRF_SPI_CE_L;
	
  	nrf_wr_buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Use the same address on the RX device as the TX device

  	nrf_wr_reg(NRF_WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  	nrf_wr_reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  	nrf_wr_reg(NRF_WRITE_REG + RF_CH,0);        // Select RF channel 40
  	nrf_wr_reg(NRF_WRITE_REG + RX_PW_P0, 32); // Select same RX payload width as TX Payload width
  	nrf_wr_reg(NRF_WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
  	nrf_wr_reg(NRF_WRITE_REG + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..

  	NRF_SPI_CE_H; // Set CE pin high to enable RX device
}

/*
*  nrf_send_byte: send data by nrf2401
*  @str: the string that will be send
*/
int nrf_send(char *str)
{
		int str_len = 0;
	
		if( (str_len = strlen(str)) < TX_PLOAD_WIDTH) {
				strcpy((char *)tx_buf, str);
				printf("Send:%s(str_len=%d)\r\n", tx_buf, str_len);
		}else {
			printf("Error: Input string is too long!\n");
		}

	nrf_wr_reg(NRF_WRITE_REG+STATUS,0xff);//clear all the status flag
		
	nrf_wr_buf(WR_TX_PLOAD,tx_buf,str_len);
	/*
	while(NRF_SPI_IRQ);//waite for the data translate finish
		if((SPI_RD_REG(STATUS)) == 0x2e){          
			printf("nrf send finish\n");
	}*/
		return 0;
}


void nrf_clear_reg(BYTE R_T)
{               
	NRF_SPI_CE_L;
  NRF_SPI_CS_L; 
	
	if(R_T==1)                // CSN low, init SPI transaction
		nrf_rw(FLUSH_TX); // ..and write value to it..
	else
		nrf_rw(FLUSH_RX); // ..and write value to it..
	
	NRF_SPI_CS_H;
	NRF_SPI_CE_H;
}
/**********************************************************************************************

                           clear all data

**********************************************************************************************/
void nrf_clear_all(void)
{
	nrf_clear_reg(0);
	nrf_clear_reg(1);
	nrf_wr_reg(NRF_WRITE_REG+STATUS,0xff);
}


int nrf(char *str)
{
	int record[20];
	
	string_divide(str, record);
	nrf_send(&str[record[1]]);
	
	return 0;
}

void nrf_config(void)
{
		nrf_spi_init();
		ifnnrf_rx_mode();
}
