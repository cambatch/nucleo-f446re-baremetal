#ifndef DRIVER_F446RE_H
#define DRIVER_F446RE_H

#include <stdint.h>

#define DISABLE 0
#define ENABLE 1
#define RESET DISABLE
#define SET ENABLE


// ==================== Processor Specific ====================
// NVIC interrupt set-enable registers
#define NVIC_ISER_BASE ((volatile uint32_t *)0xE000E100U)
#define NVIC_ISER0 ((volatile uint32_t *)0xE000E100U)
#define NVIC_ISER1 ((volatile uint32_t *)0xE000E104U)
#define NVIC_ISER2 ((volatile uint32_t *)0xE000E108U)
#define NVIC_ISER3 ((volatile uint32_t *)0xE000E10CU)

// NVIC interrupt clear-enable registers
#define NVIC_ICER_BASE ((volatile uint32_t *)0xE000E180U)
#define NVIC_ICER0 ((volatile uint32_t *)0xE000E180U)
#define NVIC_ICER1 ((volatile uint32_t *)0xE000E184U)
#define NVIC_ICER2 ((volatile uint32_t *)0xE000E188U)
#define NVIC_ICER3 ((volatile uint32_t *)0xE000E18CU)

// NVIC interrupt priority register base address
#define NVIC_IPRO_BASEADDR ((volatile uint32_t *)0xE000E400U)


// ==================== F446xx Specific ====================
// Bus base addresses
#define APB1_BASEADDR 0x40000000U
#define APB2_BASEADDR 0x40010000U
#define AHB1_BASEADDR 0x40020000U
#define AHB2_BASEADDR 0x50000000U
#define AHB3_BASEADDR 0xA0001000U

// RCC base address
#define RCC_BASEADDR ((AHB1_BASEADDR) + 0x3800)

// EXTI base address
#define EXTI_BASEADDR ((APB2_BASEADDR) + 0x3C00)

// SYSCFG base address
#define SYSCFG_BASEADDR ((APB2_BASEADDR) + 0x3800)

// GPIO base addresses
#define GPIOA_BASEADDR ((AHB1_BASEADDR) + 0x0000)
#define GPIOB_BASEADDR ((AHB1_BASEADDR) + 0x0400)
#define GPIOC_BASEADDR ((AHB1_BASEADDR) + 0x0800)
#define GPIOD_BASEADDR ((AHB1_BASEADDR) + 0x0C00)
#define GPIOE_BASEADDR ((AHB1_BASEADDR) + 0x1000)
#define GPIOF_BASEADDR ((AHB1_BASEADDR) + 0X1400)
#define GPIOG_BASEADDR ((AHB1_BASEADDR) + 0x1800)
#define GPIOH_BASEADDR ((AHB1_BASEADDR) + 0x1C00)

// SPI base addresses
#define SPI1_BASEADDR ((APB2_BASEADDR) + 0x3000)
#define SPI2_BASEADDR ((APB1_BASEADDR) + 0x3800)
#define SPI3_BASEADDR ((APB1_BASEADDR) + 0x3C00)
#define SPI4_BASEADDR ((APB2_BASEADDR) + 0x3400)

// I2C base addresses
#define I2C1_BASEADDR ((APB1_BASEADDR) + 0x5400)
#define I2C2_BASEADDR ((APB1_BASEADDR) + 0x5800)
#define I2C3_BASEADDR ((APB1_BASEADDR) + 0x5C00)

// UART/USART base addresses
#define USART1_BASEADDR ((APB2_BASEADDR) + 0x1000)
#define USART2_BASEADDR ((APB1_BASEADDR) + 0X4400)
#define USART3_BASEADDR ((APB1_BASEADDR) + 0x4800)
#define UART4_BASEADDR  ((APB1_BASEADDR) + 0x4C00)
#define UART5_BASEADDR  ((APB1_BASEADDR) + 0x5000)
#define USART6_BASEADDR ((APB2_BASEADDR) + 0x1400)

// EXTI interrupt numbers
#define IRQ_NUM_EXTI0     6
#define IRQ_NUM_EXTI1     7
#define IRQ_NUM_EXTI2     8
#define IRQ_NUM_EXTI3     9
#define IRQ_NUM_EXTI4     10
#define IRQ_NUM_EXTI9_5   23
#define IRQ_NUM_EXTI15_10 40

// SPI interrupt numbers
#define IRQ_NUM_SPI1 35
#define IRQ_NUM_SPI2 36
#define IRQ_NUM_SPI3 51
#define IRQ_NUM_SPI4 84

// I2C interrupt numbers
#define IRQ_NUM_I2C1_EV 31
#define IRQ_NUM_I2C1_ER 32
#define IRQ_NUM_I2C2_EV 33
#define IRQ_NUM_I2C2_ER 34
#define IRQ_NUM_I2C3_EV 72
#define IRQ_NUM_I2C3_ER 73

// UART/USART interrupt numbers
#define IRQ_NUM_USART1 37
#define IRQ_NUM_USART2 38
#define IRQ_NUM_USART3 39
#define IRQ_NUM_UART4  52
#define IRQ_NUM_UART5  53
#define IRQ_NUM_USART6 71

