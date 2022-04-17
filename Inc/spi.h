/*
 * spi.h
 *
 *  Created on: Feb 6, 2021
 *      Author: user
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stm32f401Re.h"

typedef struct {
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_Sclk_Speed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;

}SPI_config_t;

typedef struct {
	SPI_config_t SPIConfig;
	SPI_RegDef_t *pSPIx;      /*!< This holds the base address of SPIx (x : 1, 2, 3) peripheral >*/
}SPI_handle_t;


/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER   1
#define SPI_DEVICE_MODE_SLAVE    0

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD              1
#define SPI_BUS_CONFIG_HD              2
#define SPI_BUS_CONFIG_S_TXONLY        3
#define SPI_BUS_CONFIG_S_RXONLY        4

/*
 * @SPI_Sclk_Speed
 */
#define SPI_SCLK_SPEED_DIV2            0
#define SPI_SCLK_SPEED_DIV4            1
#define SPI_SCLK_SPEED_DIV8            2
#define SPI_SCLK_SPEED_DIV16           3
#define SPI_SCLK_SPEED_DIV32           4
#define SPI_SCLK_SPEED_DIV64           5
#define SPI_SCLK_SPEED_DIV128          6
#define SPI_SCLK_SPEED_DIV256          7

/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS      0
#define SPI_DFF_16BITS     1

/*
 * @SPI_CPOL
 */
#define SPI_CPOL_HIGH     (unsigned) 1
#define SPI_CPOL_LOW      (unsigned) 0

/*
 * @SPI_CPHA
 */
#define SPI_CPHA_HIGH      (unsigned) 1
#define SPI_CPHA_LOW       (unsigned) 0

/*
 * @SPI_SSM
 */
#define SPI_SSM_EN          1
#define SPI_SSM_DI          0

/*
 * @SPI_FLAGS
 * SPI related flag bit masks
 */
#define SPI_TXE_FLAG           (1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG          (1 << SPI_SR_RXNE)
#define SPI_BSY_FLAG           (1 << SPI_SR_BSY)


/* **************************** spi prototypes ************************************ */

/*
 * SPI flag status read
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t flagName);
/*
 * SPI perIpheral clock control
 */
void SPI_PeriClockControl( SPI_RegDef_t *pSPIx, uint8_t EnorDi );

/*
 * Init and De-Init
 */
void SPI_Init( SPI_handle_t *pSPIHandle );
void SPI_DeInit( SPI_RegDef_t *pSPIx );

/*
 * Data send and receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ configuration and ISR handling
 */
void SPI_IRQInterruptConfig( uint8_t IRQ_Number,  uint8_t EnorDi);
void SPI_IRQPriorityConfig( uint8_t IRQ_Number,  uint32_t priority);
void SPI_IRQHandle( SPI_handle_t *pSPIHandle );

/*
 * NSS related pin config
 */
void SPI_SSIConfig( SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig( SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/*
 * Other peripheral control APIs
 */



#endif /* INC_SPI_H_ */
