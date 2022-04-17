/*
 * stm32f401Re.h
 *
 *  Created on: Jan 20, 2021
 *      Author: Mahmoud
 */

#ifndef INC_STM32F401RE_H_
#define INC_STM32F401RE_H_

#include <stdint.h>


/*
 * ************************** processor specific addresses and  registers *****************************
 */

#define NVIC_BASEADDR           0xE000E100U

typedef struct
{
	volatile uint32_t ISER[3];     // GPIO port mode register
	uint32_t Reserved[4];
	volatile uint32_t ICER[3];     // GPIO port mode register
	uint32_t reserved[4];
	volatile uint32_t ISPR[3];     // GPIO port mode register
	uint32_t reserved1[4];
	volatile uint32_t ICPR[3];     // GPIO port mode register
	uint32_t reserved2[4];
	volatile uint32_t IABR[3];     // GPIO port mode register
	uint32_t reserved3[4];
	volatile uint32_t IPR[21];     // GPIO port mode register
	uint32_t reserved4[39];
	volatile uint32_t STIR;     // GPIO port mode register
} NVIC_RegDef_t;

#define NVIC         ( (NVIC_RegDef_t *) NVIC_BASEADDR )

// base addresses of flash and SRAM memories

#define FLASH_BASEADDR             0x08000000U
#define SRAM1_BASEADDR             0x20000000U
#define SRAM2_BASEADDR             0x2001C000U
#define SRAM                       SRAM1_BASEADDR


// peripheral bus domain base addresses

#define PERIPH_BASE                0x40000000U

#define AHB2PERIPH_BASE            0x50000000U
#define AHB1PERIPH_BASE            0x40020000U
#define APB2PERIPH_BASE            0x40010000U
#define APB1PERIPH_BASE            PERIPH_BASE

// ********************** AHB1 bus peripheral base addresses *****************8

#define GPIO_PORT_OFFSET_INCREMENT 0x400
#define GPIOA_BASEADDR             AHB1PERIPH_BASE
#define GPIOB_BASEADDR             (GPIOA_BASEADDR + GPIO_PORT_OFFSET_INCREMENT)
#define GPIOC_BASEADDR             (GPIOB_BASEADDR + GPIO_PORT_OFFSET_INCREMENT)

#define RCC_BASEADDR               (AHB1PERIPH_BASE + 0x3800U)

// **************** APB1 bus peripheral base addresses **********************

// I2C peripheral base addresses
#define I2C_OFFSET_INCREMENT       0x400
#define I2C1_BASEADDR              0x40005400U
#define I2C2_BASEADDR              (I2C1_BASEADDR + I2C_OFFSET_INCREMENT)
#define I2C3_BASEADDR              (I2C2_BASEADDR + I2C_OFFSET_INCREMENT)

// SPI peripheral base addresses
#define SPI_OFFSET_INCREMENT       0x400
#define SPI2_BASEADDR              0x40003800U
#define SPI3_BASEADDR              (SPI2_BASEADDR + SPI_OFFSET_INCREMENT)

// USART peripheral base addresses
#define USART2_BASEADDR            0x40004400U

// **********  APB2 bus peripheral base addresses *************************

// USART peripheral base addresses
#define USART1_BASEADDR            0x40011000U
#define USART6_BASEADDR            0x40011400U

// SPI peripheral base addresses
#define SPI1_BASEADDR              0x40013000U

// EXTI peripheral base addresses
#define EXTI_BASEADDR              0x40013C00U

// SYSCFG base address
#define SYSCFG_BASEADDR            0x40013800U


// ***************** peripheral register definition structs******************************

typedef struct
{
	volatile uint32_t MODER;     // GPIO port mode register
	volatile uint32_t OTYPER;    // GPIO port output type register
	volatile uint32_t OSPEEDR;   // GPIO port output speed register
	volatile uint32_t PUPDR;     // GPIO port pull-up/pull-down register
	volatile uint32_t IDR;       // GPIO port input data register
	volatile uint32_t ODR;       // GPIO port output data register
	volatile uint32_t BSRR;      // GPIO port bit set/reset register
	volatile uint32_t LCKR;      // GPIO port configuration lock register
	volatile uint32_t AFR[2];    // GPIO alternate function [low, high] register

} GPIO_RegDef_t;