// RCC registers
typedef struct
{
    volatile uint32_t cr;
    volatile uint32_t pll_cfgr;
    volatile uint32_t cfgr;
    volatile uint32_t cir;
    volatile uint32_t ahb1_rstr;
    volatile uint32_t ahb2_rstr;
    volatile uint32_t ahb3_rstr;
    volatile uint32_t _reserved0;
    volatile uint32_t apb1_rstr;
    volatile uint32_t apb2_rstr;
    volatile uint32_t _reserved1;
    volatile uint32_t _reserved2;
    volatile uint32_t ahb1enr;
    volatile uint32_t ahb2enr;
    volatile uint32_t ahb3enr;
    volatile uint32_t _reserved3;
    volatile uint32_t apb1enr;
    volatile uint32_t apb2enr;
    volatile uint32_t _reserved4;
    volatile uint32_t _reserved5;
    volatile uint32_t ahb1_lpenr;
    volatile uint32_t ahb2_lpenr;
    volatile uint32_t ahb3_lpenr;
    volatile uint32_t _reserved6;
    volatile uint32_t apb1_lpenr;
    volatile uint32_t apb2_lpenr;
    volatile uint32_t _reserved7;
    volatile uint32_t _reserved8;
    volatile uint32_t bdcr;
    volatile uint32_t csr;
    volatile uint32_t _reserved9;
    volatile uint32_t _reserved10;
    volatile uint32_t sscgr;
    volatile uint32_t pll_i2s_cfgr;
    volatile uint32_t pll_sai_cfgr;
    volatile uint32_t dck_cfgr;
    volatile uint32_t ck_gatenr;
    volatile uint32_t dck_cfgr2;
} rcc_regdef_t;

// GPIO registers
typedef struct
{
    volatile uint32_t moder;
    volatile uint32_t otyper;
    volatile uint32_t ospeedr;
    volatile uint32_t pupdr;
    volatile uint32_t idr;
    volatile uint32_t odr;
    volatile uint32_t bsrr;
    volatile uint32_t lckr;
    volatile uint32_t afrl;
    volatile uint32_t afrh;
} gpio_regdef_t;

// EXTI registers
typedef struct
{
    volatile uint32_t imr;
    volatile uint32_t emr;
    volatile uint32_t rtsr;
    volatile uint32_t ftsr;
    volatile uint32_t swier;
    volatile uint32_t pr;
} exti_regdef_t;

//  SYSCFG registers
typedef struct
{
    volatile uint32_t memrmp;
    volatile uint32_t pmc;
    volatile uint32_t cr[4];
    volatile uint32_t exticmpcr;
    volatile uint32_t cfgr;
} syscfg_regdef_t;

// SPI registers
typedef struct
{
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t sr;
    volatile uint32_t dr;
    volatile uint32_t crcpr;
    volatile uint32_t rxcrcr;
    volatile uint32_t txcrcr;
    volatile uint32_t i2scfgr;
    volatile uint32_t i2spr;
} spi_regdef_t;


// ==================== Peripheral definitions ====================

// RCC
#define RCC ((rcc_regdef_t *)RCC_BASEADDR)

// EXTI
#define EXTI ((exti_regdef_t *)EXTI_BASEADDR)

// SYSCFG
#define SYSCFG ((syscfg_regdef_t *)SYSCFG_BASEADDR)

// GPIOx
#define GPIOA ((gpio_regdef_t *)GPIOA_BASEADDR)
#define GPIOB ((gpio_regdef_t *)GPIOB_BASEADDR)
#define GPIOC ((gpio_regdef_t *)GPIOC_BASEADDR)
#define GPIOD ((gpio_regdef_t *)GPIOD_BASEADDR)
#define GPIOE ((gpio_regdef_t *)GPIOE_BASEADDR)
#define GPIOF ((gpio_regdef_t *)GPIOF_BASEADDR)
#define GPIOG ((gpio_regdef_t *)GPIOG_BASEADDR)
#define GPIOH ((gpio_regdef_t *)GPIOH_BASEADDR)

// SPI
#define SPI1 ((spi_regdef_t *)SPI1_BASEADDR)
#define SPI2 ((spi_regdef_t *)SPI2_BASEADDR)
#define SPI3 ((spi_regdef_t *)SPI3_BASEADDR)
#define SPI4 ((spi_regdef_t *)SPI4_BASEADDR)

// ==================== Register macros ====================
#define SPI_CR1_CPHA        0
#define SPI_CR1_CPOL        1
#define SPI_CR1_MSTR        2
#define SPI_CR1_BR          3
#define SPI_CR1_SPE         6
#define SPI_CR1_LSBFIRST    7
#define SPI_CR1_SSI         8
#define SPI_CR1_SSM         9
#define SPI_CR1_RXONLY      10
#define SPI_CR1_DFF         11
#define SPI_CR1_CRCNEXT     12
#define SPI_CR1_CRCEN       13
#define SPI_CR1_BIDIOE      14
#define SPI_CR1_BIDIMODE    15

#define SPI_CR2_RXDNAEB     0
#define SPI_CR2_TXDMAEN     1
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

// MISC
#define GPIO_BASEADDR_TO_PORT(x) ((x == GPIOA) ? 0 : \
                                  (x == GPIOB) ? 1 : \
                                  (x == GPIOC) ? 2 : \
                                  (x == GPIOD) ? 3 : \
                                  (x == GPIOE) ? 4 : \
                                  (x == GPIOF) ? 5 : \
                                  (x == GPIOG) ? 6 : \
                                  (x == GPIOH) ? 7 : 0)

#include "rcc.h"
#include "gpio.h"
#include "spi.h"

#endif
