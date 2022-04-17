/*
 * gpio_driver.c
 *
 *  Created on: Jan 20, 2021
 *      Author: user
 */

#include "stm32f401Re.h"

/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_PeriClockControl( GPIO_RegDef_t *pGPIOx, uint8_t EnorDi )
{
	if (EnorDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
	} else if (EnorDi == DISABLE)
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}

	}
}


/*********************************************************************
 * @fn      		  - GPIO_Init
 *
 * @brief             - This function initializes all gpio registers with the given configurations
 *
 * @param[in]         - the  gpio handler struct with all the confugarations populated
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_Init( GPIO_handle_t *pGPIOHandle )
{
	uint32_t temp = 0;

    // configure the mode of gpio pin
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		// the non interrupt modes
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode  << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~( 0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
		pGPIOHandle->pGPIOx->MODER |= temp;

	} else
	{
		// handle interrupt pin modes
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			// configure FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			// configure RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			// configure both FTSR and RTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		// configure GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_PORTCODE(pGPIOHandle->pGPIOx);

		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] |= portcode << (4 * temp2);

		// enable the EXTI interrupt delivery using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	temp = 0;

	// configure pin speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed  << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~( 0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	temp = 0;

	// configure pin pupd
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl  << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~( 0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp = 0;

	// configure pin optype
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType  << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~( 0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	// configure gpio pin alternate function
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint8_t temp1 = 0, temp2 = 0;

		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2) );
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2) );
	}
}

/*********************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             - This function resets the registers of a GPIO port.
 *
 * @param[in]         - base address of a gpio port
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_DeInit( GPIO_RegDef_t *pGPIOx )
{
	if (pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
}

/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPin
 *
 * @brief             - return the data stored in given pin
 *
 * @param[in]         - GPIO port base address
 * @param[in]         - pin number to read from
 * @param[in]         -
 *
 * @return            - data read from the specified pin
 *
 * @Note              -
 */
 uint8_t GPIO_ReadFromInputPin( GPIO_RegDef_t *pGPIOx, uint8_t pinNumber )
{
	 uint8_t value = 0;

	 value = (uint8_t) (pGPIOx->IDR >> pinNumber) & 0x1;

	 return value;
}

 /*********************************************************************
  * @fn      		   - GPIO_ReadFromInputPort
  *
  * @brief             - This function reads the whole port data.
  *
  * @param[in]         - GPIO port base address
  * @param[in]         -
  * @param[in]         -
  *
  * @return            - data read from the specified port
  *
  * @Note              -
  */
uint16_t GPIO_ReadFromInputPort( GPIO_RegDef_t *pGPIOx )
{
	uint16_t value = 0;

	value = (uint16_t) pGPIOx->IDR;
    return value;
}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPin
 *
 * @brief             - This function writes a bit to specified bit field of specified port
 *
 * @param[in]         - GPIO port base address
 * @param[in]         - pin number to write to
 * @param[in]         - value to be set at the bit field (binary)
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_WriteToOutputPin( GPIO_RegDef_t *pGPIOx, uint16_t pinNumber, uint8_t value )
{
	if (value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1 << pinNumber);
	}else
	{
		pGPIOx->ODR &= ~(1 << pinNumber);
	}

}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOuputPort
 *
 * @brief             - write a value to an entire GPIO port
 *
 * @param[in]         - GPIO port base address
 * @param[in]         - 16bit value for the whole port to be set to
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_WriteToOuputPort( GPIO_RegDef_t *pGPIOx, uint16_t value )
{
	pGPIOx->ODR = value;
}

/*********************************************************************
 * @fn      		  - GPIO_ToggleOutputPin
 *
 * @brief             - toggle the data in an output pin
 *
 * @param[in]         - GPIO port base address
 * @param[in]         - pin number to be toggled
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_ToggleOutputPin( GPIO_RegDef_t *pGPIOx, uint8_t pinNumber )
{
	pGPIOx->ODR ^= (1 << pinNumber);
}

/*********************************************************************
 * @fn      		  - GPIO_IRQConfig
 *
 * @brief             - configure the NVIC registers for the IRQ
 *
 * @param[in]         - IRQ number of the requested interrupt
 * @param[in]         - priority number of the requested interrupt,
 *                      the lower the number the higher the priority
 *
 * @param[in]         - ENABLE/DISABLE interrupt at given IRQ number
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_IRQConfig( uint8_t IRQ_Number,  uint8_t EnorDi, uint32_t IRQ_Priority )
{
	uint8_t temp1 = IRQ_Number / 32;
	uint8_t temp2 = IRQ_Number % 32;

	if (EnorDi == ENABLE)
	{
		// enable the interrupt at IRQ_Number
		NVIC->ISER[temp1] |= (1 << temp2);
	} else if (EnorDi == DISABLE)
	{
		// enable the interrupt at IRQ_Number
		NVIC->ICER[temp1] |= (1 << temp2);
	}

	temp1 = temp2 = 0;

	/*
	 * set the priority number for the interrupt at IRQ_Number
	 */
	if (IRQ_Priority != DEFAULT_PRIORITY)
	{
		temp1 = IRQ_Number / 4;
		temp2 = (IRQ_Number % 4) - 4;

		NVIC->IPR[temp1] |= (IRQ_Priority << 8 * temp2);
	}

}

/*********************************************************************
 * @fn      		  -
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_IRQHandle( uint8_t pinNumber )
{
	// clear the exti PR register corresponding to the pin number
	if (EXTI->PR & (1 << pinNumber))
	{
		EXTI->PR |= (1 << pinNumber);
	}
}
