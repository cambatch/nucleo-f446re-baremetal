#include "spi.h"

#include <stddef.h>

void spi_init(spi_handle_t *handle)
{
    // Ensure SPIx is disabled before modifying values
    spi_peripheral_control(handle->spix, DISABLE);

    // Enable peripheral clock
    spi_clock_control(handle->spix, ENABLE);

    // Configure baud rate
    handle->spix->cr1 &= ~(7U << SPI_CR1_BR);
    handle->spix->cr1 |= ((uint8_t)handle->config.baud << SPI_CR1_BR);

    // Configure cpol and cpha
    handle->spix->cr1 &= ~(1U << SPI_CR1_CPOL);
    handle->spix->cr1 &= ~(1U << SPI_CR1_CPHA);
    handle->spix->cr1 |= ((uint8_t)handle->config.cpol << SPI_CR1_CPOL);
    handle->spix->cr1 |= ((uint8_t)handle->config.cpha << SPI_CR1_CPHA);

    // Configure bus mode
    if(handle->config.bus_config == SPI_BUS_FULL_DUPLEX)
    {
        // Full-duplex
        handle->spix->cr1 &= ~(1U << SPI_CR1_BIDIMODE);
    }
    else if(handle->config.bus_config == SPI_BUS_HALF_DUPLEX)
    {
        // Half-duplex
        handle->spix->cr1 |= (1U << SPI_CR1_BIDIMODE);
    }
    else if(handle->config.bus_config == SPI_BUS_RX_ONLY)
    {
        // Simplex RX only
        handle->spix->cr1 &= ~(1U << SPI_CR1_BIDIMODE);
        handle->spix->cr1 |= (1U << SPI_CR1_RXONLY);
    }

    // Configure LSBFIRST
    handle->spix->cr1 &= ~(1U << SPI_CR1_LSBFIRST);
    handle->spix->cr1 |= ((uint8_t)handle->config.ff << SPI_CR1_LSBFIRST);

    // Configure SSM
    handle->spix->cr1 &= ~(1U << SPI_CR1_SSM);
    handle->spix->cr1 &= ~(1U << SPI_CR1_SSI);
    if(handle->config.ssm == SPI_SSM_SOFTWARE)
    {
        handle->spix->cr1 |= ((uint8_t)handle->config.ssm << SPI_CR1_SSM);
        handle->spix->cr1 |= (1U << SPI_CR1_SSI);
    }

    // Configure MSTR
    handle->spix->cr1 &= ~(1U << SPI_CR1_MSTR);
    handle->spix->cr1 |= ((uint8_t)handle->config.device_mode << SPI_CR1_MSTR);

    // Configure DFF
    handle->spix->cr1 &= ~(1U << SPI_CR1_DFF);
    handle->spix->cr1 |= ((uint8_t)handle->config.df << SPI_CR1_DFF);
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

bool spi_flag_status(spi_regdef_t *spix, uint32_t flag)
{
    return (spix->sr & flag) != 0;
}

void spi_send(spi_regdef_t *spix, const uint8_t *tx_buffer, uint32_t bytes)
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

void spi_receive(spi_regdef_t *spix, uint8_t *rx_buffer, uint32_t bytes)
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

spi_state_t spi_transfer_it(spi_handle_t *handle, const uint8_t *tx_buffer, uint8_t *rx_buffer, const uint32_t bytes)
{
    spi_state_t state = handle->state;

    if(state != SPI_STATE_BUSY)
    {
        handle->tx_buffer = tx_buffer;
        handle->tx_bytes = bytes;
        handle->rx_buffer = rx_buffer;
        handle->rx_bytes = bytes;
        handle->state = SPI_STATE_BUSY;

        if(rx_buffer == NULL) handle->op = SPI_OP_TX;
        else if(tx_buffer == NULL) handle->op = SPI_OP_RX;
        else handle->op = SPI_OP_TXRX;

        spi_peripheral_control(handle->spix, ENABLE);

        if(handle->tx_bytes > 0) handle->spix->cr2 |= (1U << SPI_CR2_TXEIE);
        if(handle->rx_buffer) handle->spix->cr2 |= (1U << SPI_CR2_RXNEIE);
        handle->spix->cr2 |= (1U << SPI_CR2_ERRIE);

        return SPI_STATE_READY;
    }

    return state;
}

void spi_irq_handler(spi_handle_t *handle)
{
    // TXE
    if(spi_flag_status(handle->spix, SPI_FLAG_TXE) && handle->tx_bytes > 0)
    {
        if(handle->tx_buffer)
        {
            if(handle->config.df == SPI_DF_16BIT)
            {
                handle->spix->dr = *((uint16_t *)handle->tx_buffer);
                handle->tx_buffer += 2;
                handle->tx_bytes -= 2;
            }
            else
            {
                handle->spix->dr = *handle->tx_buffer;
                ++handle->tx_buffer;
                --handle->tx_bytes;
            }
        }
        else // Dummy writes for non full-duplex communication
        {
            volatile uint16_t data = 0xFFFFU;
            if(handle->config.df == SPI_DF_16BIT)
            {
               handle->spix->dr = data;
                handle->tx_bytes -= 2;
            }
            else
            {
                handle->spix->dr = (uint8_t)data;
                --handle->tx_bytes;
            }
        }

        if(handle->tx_bytes == 0)
        {
            handle->spix->cr2 &= ~(1U << SPI_CR2_TXEIE);
        }
    }

    // RXNE
    if(spi_flag_status(handle->spix, SPI_FLAG_RXNE) && handle->rx_bytes > 0)
    {
        if(handle->rx_buffer)
        {
            if(handle->config.df == SPI_DF_16BIT)
            {
                *((uint16_t *)handle->rx_buffer) = (uint16_t)handle->spix->dr;
                handle->rx_buffer += 2;
                handle->rx_bytes -= 2;
            }
            else
            {
                *handle->rx_buffer = (uint8_t)handle->spix->dr;
                ++handle->rx_buffer;
                --handle->rx_bytes;
            }
        }
        else // Dummy reads for non full-duplex communication
        {
            volatile uint16_t dummy;
            if(handle->config.df == SPI_DF_16BIT)
            {
                dummy = (uint16_t)handle->spix->dr;
                (void)dummy;
                handle->rx_bytes -= 2;
            }
            else
            {
                dummy = (uint8_t)handle->spix->dr;
                (void)dummy;
                --handle->rx_bytes;
            }
        }

        if(handle->rx_bytes == 0)
        {
            handle->spix->cr2 &= ~(1U << SPI_CR2_RXNEIE);
        }
    }

    if(handle->tx_bytes == 0  && handle->rx_bytes == 0 && handle->state == SPI_STATE_BUSY)
    {
        // Disable interrupts
        handle->spix->cr2 &= ~(1U << SPI_CR2_ERRIE);

        if(handle->event_callback)
        {
            switch(handle->op)
            {
                case SPI_OP_TX: handle->event_callback(handle, SPI_EVENT_TX_DONE); break;
                case SPI_OP_RX: handle->event_callback(handle, SPI_EVENT_RX_DONE); break;
                case SPI_OP_TXRX: handle->event_callback(handle, SPI_EVENT_TXRX_DONE); break;
                default: break;
            }
        }

        handle->op = SPI_OP_NONE;
        handle->state = SPI_STATE_READY;
    }

    // Check for overrun error
    if(spi_flag_status(handle->spix, SPI_FLAG_OVR))
    {
        // Clear flag
        volatile uint32_t dummy = handle->spix->dr;
        dummy = handle->spix->sr;
        (void)dummy;

        if(handle->event_callback)
            handle->event_callback(handle, SPI_EVENT_OVR_ERR);
    }

    // Check for modf error
    if(spi_flag_status(handle->spix, SPI_FLAG_MODF))
    {
        // Clear flag
        volatile uint32_t dummy = handle->spix->sr;
        handle->spix->cr1 |= 0;
        (void)dummy;

        if(handle->event_callback)
            handle->event_callback(handle, SPI_EVENT_MODF_ERR);
    }
}