typedef struct
{
	volatile uint32_t CR;             // RCC clock control register
	volatile uint32_t PLLCFGR;        // RCC PLL configuration register
	volatile uint32_t CFGR;           // RCC clock configuration register
	volatile uint32_t CIR;            // RCC clock interrupt register
	volatile uint32_t AHB1RSTR;       // RCC AHB1 peripheral reset register
	volatile uint32_t AHB2RSTR;       // RCC AHB2 peripheral reset register
	volatile uint32_t AHB3RSTR;       // RCC AHB3 peripheral reset register
	uint32_t Reserved0;
	volatile uint32_t APB1RSTR;       // RCC APB1 peripheral reset register
	volatile uint32_t APB2RSTR;       // RCC APB2 peripheral reset register
	uint32_t Reserved1[2];
	volatile uint32_t AHB1ENR;        // RCC AHB1 peripheral clock register
	volatile uint32_t AHB2ENR;        // RCC AHB2 peripheral clock enable register
	volatile uint32_t AHB3ENR;        // RCC AHB3 peripheral clock enable register
	uint32_t Reserved2;
	volatile uint32_t APB1ENR;        // RCC APB1 peripheral clock enable register
	volatile uint32_t APB2ENR;        // RCC APB2 peripheral clock enable register
	uint32_t Reserved3[2];
	volatile uint32_t AHB1LPENR;      // RCC AHB1 peripheral clock enable in low power mode register
	volatile uint32_t AHB2LPENR;      // RCC AHB2 peripheral clock enable in low power mode register
	volatile uint32_t AHB3LPENR;      // RCC AHB3 peripheral clock enable in low power mode register
	uint32_t Reserved4;
	volatile uint32_t APB1LPENR;      // RCC APB1 peripheral clock enable in low power mode register
	volatile uint32_t APB2LPENR;      // RCC APB2 peripheral clock enabled in low power mode register
	uint32_t Reserved5[2];
	volatile uint32_t BDCR;           // RCC Backup domain control register
	volatile uint32_t CSR;            // RCC clock control & status register
	uint32_t Reserved6[2];
	volatile uint32_t SSCGR;          // RCC spread spectrum clock generation register
	volatile uint32_t PLLI2SCFGR;     // RCC PLLI2S configuration register
	volatile uint32_t PLLSAICFGR;     // RCC PLL configuration register
	volatile uint32_t DCKCFGR;        // RCC Dedicated Clock Configuration Register
} RCC_RegDef_t;

typedef struct {
	volatile uint32_t MEMRMP;         // SYSCFG memory remap register
	volatile uint32_t PMC;            // SYSCFG peripheral mode configuration register
	volatile uint32_t EXTICR[4];      // SYSCFG external interrupt configuration registers
	uint32_t reserved[2];
	volatile uint32_t CMPCR;          // Compensation cell control register
} SYSCFG_RegDef_t;

typedef struct {
	volatile uint32_t IMR;    // Interrupt mask register
	volatile uint32_t EMR;    // Event mask register
	volatile uint32_t RTSR;   // Rising trigger selection register
	volatile uint32_t FTSR;   // Falling trigger selection register
	volatile uint32_t SWIER;  // Software interrupt event register
	volatile uint32_t PR;     // Software interrupt event register
} EXTI_RegDef_t;

typedef struct {
	volatile uint32_t CR1;      // SPI control register 1
	volatile uint32_t CR2;      // SPI control register 2
	volatile uint32_t SR;       // SPI status register
	volatile uint32_t DR;     	// SPI data register
	volatile uint32_t CRCPR;    // SPI CRC polynomial register
	volatile uint32_t RXCRCR;   // SPI RX CRC register
	volatile uint32_t TXCRCR;   // SPI TX CRC register
	volatile uint32_t I2SCFGR;  // SPI_I2S configuration register
	volatile uint32_t I2SPR;    // SPI_I2S prescaler register
} SPI_RegDef_t;


// **************** peripheral register struct pointers *********************************

// peripheral definitions
#define GPIOA               ( (GPIO_RegDef_t *) GPIOA_BASEADDR )
#define GPIOB               ( (GPIO_RegDef_t *) GPIOB_BASEADDR )
#define GPIOC               ( (GPIO_RegDef_t *) GPIOC_BASEADDR )
#define RCC                 ( (RCC_RegDef_t *) RCC_BASEADDR)
#define SYSCFG              ( (SYSCFG_RegDef_t *)  SYSCFG_BASEADDR)
#define EXTI                ( (EXTI_RegDef_t *)  EXTI_BASEADDR)
#define SPI1                ( (SPI_RegDef_t *) SPI1_BASEADDR)
#define SPI2                ( (SPI_RegDef_t *) SPI2_BASEADDR)
#define SPI3                ( (SPI_RegDef_t *) SPI3_BASEADDR)


/************************************************************************************************
 * 						SPI peripheral BIT FIELD macros
 ***********************************************************************************************/
#define SPI_CR1_CPHA        0
#define SPI_CR1_CPOL        1
#define SPI_CR1_MSTR        2
#define SPI_CR1_BR          3
#define SPI_CR1_SPE         6
#define SPI_CR1_SSI         8
#define SPI_CR1_SSM         9
#define SPI_CR1_RXONLY      10
#define SPI_CR1_DFF         11
#define SPI_CR1_BIDIOE      14
#define SPI_CR1_BIDIMODE    15

#define SPI_CR2_SSOE        2
#define SPI_CR2_FRF         4
#define SPI_CR2_ERRIE       5
#define SPI_CR2_RXNEIE      6
#define SPI_CR2_TXEIE       7

