#include "gpio.h"

void gpio_init(gpio_handle_t *handle)
{
    // Enable GPIO clock
    gpio_clock_control(handle->gpiox, ENABLE);

    // Configure mode
    if(handle->config.mode <= GPIO_MODE_ALTFN)
    {
        handle->gpiox->moder &= ~(0x3U << (2 * handle->config.pin_num));
        handle->gpiox->moder |= ((uint8_t)handle->config.mode << (2 * handle->config.pin_num));
    }
    else
    {
        // Interrupt information found in ARM DUI 0553A cortex-m and STM RM0390
        // Set pin as input
        handle->gpiox->moder &= ~(0x3U << (2 * handle->config.pin_num));

        // Enable SYSCFG clock
        SYSCFG_PCLK_EN();

        // Set EXTI line to GPIO port
        uint8_t cr_index = handle->config.pin_num / 4;
        uint8_t cr_shift = (handle->config.pin_num % 4) * 4;
        uint8_t portcode = GPIO_BASEADDR_TO_PORT(handle->gpiox);
        SYSCFG->cr[cr_index] &= ~(0xFU << cr_shift);
        SYSCFG->cr[cr_index] |= (portcode << cr_shift);

        // Unmask IMR
        EXTI->imr |= (1U << handle->config.pin_num);

        // Configure edge trigger
        if(handle->config.mode == GPIO_MODE_IT_RT)
        {
            // Rising edge trigger
            EXTI->ftsr &= ~(1U << handle->config.pin_num);
            EXTI->rtsr |= (1U << handle->config.pin_num);
        }
        else if(handle->config.mode == GPIO_MODE_IT_FT)
        {
            // Falling edge trigger
            EXTI->rtsr &= ~(1U << handle->config.pin_num);
            EXTI->ftsr |= (1U << handle->config.pin_num);
        }
        else
        {
            // Rising and falling edge trigger
            EXTI->rtsr |= (1U << handle->config.pin_num);
            EXTI->ftsr |= (1U << handle->config.pin_num);
        }
    }

    // Configure alternate function if applicable
    if(handle->config.mode == GPIO_MODE_ALTFN)
    {
        uint8_t bit_pos = handle->config.pin_num % 8;

        if(handle->config.pin_num < 8)
        {
            // Alternate function low register (pins 0-7)
            handle->gpiox->afrl &= ~(0xFU << (4 * bit_pos));
            handle->gpiox->afrl |= (handle->config.altfn << (4 * bit_pos));
        }
        else
        {
            // Alternate function high register (pins 8-15)
            handle->gpiox->afrh &= ~(0xFU << (4 * bit_pos));
            handle->gpiox->afrh |= (handle->config.altfn << (4 * bit_pos));
        }
    }

    // Configure output type
    handle->gpiox->otyper &= ~(0x1U << handle->config.pin_num);
    handle->gpiox->otyper |= ((uint8_t)handle->config.otype << handle->config.pin_num);

    // Configure output speed
    handle->gpiox->ospeedr &= ~(0x3U << (2 * handle->config.pin_num));
    handle->gpiox->ospeedr |= ((uint8_t)handle->config.speed << (2 * handle->config.pin_num));

    // Configure pull-up/pull-down
    handle->gpiox->pupdr &= ~(0x3U << (2 * handle->config.pin_num));
    handle->gpiox->pupdr |= ((uint8_t)handle->config.pupd << (2 * handle->config.pin_num));
}

void gpio_clock_control(gpio_regdef_t *gpiox, bool status)
{
    if(status)
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
    uint8_t value = (uint8_t)(gpiox->idr >> pin) & (0x1U);
    return value;
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
    uint32_t reset_bits = (uint32_t)(~value) << 16U;
    gpiox->bsrr = (set_bits | reset_bits);
}

void gpio_toggle_port(gpio_regdef_t *gpiox, uint16_t mask)
{
    gpiox->odr ^= mask;
}

void gpio_irq_handler(uint8_t pin)
{
    if(EXTI->pr & (1U << pin))
    {
        // Pending register is cleared by setting to 1
        EXTI->pr |= (1U << pin);
    }
}
