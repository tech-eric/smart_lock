#ifndef __NRF24L01_H
#define __NRF24L01_H
#define BYTE unsigned char  

#include "stm32f10x.h" 

#define TX_ADR_WIDTH    5   // 5  bytes TX(RX) address width

#define TX_PLOAD_WIDTH  1  // 32 bytes TX payload


#define NRF_READ_REG        0x00  // Define read command to register
#define NRF_WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define STA_MARK_RX     0X40
#define STA_MARK_TX     0X20
#define STA_MARK_MX     0X10	


#define NRF_SPI_CS_H GPIO_SetBits(GPIOA, GPIO_Pin_4) 
#define NRF_SPI_CS_L GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define NRF_SPI_CE_H GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define NRF_SPI_CE_L GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define NRF_SPI_IRQ  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

#define SPI_SEND(dat)		SPI_I2S_SendData(SPI1, dat)
#define SPI_RECV  			(SPI_I2S_ReceiveData(SPI1))

extern void nrf_config(void);
extern int nrf_send(char *str);
extern void nrf_clear_all(void);
extern int nrf(char *str);
extern uint8_t nrf_rd_reg(u8 reg);
extern uint8_t nrf_wr_reg(u8 reg, u8 dat);
extern void power_off(void);
extern void power_on(void);
extern void ifnnrf_rx_mode(void);

extern uint8_t nrf_rd_buf(uint8_t cmd, uint8_t *buf, uint8_t cnt);

#endif

