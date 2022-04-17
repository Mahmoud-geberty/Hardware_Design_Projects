/*
 * gpio_driver.h
 *
 *  Created on: Jan 20, 2021
 *      Author: user
 */

#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_

#include <stdint.h>

#include "stm32f401Re.h"

typedef struct
{
	uint8_t GPIO_PinNumber;           /*!< possible value from @GPIO_PIN_NUMBERS >*/
	uint8_t GPIO_PinMode;             /*!< possible value from @GPIO_PIN_MODES >*/
	uint8_t GPIO_PinSpeed;            /*!< possible value from @GPIO_PIN_SPEED >*/
	uint8_t GPIO_PinPuPdControl;      /*!< possible value from @GPIO_PIN_PUPD >*/
	uint8_t GPIO_PinOPType;           /*!< possible value from @GPIO_PIN_OPTYPE >*/
	uint8_t GPIO_PinAltFunMode;       /*!< possible value from @GPIO_PIN_SPEED >*/
} GPIO_PinConfig_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;  // hold base address of port to which pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_handle_t;


/*
 * @GPIO_PIN_NUMBERS
 * gpio port possible pin numbers
 */
#define GPIO_PIN_NO_0   0
#define GPIO_PIN_NO_1   1
#define GPIO_PIN_NO_2   2
#define GPIO_PIN_NO_3   3
#define GPIO_PIN_NO_4   4
#define GPIO_PIN_NO_5   5
#define GPIO_PIN_NO_6   6
#define GPIO_PIN_NO_7   7
#define GPIO_PIN_NO_8   8
#define GPIO_PIN_NO_9   9
#define GPIO_PIN_NO_10  10
#define GPIO_PIN_NO_11  11
#define GPIO_PIN_NO_12  12
#define GPIO_PIN_NO_13  13
#define GPIO_PIN_NO_14  14
#define GPIO_PIN_NO_15  15


/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define GPIO_MODE_IN       0
#define GPIO_MODE_OUT      1
#define GPIO_MODE_ALTFN    2    // alternate function
#define GPIO_MODE_ANALOG   3
#define GPIO_MODE_IT_FT    4    // Interrupt rising edge trigger
#define GPIO_MODE_IT_RT    5    // Interrupt falling edge trigger
#define GPIO_MODE_IT_RFT   6    // Interrupt rising edge falling edge trigger


/*
 * @GPIO_PIN_ALTFN_MODES
 * GPIO pin possible alternate function numbers
 */
#define GPIO_ALTFN_MODE_1   1
#define GPIO_ALTFN_MODE_2   2
#define GPIO_ALTFN_MODE_3   3
#define GPIO_ALTFN_MODE_4   4
#define GPIO_ALTFN_MODE_5   5

/*
 * @GPIO_PIN_OPTYPE
 * GPIO pin output types
 */
#define GPIO_OP_TYPE_PP    0
#define GPIO_OP_TYPE_OD    1


/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW      0
#define GPIO_SPEED_MEDIUM   1
#define GPIO_SPEED_FAST     2
#define GPIO_SPEED_HIGH     3


/*
 * @GPIO_PIN_PUPD
 * GPIO pull up AND pull down configuration
 */
#define GPIO_NO_PUPD  0
#define GPIO_PIN_PU   1
#define GPIO_PIN_PD   2

// peripheral clock setup
void GPIO_PeriClockControl( GPIO_RegDef_t *pGPIOx, uint8_t EnorDi );

// Init and De-Init
void GPIO_Init( GPIO_handle_t *pGPIOHandle );
void GPIO_DeInit( GPIO_RegDef_t *pGPIOx );

// data read and write
uint8_t GPIO_ReadFromInputPin( GPIO_RegDef_t *pGPIOx, uint8_t pinNumber );
uint16_t GPIO_ReadFromInputPort( GPIO_RegDef_t *pGPIOx );
void GPIO_WriteToOutputPin( GPIO_RegDef_t *pGPIOx, uint16_t pinNumber, uint8_t value );
void GPIO_WriteToOuputPort( GPIO_RegDef_t *pGPIOx, uint16_t value );
void GPIO_ToggleOutputPin( GPIO_RegDef_t *pGPIOx, uint8_t pinNumber );

// IRQ config and handling
void GPIO_IRQConfig( uint8_t IRQ_Number,  uint8_t EnorDi, uint32_t IRQ_Priority);
void GPIO_IRQHandle( uint8_t pinNumber );

#endif /* INC_GPIO_DRIVER_H_ */
