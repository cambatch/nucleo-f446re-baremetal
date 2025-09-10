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

    handle->usartx->cr1 &= ~(1U << USART_CR1_UE);

    // ==================== CR1 ====================
    // Clear bits
    handle->usartx->cr1 &= ~((1U << USART_CR1_M) | (1U << USART_CR1_PCE) | (1U << USART_CR1_PS)
                           | (1U << USART_CR1_TE) | (1U << USART_CR1_RE));

    // Word length
    if(handle->config.word_len == USART_WORDLEN_9) handle->usartx->cr1 |= (1U << USART_CR1_M);

    // Mode
    if(handle->config.mode != USART_MODE_RX_ONLY) handle->usartx->cr1 |= (1U << USART_CR1_TE);
    if(handle->config.mode != USART_MODE_TX_ONLY) handle->usartx->cr1 |= (1U << USART_CR1_RE);

    // Parity
    if(handle->config.parity == USART_PARITY_EN_EVEN)
        handle->usartx->cr1 |= (1U << USART_CR1_PCE);
    else if(handle->config.parity == USART_PARITY_EN_ODD)
        handle->usartx->cr1 |= ((1U << USART_CR1_PCE) | (1U << USART_CR1_PS));

    // ==================== CR2 ====================
    // Clear bits
    handle->usartx->cr2 &= ~(3U << USART_CR2_STOP);
    handle->usartx->cr2 |= ((uint32_t)handle->config.stop_bits << USART_CR2_STOP);

    // ==================== CR3 ====================
    // Clear bits
    handle->usartx->cr3 &= ~((1U << USART_CR3_CTSE) | (1U << USART_CR3_RTSE));

    // HW flow control
    if(handle->config.hw_control != USART_HWFLOW_NONE)
    {
        if(handle->config.hw_control != USART_HWFLOW_RTS) handle->usartx->cr3 |= (1U << USART_CR3_CTSE);
        if(handle->config.hw_control != USART_HWFLOW_CTS) handle->usartx->cr3 |= (1U << USART_CR3_RTSE);
    }

    usart_set_baudrate(handle->usartx, handle->config.baud);
}

void usart_set_baudrate(usart_regdef_t *usartx, uint32_t baud_rate)
{
    uint32_t pclk = (usartx == USART1 || usartx == USART6) ? rcc_apb2_clock_hz() : rcc_apb1_clock_hz();
    uint32_t over8 = (usartx->cr1 & (1U << USART_CR1_OVER8)) ? 1U : 0U;
    uint32_t usart_div = over8 ? ((25U * pclk) / (2U * baud_rate)) : ((25U * pclk) / (4U * baud_rate));
    uint32_t mantissa = usart_div / 100;
    uint32_t fraction = (usart_div - (mantissa * 100));

    if(over8) fraction = (((fraction * 8) + 50) / 100) & ((uint8_t)0x07);
    else      fraction = (((fraction * 16) + 50) / 100) & ((uint8_t)0x0F);

    usartx->brr = ((mantissa << USART_BRR_DIV_MANTISSA) | (fraction << USART_BRR_DIV_FRACTION));
    (void)0;
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
