#ifndef DRIVER_RCC_H
#define DRIVER_RCC_H

#include "f446re.h"

// SYSCFG peripheral clock enable
#define SYSCFG_PCLK_EN() (RCC->apb2enr |= (1 << 14))

// SYSCFG peripheral clock disable
#define SYSCFG_PCLK_DI() (RCC->apb2enr &= ~(1 << 14))

// GPIO peripheral clock enable
#define GPIOA_PCLK_EN() (RCC->ahb1enr |= (1U << 0))
#define GPIOB_PCLK_EN() (RCC->ahb1enr |= (1U << 1))
#define GPIOC_PCLK_EN() (RCC->ahb1enr |= (1U << 2))
#define GPIOD_PCLK_EN() (RCC->ahb1enr |= (1U << 3))
#define GPIOE_PCLK_EN() (RCC->ahb1enr |= (1U << 4))
#define GPIOF_PCLK_EN() (RCC->ahb1enr |= (1U << 5))
#define GPIOG_PCLK_EN() (RCC->ahb1enr |= (1U << 6))
#define GPIOH_PCLK_EN() (RCC->ahb1enr |= (1U << 7))

// GPIO peripheral clock disable
#define GPIOA_PCLK_DI() (RCC->ahb1enr &= ~(1U << 0))
#define GPIOB_PCLK_DI() (RCC->ahb1enr &= ~(1U << 1))
#define GPIOC_PCLK_DI() (RCC->ahb1enr &= ~(1U << 2))
#define GPIOD_PCLK_DI() (RCC->ahb1enr &= ~(1U << 3))
#define GPIOE_PCLK_DI() (RCC->ahb1enr &= ~(1U << 4))
#define GPIOF_PCLK_DI() (RCC->ahb1enr &= ~(1U << 5))
#define GPIOG_PCLK_DI() (RCC->ahb1enr &= ~(1U << 6))
#define GPIOH_PCLK_DI() (RCC->ahb1enr &= ~(1U << 7))

// SPI peripheral clock enable
#define SPI1_PCLK_EN() (RCC->apb2enr |= (1U << 12))
#define SPI2_PCLK_EN() (RCC->apb1enr |= (1U << 14))
#define SPI3_PCLK_EN() (RCC->apb1enr |= (1U << 15))
#define SPI4_PCLK_EN() (RCC->apb2enr |= (1U << 13))

// SPI peripheral clock disable
#define SPI1_PCLK_DI() (RCC->apb2enr &= ~(1U << 12))
#define SPI2_PCLK_DI() (RCC->apb1enr &= ~(1U << 14))
#define SPI3_PCLK_DI() (RCC->apb1enr &= ~(1U << 15))
#define SPI4_PCLK_DI() (RCC->apb2enr &= ~(1U << 13))

#endif
