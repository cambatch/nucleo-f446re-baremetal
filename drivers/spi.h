#ifndef DRIVER_SPI_H
#define DRIVER_SPI_H

#include "f446re.h"

#include <stdbool.h>

typedef struct
{
    uint8_t device_mode;
    uint8_t bus_config;
    uint8_t clk_speed;
    uint8_t cpol;
    uint8_t cpha;
    uint8_t ssm;
    uint8_t data_format;
    uint8_t frame_format;
} spi_config_t;

typedef struct
{
    spi_regdef_t *spix;
    spi_config_t config;
} spi_handle_t;

// SPI device mode
#define SPI_DEVICE_MODE_SLAVE  0
#define SPI_DEVICE_MODE_MASTER 1

// SPI bus config
#define SPI_BUS_MODE_FD         0   // Full-duplex
#define SPI_BUS_MODE_HD         1   // Half-duplex
#define SPI_BUS_MODE_RXONLY     2

// SPI slave select management
#define SPI_SS_HARDWARE 0   // Hardware management
#define SPI_SS_SOFTWARE 1   // Software mangaement

// SPI data format
#define SPI_FORMAT_8BITS  0
#define SPI_FORMAT_16BITS 1

// SPI baud rate
#define SPI_BAUD_DIV2       0
#define SPI_BAUD_DIV4       1
#define SPI_BAUD_DIV8       2
#define SPI_BAUD_DIV16      3
#define SPI_BAUD_DIV32      4
#define SPI_BAUD_DIV64      5
#define SPI_BAUD_DIV128     6
#define SPI_BAUD_DIV256     7

// SPI CPOL
#define SPI_CPOL_LOW  0
#define SPI_CPOL_HIGH 1

// SPI CPHA
#define SPI_CPHA_FIRST  0
#define SPI_CPHA_SECOND 1

// SPI Frame Format
#define SPI_FF_MSB_FIRST 0
#define SPI_FF_LSB_FIRST 1

// SPI Flags
#define SPI_FLAG_RESET   0
#define SPI_FLAG_SET     1
#define SPI_FLAG_RXNE    (1U << SPI_SR_RXNE)
#define SPI_FLAG_TXE     (1U << SPI_SR_TXE)
#define SPI_FLAG_BUSY    (1U << SPI_SR_BSY)


void spi_init(spi_handle_t *handle);

void spi_clock_control(spi_regdef_t *spix, bool status);
void spi_peripheral_control(spi_regdef_t *spix, bool status);
void spi_ssoe_control(spi_regdef_t *spix, bool status);

uint8_t spi_flag_status(spi_regdef_t *spix, uint8_t flag);

void spi_data_send(spi_regdef_t *spix, uint8_t *tx_buffer, uint32_t bytes);
void spi_data_receive(spi_regdef_t *spix, uint8_t *rx_buffer, uint32_t bytes);

#endif
