#include "spi.h"

void spi_init(spi_handle_t *handle)
{
    // Ensure SPIx is disabled before modifying values
    spi_peripheral_control(handle->spix, DISABLE);

    // Enable peripheral clock
    spi_clock_control(handle->spix, ENABLE);

    // Configure baud rate
    handle->spix->cr1 &= ~(7U << SPI_CR1_BR);
    handle->spix->cr1 |= (handle->config.clk_speed << SPI_CR1_BR);

    // Configure cpol and cpha
    handle->spix->cr1 &= ~(1U << SPI_CR1_CPOL);
    handle->spix->cr1 &= ~(1U << SPI_CR1_CPHA);
    handle->spix->cr1 |= (handle->config.cpol << SPI_CR1_CPOL);
    handle->spix->cr1 |= (handle->config.cpha << SPI_CR1_CPHA);

    // Configure bus mode
    if(handle->config.bus_config == SPI_BUS_MODE_FD)
    {
        // Full-duplex
        handle->spix->cr1 &= ~(1U << SPI_CR1_BIDIMODE);
    }
    else if(handle->config.bus_config == SPI_BUS_MODE_HD)
    {
        // Half-duplex
        handle->spix->cr1 |= (1U << SPI_CR1_BIDIMODE);
    }
    else if(handle->config.bus_config == SPI_BUS_MODE_RXONLY)
    {
        // Simplex RX only
        handle->spix->cr1 &= ~(1U << SPI_CR1_BIDIMODE);
        handle->spix->cr1 |= (1U << SPI_CR1_RXONLY);
    }

    // Configure LSBFIRST
    handle->spix->cr1 &= ~(1U << SPI_CR1_LSBFIRST);
    handle->spix->cr1 |= (handle->config.frame_format << SPI_CR1_LSBFIRST);

    // Configure SSM
    handle->spix->cr1 &= ~(1U << SPI_CR1_SSM);
    handle->spix->cr1 &= ~(1U << SPI_CR1_SSI);
    if(handle->config.ssm == SPI_SS_SOFTWARE)
    {
        handle->spix->cr1 |= (handle->config.ssm << SPI_CR1_SSM);
        handle->spix->cr1 |= (1U << SPI_CR1_SSI);
    }

    // Configure MSTR
    handle->spix->cr1 &= ~(1U << SPI_CR1_MSTR);
    handle->spix->cr1 |= (handle->config.device_mode << SPI_CR1_MSTR);

    // Configure DFF
    handle->spix->cr1 &= ~(1U << SPI_CR1_DFF);
    handle->spix->cr1 |= (handle->config.data_format << SPI_CR1_DFF);
}

void spi_clock_control(spi_regdef_t *spix, bool status)
{
    if(status)
    {
        if(spix == SPI1) SPI1_PCLK_EN();
        else if(spix == SPI2) SPI2_PCLK_EN();
        else if(spix == SPI3) SPI3_PCLK_EN();
        else if(spix == SPI4) SPI4_PCLK_EN();
    }
    else
    {
        if(spix == SPI1) SPI1_PCLK_DI();
        else if(spix == SPI2) SPI2_PCLK_DI();
        else if(spix == SPI3) SPI3_PCLK_DI();
        else if(spix == SPI4) SPI4_PCLK_DI();
    }
}

void spi_peripheral_control(spi_regdef_t *spix, bool status)
{
    if(status)
    {
        spix->cr1 |= (1U << SPI_CR1_SPE);
    }
    else
    {
        spix->cr1 &= ~(1U << SPI_CR1_SPE);
    }
}

void spi_ssoe_control(spi_regdef_t *spix, bool status)
{
    if(status)
    {
        spix->cr2 |= (1U << SPI_CR2_SSOE);
    }
    else
    {
        spix->cr2 &= ~(1U << SPI_CR2_SSOE);
    }
}

uint8_t spi_flag_status(spi_regdef_t *spix, uint8_t flag)
{
    if(spix->sr & flag)
    {
        return SPI_FLAG_SET;
    }

    return SPI_FLAG_RESET;
}

void spi_data_send(spi_regdef_t *spix, uint8_t *tx_buffer, uint32_t bytes)
{
    while(spi_flag_status(spix, SPI_FLAG_BUSY) == SPI_FLAG_SET);

    while(bytes > 0)
    {
        while(spi_flag_status(spix, SPI_FLAG_TXE) == SPI_FLAG_RESET);

        if(spix->cr1 & (1U << SPI_CR1_DFF))
        {
            // 16-bit transfer
           spix->dr = *((uint16_t *)tx_buffer);
           bytes -= 2;
           tx_buffer += 2;
        }
        else
        {
            // 8-bit transer
            spix->dr = *tx_buffer;
            --bytes;
            ++tx_buffer;
        }
    }
}

void spi_data_receive(spi_regdef_t *spix, uint8_t *rx_buffer, uint32_t bytes)
{
    while(spi_flag_status(spix, SPI_FLAG_BUSY) == SPI_FLAG_SET);

    while(bytes > 0)
    {
        while(spi_flag_status(spix, SPI_FLAG_RXNE) == SPI_FLAG_RESET);

        if(spix->cr1 & (1U << SPI_CR1_DFF))
        {
            // 16-bit
            *((uint16_t *)rx_buffer) = (uint16_t)spix->dr;
            bytes -= 2;
            rx_buffer += 2;
        }
        else
        {
            // 8-bit
            *rx_buffer = (uint8_t)spix->dr;
            --bytes;
            ++rx_buffer;
        }
    }
}
