#include "f446re.h"

void spin(volatile long count)
{
    while(count--) (void)0;
}

void init_button(void)
{
    // PC13 user button, pressed => LOW
    gpio_handle_t button;
    button.gpiox = GPIOC;
    button.config.pin_num = GPIO_PIN_NUM_13;
    button.config.mode = GPIO_MODE_IT_FT;
    button.config.otype = GPIO_OTYPE_PP;
    button.config.speed = GPIO_SPEED_FAST;
    button.config.pupd = GPIO_PUPD_PU;

    gpio_init(&button);
}

void init_led(void)
{
    gpio_handle_t led;
    led.gpiox = GPIOA;
    led.config.pin_num = GPIO_PIN_NUM_5;
    led.config.mode = GPIO_MODE_OUTPUT;
    led.config.otype = GPIO_OTYPE_PP;
    led.config.speed = GPIO_SPEED_FAST;
    led.config.pupd = GPIO_PUPD_DI;

    gpio_init(&led);
}

int main(void)
{
    init_button();
    init_led();

    gpio_irq_config(IRQ_NUM_EXTI15_10, ENABLE);

    while(1) (void)0;
}

void EXTI15_10_Handler(void)
{
    gpio_irq_handler(GPIO_PIN_NUM_13);
    gpio_toggle_pin(GPIOA, GPIO_PIN_NUM_5);
}
