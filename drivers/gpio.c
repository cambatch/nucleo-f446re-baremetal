#include "gpio.h"

void gpio_init(gpio_handle_t *handle)
{
    // Enable GPIO clock
    gpio_clock_control(handle->gpiox, ENABLE);

    // TODO: Add support for interrupts
    // Configure mode
    handle->gpiox->moder |= (handle->config.mode << (2 * handle->config.pin_num));

    // Configure alternate function if applicable
    if(handle->config.mode == GPIO_MODE_ALTFN)
    {
        uint8_t bit_pos = handle->config.pin_num % 8;

        if(handle->config.pin_num < 8)
        {
            // Alternate function low register (pins 0-7)
            handle->gpiox->afrl |= (handle->config.altfn << (4 * bit_pos));
        }
        else
        {
            // Alternate function high register (pins 8-15)
            handle->gpiox->afrh |= (handle->config.altfn << (4 * bit_pos));
        }
    }

    // Configure output type
    handle->gpiox->otyper |= (handle->config.otype << handle->config.pin_num);

    // Configure output speed
    handle->gpiox->ospeedr |= (handle->config.speed << (2 * handle->config.pin_num));

    // Configure pull-up/pull-down
    handle->gpiox->pupdr |= (handle->config.pupd << (2 * handle->config.pin_num));
}

void gpio_clock_control(gpio_regdef_t *gpiox, bool status)
{
    if(status == ENABLE)
    {
        if(gpiox == GPIOA) GPIOA_PCLK_EN();
        else if(gpiox == GPIOB) GPIOB_PCLK_EN();
        else if(gpiox == GPIOC) GPIOC_PCLK_EN();
        else if(gpiox == GPIOD) GPIOD_PCLK_EN();
        else if(gpiox == GPIOE) GPIOE_PCLK_EN();
        else if(gpiox == GPIOF) GPIOF_PCLK_EN();
        else if(gpiox == GPIOG) GPIOG_PCLK_EN();
        else if(gpiox == GPIOH) GPIOH_PCLK_EN();
    }
    else
    {
        if(gpiox == GPIOA) GPIOA_PCLK_DI();
        else if(gpiox == GPIOB) GPIOB_PCLK_DI();
        else if(gpiox == GPIOC) GPIOC_PCLK_DI();
        else if(gpiox == GPIOD) GPIOD_PCLK_DI();
        else if(gpiox == GPIOE) GPIOE_PCLK_DI();
        else if(gpiox == GPIOF) GPIOF_PCLK_DI();
        else if(gpiox == GPIOG) GPIOG_PCLK_DI();
        else if(gpiox == GPIOH) GPIOH_PCLK_DI();
    }
}

uint8_t gpio_read_pin(gpio_regdef_t *gpiox, uint8_t pin)
{
    return (uint8_t)((gpiox->idr >> pin) & 0x1);
}

void gpio_write_pin(gpio_regdef_t *gpiox, uint8_t pin, uint8_t value)
{
    if(value)
        gpiox->bsrr = (1U << pin); // set
    else
        gpiox->bsrr = (1U << (pin + 16U)); // reset
}

void gpio_toggle_pin(gpio_regdef_t *gpiox, uint8_t pin)
{
    gpiox->odr ^= (1U << pin);
}

void gpio_write_port(gpio_regdef_t *gpiox, uint16_t value)
{
    uint32_t set_bits = (uint32_t)(value);
    uint32_t reset_bits = (uint32_t)(~value) << 16;
    gpiox->bsrr = (set_bits | reset_bits);
}

void gpio_toggle_port(gpio_regdef_t *gpiox, uint16_t mask)
{
    gpiox->odr ^= mask;
}
