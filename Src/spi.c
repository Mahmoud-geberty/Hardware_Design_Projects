/*
 * spi.c
 *
 *  Created on: Feb 6, 2021
 *      Author: user
 */

#include "stm32f401Re.h"


/*********************************************************************
 * @fn      		  - SPI_GetFlagStatus
 *
 * @brief             - Reads the SPI status register
 *
 * @param[in]         - base address of the spi peripheral
 * @param[in]         - possible values from @SPI_FLAGS
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t flagName)
{
	if (pSPIx->SR & flagName)
	{
		return SET;
	}
	return RESET;
}

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             - enable or disable spi peripheral clock
 *
 * @param[in]         - base address of the spi peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_PeriClockControl( SPI_RegDef_t *pSPIx, uint8_t EnorDi )
{
	if (EnorDi == ENABLE)
	{
		if (pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		} else if (pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		} else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	} else if (EnorDi == DISABLE)
	{
		if (pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		} else if (pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		} else if (pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}

	}
}


/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             - initialize the spix peripheral with the given configuration
 *
 * @param[in]         - pointer to spi handler struct
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_Init( SPI_handle_t *pSPIHandle )
{
	// configure device mode
	uint32_t tempreg = 0;

	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	// configure the bus
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		// clear the bidi bit
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		// set the bidi bit
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_S_RXONLY)
	{
		// clear the bidi bit
		// set the RXONLY bit
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	// configure baud rate
	tempreg |= ( (uint32_t) pSPIHandle->SPIConfig.SPI_Sclk_Speed << SPI_CR1_BR);

	// configure DFF
	tempreg |= ( (uint32_t) pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

	// configure CPOL AND CPHA
	tempreg |= ( pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL); //CPOL
	tempreg |= ( pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA); //CPHA

	// configure SSM
	tempreg |= ( pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM );
	tempreg |= ( pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSI ); // temp, set ssi same as ssm

	// set the SPI_CR1 register to tempreg
	pSPIHandle->pSPIx->CR1 |= tempreg;
	tempreg = 0;

}


/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             - reset all registered of given SPIx peripheral
 *
 * @param[in]         - pointer to SPIx register struct
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_DeInit( SPI_RegDef_t *pSPIx )
{
	if (pSPIx == SPI1)
		{
			SPI1_REG_RESET();
		} else if (pSPIx == SPI2)
		{
			SPI2_REG_RESET();
		} else if (pSPIx == SPI3)
		{
			SPI3_REG_RESET();
		}

}


/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  This is a blocking call
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while (Len)
	{
		// wait until TX buffer is empty
		while(!SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG));

		// check the DFF and send data accordingly
		if (pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			// 16 bit DFF
			pSPIx->DR = *( (uint16_t *) pTxBuffer );
			Len--;
			(uint16_t *) pTxBuffer++;
		} else
		{
			// 8 bit DFF
			pSPIx->DR = *pTxBuffer;
			pTxBuffer++;
		}

		Len--;
	}
}


/*********************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             - read bytes from rxBuffer when it is not empty
 *
 * @param[in]         - struct pointer to the spix peripheral
 * @param[in]         - buffer to store received bytes
 * @param[in]         - number of bytes to be received
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while (Len)
	{
		// wait until rxbuffer is populated
		while (!SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG));

		if (pSPIx->CR1 & (1 << SPI_CR1_DFF) )
		{
			// 16 bit data frame
			*( (uint16_t *)pRxBuffer ) = pSPIx->DR;
			(uint16_t *)pRxBuffer++;
			Len -= 2;
		} else
		{
			// 8 bit data frame.
			*pRxBuffer = pSPIx->DR;
			pRxBuffer++;
			Len--;
		}
	}

}


/*********************************************************************
 * @fn      		  - SPI_IRQInterruptConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_IRQInterruptConfig( uint8_t IRQ_Number,  uint8_t EnorDi)
{

}


/*********************************************************************
 * @fn      		  - SPI_IRQPriorityConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_IRQPriorityConfig( uint8_t IRQ_Number,  uint32_t priority)
{

}


/*********************************************************************
 * @fn      		  - SPI_IRQHandle
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_IRQHandle( SPI_handle_t *pSPIHandle )
{

}


/*********************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 * @brief             - set or reset the ssi bit in cr1 register
 *
 * @param[in]         - struct pointer to the spix register base
 * @param[in]         - ENABLE OR DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_SSIConfig( SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		// set the SSI BIT of SPI_CR1 register.
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	} else if (EnOrDi == DISABLE)
	{
		// rest the bit
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}

}


/*********************************************************************
 * @fn      		  - SPI_SSOEConfig
 *
 * @brief             - set or reset the SSOE bit in cr2 register
 *
 * @param[in]         - struct pointer to the spix register base
 * @param[in]         - ENABLE OR DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_SSOEConfig( SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		// set the SSOE BIT of SPI_CR2 register.
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	} else if (EnOrDi == DISABLE)
	{
		// rest the bit
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}

