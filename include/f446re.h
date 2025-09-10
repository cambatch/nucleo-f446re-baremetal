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

// Debug specific
#define DFSR_BASEADDR  ((volatile uint32_t *)0xE000ED30U) // Debug fault status register
#define DHCSR_BASEADDR ((volatile uint32_t *)0xE000EDF0U) // Debug halting control and status register
#define DCRSR_BASEADDR ((volatile uint32_t *)0xE000EDF4U) // Debug core register selector register
#define DCRDR_BASEADDR ((volatile uint32_t *)0xE000EDF8U) // Debug core register data register
#define DEMCR_BASEADDR ((volatile uint32_t *)0xE000EDFCU) // Debug exception and monitor control register

#define DWT_BASEADDR ((volatile uint32_t *)0xE0001000U)

typedef struct
{
    volatile uint32_t ctrl;
    volatile uint32_t cyccnt;
    volatile uint32_t cpicnt;
    volatile uint32_t exccnt;
    volatile uint32_t sleepcnt;
    volatile uint32_t lsucnt;
    volatile uint32_t foldcnt;
    volatile uint32_t pcsr;

    volatile uint32_t comp0;
    volatile uint32_t mask0;
    volatile uint32_t function0;
    volatile uint32_t _reserved0;

    volatile uint32_t comp1;
    volatile uint32_t mask1;
    volatile uint32_t function1;
    volatile uint32_t _reserved1;

    volatile uint32_t comp2;
    volatile uint32_t mask2;
    volatile uint32_t function2;
    volatile uint32_t _reserved2;

    volatile uint32_t comp3;
    volatile uint32_t mask3;
    volatile uint32_t function3;

    volatile uint32_t _reserved3[990]; // gap from 0xE0001058-0xE0001FD0

    volatile uint32_t pid4;
    volatile uint32_t pid5;
    volatile uint32_t pid6;
    volatile uint32_t pid7;
    volatile uint32_t pid0;
    volatile uint32_t pid1;
    volatile uint32_t pid2;
    volatile uint32_t pid3;
    volatile uint32_t cid0;
    volatile uint32_t cid1;
    volatile uint32_t cid2;
    volatile uint32_t cid3;
} dwt_regdef_t;

#define DWT ((dwt_regdef_t *)DWT_BASEADDR)

// DEMCR bit positions
#define DEMCR_TRCENA 24U
#define DEMCR_MON_REQ 19U
#define DEMCR_MON_SETP 18U
#define DEMCR_MON_PEND 17U
#define DEMCR_EN 16U
#define DEMCR_VC_HARDERR 10U
#define DEMCR_VC_INTERR 9U
#define DEMCR_VC_BUSERR 8U
#define DEMCR_VC_STATERR 7U
#define DEMCR_VC_CHKERR 6U
#define DEMCR_VC_NOCPERR 5U
#define DEMCR_VC_MMERR 4U
#define DEMCR_VC_CORERESET 0U

// DWT bit positions
#define DWT_CTRL_CYCCNTENA 0


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

// Interrupt Numbers
typedef enum irq_num_t
{
    IRQ_EXTI0       = 6,
    IRQ_EXTI1       = 7,
    IRQ_EXTI2       = 8,
    IRQ_EXTI3       = 9,
    IRQ_EXTI4       = 10,
    IRQ_EXTI9_5     = 23,
    IRQ_EXTI15_10   = 40,

    IRQ_SPI1        = 35,
    IRQ_SPI2        = 36,
    IRQ_SPI3        = 51,
    IRQ_SPI4        = 84,

    IRQ_I2C1_EV     = 31,
    IRQ_I2C1_ER     = 32,
    IRQ_I2C2_EV     = 33,
    IRQ_I2C2_ER     = 34,
    IRQ_I2C3_EV     = 72,
    IRQ_I2C3_ER     = 73,

    IRQ_USART1      = 37,
    IRQ_USART2      = 38,
    IRQ_USART3      = 39,
    IRQ_UART4       = 52,
    IRQ_UART5       = 53,
    IRQ_USART6      = 71,
} irq_num_t;

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

// I2C registers
typedef struct
{
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t oar1;
    volatile uint32_t oar2;
    volatile uint32_t dr;
    volatile uint32_t sr1;
    volatile uint32_t sr2;
    volatile uint32_t ccr;
    volatile uint32_t trise;
    volatile uint32_t fltr;
} i2c_regdef_t;

// USART registers
typedef struct
{
    volatile uint32_t sr;
    volatile uint32_t dr;
    volatile uint32_t brr;
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t cr3;
    volatile uint32_t gtpr;
} usart_regdef_t;

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

// I2C
#define I2C1 ((i2c_regdef_t *)I2C1_BASEADDR)
#define I2C2 ((i2c_regdef_t *)I2C2_BASEADDR)
#define I2C3 ((i2c_regdef_t *)I2C3_BASEADDR)

// USART/UART
#define USART1 ((usart_regdef_t *)USART1_BASEADDR)
#define USART2 ((usart_regdef_t *)USART2_BASEADDR)
#define USART3 ((usart_regdef_t *)USART3_BASEADDR)
#define UART4  ((usart_regdef_t *)UART4_BASEADDR)
#define UART5  ((usart_regdef_t *)UART5_BASEADDR)
#define USART6 ((usart_regdef_t *)USART6_BASEADDR)

// ==================== RCC register bit position macros ====================
#define RCC_CFGR_SW         0
#define RCC_CFGR_SWS        2
#define RCC_CFGR_HPRE       4
#define RCC_CFGR_PPRE1      10
#define RCC_CFGR_PPRE2      13
#define RCC_CFGR_RTCPRE     16
#define RCC_CFGR_MCO1       21
#define RCC_CFGR_MCO1PRE    24
#define RCC_CFGR_MCO2PRE    27
#define RCC_CFGR_MCO2       30

