#include "usart.h"



void usart_clock_control(usart_regdef_t *usartx, bool enable)
{
    if(enable)
    {
        if(usartx == USART1) USART1_PCLK_EN();
        else if(usartx == USART2) USART2_PCLK_EN();
        else if(usartx == USART3) USART3_PCLK_EN();
        else if(usartx == UART4)  UART4_PCLK_EN();
        else if(usartx == UART5)  UART5_PCLK_EN();
        else if(usartx == USART6) USART6_PCLK_EN();
    }
    else
    {
        if(usartx == USART1) USART1_PCLK_DI();
        else if(usartx == USART2) USART2_PCLK_DI();
        else if(usartx == USART3) USART3_PCLK_DI();
        else if(usartx == UART4)  UART4_PCLK_DI();
        else if(usartx == UART5)  UART5_PCLK_DI();
        else if(usartx == USART6) USART6_PCLK_DI();
    }
}

void usart_peripheral_control(usart_regdef_t *usartx, bool enable)
{
    if(enable) usartx->cr1 |= (1U << USART_CR1_UE);
    else       usartx->cr1 &= ~(1U << USART_CR1_UE);
}

uint8_t usart_flag_status(usart_regdef_t *usartx, uint32_t flag)
{
    return (usartx->sr & flag) != 0;
}

void usart_init(usart_handle_t *handle)
{
    usart_clock_control(handle->usartx, ENABLE);

    if(handle->config.mode == USART_MODE_TX_ONLY)
    {
        handle->usartx->cr1 &= ~(1U << USART_CR1_RE);
        handle->usartx->cr1 |= (1U << USART_CR1_TE);
    }
    else if(handle->config.mode == USART_MODE_RX_ONLY)
    {
        handle->usartx->cr1 &= ~(1U << USART_CR1_TE);
        handle->usartx->cr1 |= (1U << USART_CR1_RE);
    }
    else
    {
        handle->usartx->cr1 |= (1U << USART_CR1_TE);
        handle->usartx->cr1 |= (1U << USART_CR1_RE);
    }

    handle->usartx->cr1 &= ~(1U << USART_CR1_M);
    handle->usartx->cr1 |= (1U << USART_CR1_M);

    if(handle->config.parity == USART_PARITY_EN_EVEN)
    {
        // even parity is on by default when parity is enabled
        handle->usartx->cr1 |= (1U << USART_CR1_PCE);
    }
    else if(handle->config.parity == USART_PARITY_EN_ODD)
    {
        handle->usartx->cr1 |= (1U << USART_CR1_PCE);
        handle->usartx->cr1 |= (1U << USART_CR1_PS);
    }

    // configure cr2
    handle->usartx->cr2 |= handle->config.stop_bits << USART_CR2_STOP;

    // configure cr3
    if(handle->config.hw_control == USART_HWFLOW_CTS)
    {
        handle->usartx->cr3 |= (1U << USART_CR3_CTSE);
    }
    else if(handle->config.hw_control == USART_HWFLOW_RTS)
    {
        handle->usartx->cr3 |= (1U << USART_CR3_RTSE);
    }
    else
    {
        handle->usartx->cr3 |= (1U << USART_CR3_CTSE);
        handle->usartx->cr3 |= (1U << USART_CR3_RTSE);
    }

    usart_set_baudrate(handle->usartx, handle->config.baud);
}

void usart_set_baudrate(usart_regdef_t *usartx, uint32_t baud_rate)
{
    uint32_t clk;
    uint32_t usart_div;
    uint32_t mpart;
    uint32_t fpart;

    if(usartx == USART1 || usartx == USART6) clk = rcc_apb2_clock_hz();
    else                                     clk = rcc_apb1_clock_hz();

    if(usartx->cr1 & (1U << USART_CR1_OVER8)) usart_div = ((25 * clk) / (2 * baud_rate));
    else                                      usart_div = ((25 * clk) / (4 * baud_rate));

    mpart = usart_div / 100;

    usartx->brr |= mpart << USART_BRR_DIV_MANTISSA;

    fpart = (usart_div - (mpart * 100));

    if(usartx->cr1 & (1U << USART_CR1_OVER8)) fpart = (((fpart * 8) + 50) / 100) & ((uint8_t)0x07);
    else                                      fpart = (((fpart * 16) + 50) / 100) & ((uint8_t)0x07);

    usartx->brr |= fpart << USART_BRR_DIV_FRACTION;
}

void usart_send(usart_handle_t *handle, uint8_t *buffer, uint32_t len)
{
    uint16_t *data;

    for(uint32_t i = 0; i < len; ++i)
    {
        while(!usart_flag_status(handle->usartx, USART_FLAG_TXE));

        if(handle->config.word_len == USART_WORDLEN_9)
        {
            data = (uint16_t *)buffer;
            handle->usartx->dr = (*data & (uint16_t)0x01FFU);

            if(handle->config.parity == USART_PARITY_DISABLE) buffer += 2;
            else                                              ++buffer;
        }
        else
        {
            handle->usartx->dr = (*buffer & (uint8_t)0xFFU);
            ++buffer;
        }
    }

    while(!usart_flag_status(handle->usartx, USART_FLAG_TC));
}

void usart_receive(usart_handle_t *handle, uint8_t *buffer, uint32_t len)
{
    for(uint32_t i = 0; i < len; ++i)
    {
        while(!usart_flag_status(handle->usartx, USART_FLAG_RXNE));

        if(handle->config.word_len == USART_WORDLEN_9)
        {
            if(handle->config.parity == USART_PARITY_DISABLE)
            {
                *((uint16_t*)buffer) = (handle->usartx->dr & (uint16_t)0x1FFU);
                buffer += 2;
            }
            else
            {
                *buffer = (handle->usartx->dr & (uint8_t)0xFF);
                ++buffer;
            }
        }
        else
        {
            if(handle->config.parity == USART_PARITY_DISABLE) *buffer = handle->usartx->dr & 0xFFU;
            else                                              *buffer = handle->usartx->dr & 0x7FU;

            ++buffer;
        }
    }
}
