#ifndef DRIVER_SPI_H
#define DRIVER_SPI_H

#include "f446re.h"

#include <stdbool.h>

// Device mode
typedef enum spi_mode_t
{
    SPI_MODE_SLAVE  = 0,
    SPI_MODE_MASTER = 1
} spi_mode_t;

// Bus mode
typedef enum spi_bus_t
{
    SPI_BUS_FULL_DUPLEX   = 0,
    SPI_BUS_HALF_DUPLEX   = 1,
    SPI_BUS_RX_ONLY       = 2
} spi_bus_t;

// Software slave management
typedef enum spi_ssm_t
{
    SPI_SSM_HARDWARE = 0, // Hardware management
    SPI_SSM_SOFTWARE = 1  // Software management
} spi_ssm_t;

// Data format
typedef enum spi_df_t
{
    SPI_DF_8BIT   = 0,
    SPI_DF_16BIT  = 1,
} spi_df_t;

// Baud rate
typedef enum spi_baud_t
{
    SPI_BAUD_DIV2   = 0,
    SPI_BAUD_DIV4   = 1,
    SPI_BAUD_DIV8   = 2,
    SPI_BAUD_DIV16  = 3,
    SPI_BAUD_DIV32  = 4,
    SPI_BAUD_DIV64  = 5,
    SPI_BAUD_DIV128 = 6,
    SPI_BAUD_DIV256 = 7
} spi_baud_t;

// Clock polarity
typedef enum spi_cpol_t
{
    SPI_CPOL_LOW    = 0,
    SPI_CPOL_HIGH   = 1
} spi_cpol_t;

// Clock phase
typedef enum spi_cpha_t
{
    SPI_CPHA_1EDGE  = 0,
    SPI_CPHA_2EDGE = 1
} spi_cpha_t;

// Frame format
typedef enum spi_ff_t
{
    SPI_FF_MSB_FIRST = 0,
    SPI_FF_LSB_FIRST = 1
} spi_ff_t;

// Configuration struct
typedef struct
{
    spi_mode_t device_mode;
    spi_bus_t bus_config;
    spi_baud_t baud;
    spi_cpol_t cpol;
    spi_cpha_t cpha;
    spi_ssm_t ssm;
    spi_df_t df;
    spi_ff_t ff;
} spi_config_t;

// SPI peripheral handle struct
typedef struct
{
    spi_regdef_t *spix;
    spi_config_t config;
} spi_handle_t;

// SPI Flags
#define SPI_FLAG_RESET   0
#define SPI_FLAG_SET     1
#define SPI_FLAG_RXNE    (1U << SPI_SR_RXNE)
#define SPI_FLAG_TXE     (1U << SPI_SR_TXE)
#define SPI_FLAG_BUSY    (1U << SPI_SR_BSY)

/**
 * @brief Initializes the SPI peripheral according to the parameters supplied in the handle.
 *
 * This function performs a full initialization of the SPI peripheral: 
 * - Disables the peripheral before configuration.
 * - Enables peripheral clock.
 * - Configures baud rate, clock polarity, clock phase.
 * - Sets SPI bus mode; Full-duplex, half-duplex, simplex RX-only.
 * - Sets frame format (MSB/LSB first).
 * - Configures software slave management (SSM/SSI).
 * - Configures master/slave mode.
 * - Sets data frame format (8-bit or 16-bit)
 *
 * @param handle Pointer to spi_handle_t that contains the configuration for the SPI peripheral
 * 
 * @note The SPI peripheral is not enabled by this function. Call spi_peripheral_control() with ENABLE
 *       to start communication after configuration.
 */
void spi_init(spi_handle_t *handle);

/**
 * @brief Enables or disables the SPI peripheal clock.
 * 
 * @param spix Pointer to the SPI peripheral base address.
 * @param status ENABLE to turn on clock, DISABLE to turn it off.
 */
void spi_clock_control(spi_regdef_t *spix, bool status);

/**
 * @brief Enables or disables the SPI peripheral.
 * 
 * @param spix Pointer to the SPI peripheral base address.
 * @param status ENABLE to enable SPI, DISABLE to disable it.
 */
void spi_peripheral_control(spi_regdef_t *spix, bool status);

/**
 * @brief Enables or disables the Slave Select Output Enable (SSOE) feature.
 * 
 * @param spix Pointer to the SPI peripheal base address.
 * @param status ENABLE to enable SSOE, DISABLE to disable it.
 */
void spi_ssoe_control(spi_regdef_t *spix, bool status);

/**
 * @brief Returns the current status of a specified SPI flag.
 * 
 * @param spix Pointer to the SPI peripheral base address.
 * @param flag The flag to check (TXE, RXNE, etc.).
 * @return SPI_FLAG_SET if the flag is set, SPI_FLAG_RESET otherwise.
 */
uint8_t spi_flag_status(spi_regdef_t *spix, uint8_t flag);

/**
 * @brief Sends data over SPI (Polling)
 * 
 * Blocks until the SPI is not busy, then sends the specified number of bytes using polling.
 * Supports 8-bit and 16-bit formats.
 * 
 * @param spix Pointer to the SPI peripheral base address.
 * @param tx_buffer Pointer to the data buffer to send.
 * @param bytes Number of bytes to send.
 */
void spi_send(spi_regdef_t *spix, const uint8_t *tx_buffer, uint32_t bytes);

/**
 * @brief Receives data over SPI (polling)
 * 
 * Blocks until the SPI is not bust, then reads the specified number of bytes using polling.
 * Supports 8-bit and 16-bit formats.
 * 
 * @param spix Pointer to the SPI peripheral base address.
 * @param rx_buffer Pointer to the buffer where data will be stored.
 * @param bytes Number of bytes to receive
 */
void spi_receive(spi_regdef_t *spix, uint8_t *rx_buffer, uint32_t bytes);

#endif