#define SPI_SR_RXNE         0
#define SPI_SR_TXE          1
#define SPI_SR_CHSIDE       2
#define SPI_SR_UDR          3
#define SPI_SR_CRCERR       4
#define SPI_SR_MODF         5
#define SPI_SR_OVR          6
#define SPI_SR_BSY          7
#define SPI_SR_FRE          8


/***********************************************************************************
 * 							Peripheral enable/disable macros
 *************************************************************************************/
#define SPI_PERIPH_ENABLE(pSPIx)        (pSPIx->CR1 |= (1 << SPI_CR1_SPE))
#define SPI_PERIPH_DISABLE(pSPIx)       (pSPIx->CR1 &= ~(1 << SPI_CR1_SPE))


// ************* peripheral clock enable ******************************

// AHB1 peripherals
#define GPIOA_PCLK_EN()     ( RCC->AHB1ENR |= (1 << 0) )
#define GPIOB_PCLK_EN()     ( RCC->AHB1ENR |= (1 << 1) )
#define GPIOC_PCLK_EN()     ( RCC->AHB1ENR |= (1 << 2) )

// APB1 peripherals
#define I2C1_PCLK_EN()      ( RCC->APB1ENR |= (1 << 21) )
#define I2C2_PCLK_EN()      ( RCC->APB1ENR |= (1 << 22) )
#define I2C3_PCLK_EN()      ( RCC->APB1ENR |= (1 << 23) )
#define SPI2_PCLK_EN()      ( RCC->APB1ENR |= (1 << 14) )
#define SPI3_PCLK_EN()      ( RCC->APB1ENR |= (1 << 15) )
#define USART2_PCLK_EN()    ( RCC->APB1ENR |= (1 << 17) )

// APB2 peripherals
#define USART1_PCLK_EN()    ( RCC->APB2ENR |= (1 << 4) )
#define USART6_PCLK_EN()    ( RCC->APB2ENR |= (1 << 5) )
#define SPI1_PCLK_EN()      ( RCC->APB2ENR |= (1 << 12) )
#define SYSCFG_PCLK_EN()    ( RCC->APB2ENR |= (1 << 14) )

// ************* peripheral clock disable ******************************

// AHB1 peripherals
#define GPIOA_PCLK_DI()     ( RCC->AHB1ENR &= ~(1 << 0) )
#define GPIOB_PCLK_DI()     ( RCC->AHB1ENR &= ~(1 << 1) )
#define GPIOC_PCLK_DI()     ( RCC->AHB1ENR &= ~(1 << 2) )

// APB1 peripherals
#define I2C1_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 21) )
#define I2C2_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 22) )
#define I2C3_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 23) )
#define SPI2_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 14) )
#define SPI3_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 15) )
#define USART2_PCLK_DI()    ( RCC->APB1ENR &= ~(1 << 17) )

// APB2 peripherals
#define USART1_PCLK_DI()    ( RCC->APB2ENR &= ~(1 << 4) )
#define USART6_PCLK_DI()    ( RCC->APB2ENR &= ~(1 << 5) )
#define SPI1_PCLK_DI()      ( RCC->APB2ENR &= ~(1 << 12) )
#define SYSCFG_PCLK_DI()    ( RCC->APB2ENR &= ~(1 << 14) )

/*
 * ************** GPIO peripheral reset macros *******************
 */
#define GPIOA_REG_RESET()   do{ ( RCC->AHB1RSTR |= (1 << 0) ); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()   do{ ( RCC->AHB1RSTR |= (1 << 1) ); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()   do{ ( RCC->AHB1RSTR |= (1 << 2) ); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define SPI1_REG_RESET()    do{ ( RCC->APB2RSTR |= (1 << 12) ); (RCC->APB2RSTR &= ~(1 << 12)); }while(0)
#define SPI2_REG_RESET()    do{ ( RCC->APB1RSTR |= (1 << 14) ); (RCC->APB1RSTR &= ~(1 << 14)); }while(0)
#define SPI3_REG_RESET()    do{ ( RCC->APB1RSTR |= (1 << 15) ); (RCC->APB1RSTR &= ~(1 << 15)); }while(0)

// ****************** some generic macros ****************
#define ENABLE              1
#define DISABLE             0
#define SET                 ENABLE
#define RESET               DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET

#define GPIO_BASEADDR_TO_PORTCODE(x)     ( (x == GPIOA)? 0 : \
										   (x == GPIOB)? 1 : \
										   (x == GPIOC)? 2 : -1)

/*
 * IRQ (interrupt request) numbers for MCU family
 */
#define IRQ_NO_EXTI0          6
#define IRQ_NO_EXTI1          7
#define IRQ_NO_EXTI2          8
#define IRQ_NO_EXTI3          9
#define IRQ_NO_EXTI4          10
#define IRQ_NO_EXTI9_5        23
#define IRQ_NO_EXTI15_10      40

#define DEFAULT_PRIORITY      -1

#include "gpio_driver.h"
#include "spi.h"

#endif /* INC_STM32F401RE_C_ */
