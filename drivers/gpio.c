#include "gpio.h"

void gpio_init(gpio_handle_t *handle)
{
    // Enable GPIO clock
    gpio_clock_control(handle->gpiox, ENABLE);

    // Configure mode
    if(handle->config.mode <= GPIO_MODE_ALTFN)
    {
        handle->gpiox->moder |= (handle->config.mode << (2 * handle->config.pin_num));
    }
    else
    {
        // Interrupt information found in ARM DUI 0553A cortex-m and STM RM0390
        // Set pin as input
        handle->gpiox->moder &= ~(1U << handle->config.pin_num);

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
    return (uint8_t)((gpiox->idr >> pin) & 0x1U);
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

void gpio_irq_config(uint8_t irq_num, bool status)
{
    // ISER0 covers IRQ 0-31, ISER1 covers IRQ 32-63, etc...
    uint8_t reg_index = irq_num / 32U;
    uint32_t mask = 1U << (irq_num % 32U);

    if(status == ENABLE)
    {
        NVIC_ISER_BASE[reg_index] = mask;
    }
    else
    {
        NVIC_ICER_BASE[reg_index] = mask;
    }
}

void gpio_irq_priority_config(uint8_t irq_num, uint32_t priority)
{
    // Cortex-M4 supports up to 240 external IRQs
    if(irq_num < 240U)
    {
        volatile uint8_t *nvic_ipr = (volatile uint8_t*)NVIC_IPRO_BASEADDR;
        // Only 4 bits of interrupt priority are used (RM0390 10.1.1)
        nvic_ipr[irq_num] = (priority & 0x0FU) << 4U;
    }
}

void gpio_irq_handler(uint8_t pin)
{
    if(EXTI->pr & (1 << pin))
    {
        // Pending register is cleared by setting to 1
        EXTI->pr |= (1 << pin);
    }
}
