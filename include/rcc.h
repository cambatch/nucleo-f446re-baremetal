#ifndef DRIVER_RCC_H
#define DRIVER_RCC_H

#include "f446re.h"

#define HSI_CLOCK_HZ 16000000U
#define HSE_CLOCK_HZ 8000000U

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

// GPIO peripheral reset
#define GPIOA_RESET() do { RCC->ahb1rstr |= (1U << 0); RCC->ahb1rstr &= ~(1U << 0); } while(0)
#define GPIOB_RESET() do { RCC->ahb1rstr |= (1U << 1); RCC->ahb1rstr &= ~(1U << 1); } while(0)
#define GPIOC_RESET() do { RCC->ahb1rstr |= (1U << 2); RCC->ahb1rstr &= ~(1U << 2); } while(0)
#define GPIOD_RESET() do { RCC->ahb1rstr |= (1U << 3); RCC->ahb1rstr &= ~(1U << 3); } while(0)
#define GPIOE_RESET() do { RCC->ahb1rstr |= (1U << 4); RCC->ahb1rstr &= ~(1U << 4); } while(0)
#define GPIOF_RESET() do { RCC->ahb1rstr |= (1U << 5); RCC->ahb1rstr &= ~(1U << 5); } while(0)
#define GPIOG_RESET() do { RCC->ahb1rstr |= (1U << 6); RCC->ahb1rstr &= ~(1U << 6); } while(0)
#define GPIOH_RESET() do { RCC->ahb1rstr |= (1U << 7); RCC->ahb1rstr &= ~(1U << 7); } while(0)

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

// SPI peripheral reset
#define SPI1_RESET() do { RCC->apb2rstr |= (1U << 12); RCC->apb2rstr &= ~(1U << 12); } while(0)
#define SPI2_RESET() do { RCC->apb1rstr |= (1U << 14); RCC->apb1rstr &= ~(1U << 14); } while(0)
#define SPI3_RESET() do { RCC->apb1rstr |= (1U << 15); RCC->apb1rstr &= ~(1U << 15); } while(0)
#define SPI4_RESET() do { RCC->apb2rstr |= (1U << 13); RCC->apb2rstr &= ~(1U << 13); } while(0)

// I2C peripheral clock enable
#define I2C1_PCLK_EN() (RCC->apb1enr |= (1U << 21))
#define I2C2_PCLK_EN() (RCC->apb1enr |= (1U << 22))
#define I2C3_PCLK_EN() (RCC->apb1enr |= (1U << 23))

// I2C peripheral clock disable
#define I2C1_PCLK_DI() (RCC->apb1enr &= ~(1U << 21))
#define I2C2_PCLK_DI() (RCC->apb1enr &= ~(1U << 22))
#define I2C3_PCLK_DI() (RCC->apb1enr &= ~(1U << 23))

// I2C peripheral reset
#define I2C1_RESET() do { RCC->apb1rstr |= (1U << 21); RCC->apb1rstr &= ~(1U << 21); } while(0)
#define I2C2_RESET() do { RCC->apb1rstr |= (1U << 22); RCC->apb1rstr &= ~(1U << 22); } while(0)
#define I2C3_RESET() do { RCC->apb1rstr |= (1U << 23); RCC->apb1rstr &= ~(1U << 23); } while(0)

// USART/UART clock enable
#define USART1_PCLK_EN() (RCC->apb2enr |= (1U << 4))
#define USART2_PCLK_EN() (RCC->apb1enr |= (1U << 17))
#define USART3_PCLK_EN() (RCC->apb1enr |= (1U << 18))
#define UART4_PCLK_EN()  (RCC->apb1enr |= (1U << 19))
#define UART5_PCLK_EN()  (RCC->apb1enr |= (1U << 20))
#define USART6_PCLK_EN() (RCC->apb2enr |= (1U << 5))

// USART/UART peripheral clock disable
#define USART1_PCLK_DI() (RCC->apb2enr &= ~(1U << 4))
#define USART2_PCLK_DI() (RCC->apb1enr &= ~(1U << 17))
#define USART3_PCLK_DI() (RCC->apb1enr &= ~(1U << 18))
#define UART4_PCLK_DI()  (RCC->apb1enr &= ~(1U << 19))
#define UART5_PCLK_DI()  (RCC->apb1enr &= ~(1U << 20))
#define USART6_PCLK_DI() (RCC->apb2enr &= ~(1U << 5))

// USART/UART peripheral reset
#define USART1_RESET() do { RCC->apb2rstr |= (1U << 4); RCC->apb2rstr &= ~(1U << 4); } while(0)
#define USART2_RESET() do { RCC->apb1rstr |= (1U << 17); RCC->apb1rstr &= ~(1U << 17); } while(0)
#define USART3_RESET() do { RCC->apb1rstr |= (1U << 18); RCC->apb1rstr &= ~(1U << 18); } while(0)
#define UART4_RESET()  do { RCC->apb1rstr |= (1U << 19); RCC->apb1rstr &= ~(1U << 19); } while(0)
#define UART5_RESET()  do { RCC->apb1rstr |= (1U << 20); RCC->apb1rstr &= ~(1U << 20); } while(0)
#define USART6_RESET() do { RCC->apb2rstr |= (1U << 5); RCC->apb2rstr &= ~(1U << 5); } while(0)


// Clock types
typedef enum rcc_clock_t
{
    RCC_CLOCK_HSI = 0,  // High speed internal oscillator
    RCC_CLOCK_HSE,      // High speed external oscillator
    RCC_CLOCK_PLL_P,    // PLL output (main system clock)
    RCC_CLOCK_PLL_R     // PLL R output (e.g., I2S/SAI peripherals)
} rcc_clock_t;

rcc_clock_t rcc_system_clock();

uint32_t rcc_system_clock_hz();

uint16_t rcc_ahb_prescalar();

uint8_t rcc_apb1_prescalar();

uint8_t rcc_apb2_prescalar();

uint32_t rcc_apb1_clock_hz();

uint32_t rcc_apb2_clock_hz();

#endif
