#include "f446re.h"

void spin(volatile long count)
{
    while(count--) (void)0;
}

int main(void)
{
    gpio_handle_t handle;
    handle.gpiox = GPIOA;
    handle.config.pin_num = 5;
    handle.config.mode = GPIO_MODE_OUTPUT;
    handle.config.otype = GPIO_OTYPE_PP;
    handle.config.speed = GPIO_SPEED_FAST;
    handle.config.pupd = GPIO_PUPD_DI;

    gpio_init(&handle);

    while(1)
    {
        gpio_write_pin(handle.gpiox, handle.config.pin_num, RESET);
        spin(999999);
        gpio_write_pin(handle.gpiox, handle.config.pin_num, SET);
        spin(999999);
    }
}