#define RCC_PLLCFGR_PLLM    0
#define RCC_PLLCFGR_PLLN    6
#define RCC_PLLCFGR_PLLP    16
#define RCC_PLLCFGR_PLLSRC  22
#define RCC_PLLCFGR_PLLQ    24
#define RCC_PLLCFGR_PLLR    28

// ==================== SPI register bit position macros ====================
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

// ==================== I2C register bit position macros ====================
#define I2C_CR1_PE          0
#define I2C_CR1_SMBUS       1
#define I2C_CR1_SMB_TYPE    3
#define I2C_CR1_ENARP       4
#define I2C_CR1_ENPEC       5
#define I2C_CR1_ENGC        6
#define I2C_CR1_NOSTRETCH   7
#define I2C_CR1_START       8
#define I2C_CR1_STOP        9
#define I2C_CR1_ACK         10
#define I2C_CR1_POS         11
#define I2C_CR1_PEC         12
#define I2C_CR1_ALERT       13
#define I2C_CR1_SWRST       15

#define I2C_CR2_FREQ        0
#define I2C_CR2_ITTERREN    8
#define I2C_CR2_ITTEVTEN    9
#define I2C_CR2_ITBUFEN     10
#define I2C_CR2_DMAEN       11
#define I2C_CR2_LAST        12

#define I2C_OAR1_ADD0       0
#define I2C_OAR1_ADD1       1
#define I2C_OAR1_ADD8       8
#define I2C_OAR1_ADDMODE    15

#define I2C_OAR2_ENDUAL     0
#define I2C_OAR2_ADD2       1

#define I2C_SR1_SB           0
#define I2C_SR1_ADDR         1
#define I2C_SR1_BTF          2
#define I2C_SR1_ADD10        3
#define I2C_SR1_STOPF        4
#define I2C_SR1_RXNE         6
#define I2C_SR1_TXE          7
#define I2C_SR1_BERR         8
#define I2C_SR1_ARLO         9
#define I2C_SR1_AF           10
#define I2C_SR1_OVR          11
#define I2C_SR1_PECERR       12
#define I2C_SR1_TIMEOUT      14
#define I2C_SR1_SMBALERT     15

#define I2C_SR2_MSL          0
#define I2C_SR2_BUSY         1
#define I2C_SR2_TRA          2
#define I2C_SR2_GENCALL      4
#define I2C_SR2_SMBDEFAULT   5
#define I2C_SR2_SMBHOST      6
#define I2C_SR2_DUALF        7
#define I2C_SR2_PEC          8

#define I2C_CCR_CCR          0
#define I2C_CCR_DUTY         14
#define I2C_CCR_FS           15

#define I2C_TRISE_TRISE      0

#define I2C_FLTR_DNF         0
#define I2C_FLTR_ANOFF       4

// ==================== USART register bit position macros ====================
#define USART_SR_PE 0
#define USART_SR_FE 1
#define USART_SR_NF 2
#define USART_SR_ORE 3
#define USART_SR_IDLE 4
#define USART_SR_RXNE 5
#define USART_SR_TC  6
#define USART_SR_TXE 7
#define USART_SR_LBD 8
#define USART_SR_CRS 9

#define USART_BRR_DIV_FRACTION 0
#define USART_BRR_DIV_MANTISSA 4

#define USART_CR1_SBK 0
#define USART_CR1_RWU 1
#define USART_CR1_RE 2
#define USART_CR1_TE 3
#define USART_CR1_IDLEIE 4
#define USART_CR1_RXNEIE 5
#define USART_CR1_TCIE 6
#define USART_CR1_TXEIE 7
#define USART_CR1_PEIE 8
#define USART_CR1_PS 9
#define USART_CR1_PCE 10
#define USART_CR1_WAKE 11
#define USART_CR1_M 12
#define USART_CR1_UE 13
#define USART_CR1_OVER8 15

#define USART_CR2_ADD 0
#define USART_CR2_LBDL 5
#define USART_CR2_LBDIE 6
#define USART_CR2_LBCL 8
#define USART_CR2_CPHA 9
#define USART_CR2_CPOL 10
#define USART_CR2_CLKEN 11
#define USART_CR2_STOP  12
#define USART_CR2_LINEN 14

#define USART_CR3_EIE 0
#define USART_CR3_IREN 1
#define USART_CR3_IRLP 2
#define USART_CR3_HDSEL 3
#define USART_CR3_NACK 4
#define USART_CR3_SCEN 5
#define USART_CR3_DMAR 6
#define USART_CR3_DMAT 7
#define USART_CR3_RTSE 8
#define USART_CR3_CTSE 9
#define USART_CR3_ONEBIT 11

#define USART_GTPR_PSC 0
#define USART_GTPR_GT 8


// MISC
#define GPIO_BASEADDR_TO_PORT(x) ((x == GPIOA) ? 0 : \
                                  (x == GPIOB) ? 1 : \
                                  (x == GPIOC) ? 2 : \
                                  (x == GPIOD) ? 3 : \
                                  (x == GPIOE) ? 4 : \
                                  (x == GPIOF) ? 5 : \
                                  (x == GPIOG) ? 6 : \
                                  (x == GPIOH) ? 7 : 0)

#include "irq.h"
#include "rcc.h"
#include "gpio.h"
#include "spi.h"
#include "i2c.h"
#include "usart.h"
#include "dwt.h"

#endif
