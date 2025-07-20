#ifndef DRIVER_F446RE_H
#define DRIVER_F446RE_H

#include <stdint.h>

#define DISABLE 0
#define ENABLE 1
#define RESET DISABLE
#define SET ENABLE

// Bus base addresses
#define APB1_BASEADDR 0x40000000U
#define APB2_BASEADDR 0x40010000U
#define AHB1_BASEADDR 0x40020000U
#define AHB2_BASEADDR 0x50000000U
#define AHB3_BASEADDR 0xA0001000U

// RCC base address
#define RCC_BASEADDR ((AHB1_BASEADDR) + 0x3800)

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
    volatile uint32_t ss_cgr;
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



// ==================== Peripheral definitions ====================

// RCC
#define RCC ((rcc_regdef_t *)RCC_BASEADDR)

// GPIOx
#define GPIOA ((gpio_regdef_t *)GPIOA_BASEADDR)
#define GPIOB ((gpio_regdef_t *)GPIOB_BASEADDR)
#define GPIOC ((gpio_regdef_t *)GPIOC_BASEADDR)
#define GPIOD ((gpio_regdef_t *)GPIOD_BASEADDR)
#define GPIOE ((gpio_regdef_t *)GPIOE_BASEADDR)
#define GPIOF ((gpio_regdef_t *)GPIOF_BASEADDR)
#define GPIOG ((gpio_regdef_t *)GPIOG_BASEADDR)
#define GPIOH ((gpio_regdef_t *)GPIOH_BASEADDR)



#include "rcc.h"
#include "gpio.h"

#endif
