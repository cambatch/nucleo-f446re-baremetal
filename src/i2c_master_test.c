#include "f446re.h"

#define SLAVE_ADDR 0x3C

gpio_handle_t g_button;
i2c_handle_t g_i2c1_handle;

void init_button(void)
{
    // PC13 user button, pressed => LOW
    g_button.gpiox = GPIOC;
    g_button.config.pin_num = GPIO_PIN_13;
    g_button.config.mode = GPIO_MODE_INPUT;
    g_button.config.otype = GPIO_OTYPE_PP;
    g_button.config.speed = GPIO_SPEED_LOW;
    g_button.config.pupd = GPIO_PUPD_PU;

    gpio_init(&g_button);
}

void init_i2c1(void)
{
    gpio_handle_t i2c_pin;
    i2c_pin.gpiox = GPIOB;
    i2c_pin.config.mode = GPIO_MODE_ALTFN;
    i2c_pin.config.altfn = 4;
    i2c_pin.config.otype = GPIO_OTYPE_OD;
    i2c_pin.config.pupd = GPIO_PUPD_DI;
    i2c_pin.config.speed = GPIO_SPEED_FAST;

    // I2C1_SCL
    i2c_pin.config.pin_num = GPIO_PIN_8;
    gpio_init(&i2c_pin);

    // I2C1_SDA
    i2c_pin.config.pin_num = GPIO_PIN_9;
    gpio_init(&i2c_pin);

    // Cofnigure I2C1
    g_i2c1_handle.i2cx = I2C1;
    g_i2c1_handle.config.scl_speed = I2C_SCL_SPEED_SM;
    g_i2c1_handle.config.device_addr = 0x01; // Doesn't matter for master mode
    g_i2c1_handle.config.ack = I2C_ACK_EN; // Doesn't matter for master mode
    g_i2c1_handle.config.duty = I2C_FMDUTY_2; // Doesn't matter for standard mode

    i2c_init(&g_i2c1_handle);
}

void spin(volatile uint32_t count)
{
    while(count--) (void)0;
}

int main()
{
    uint8_t send_buffer[1] = { 0x3E };
    uint8_t recv_buffer[1] = {};
    bool send = true;

    init_button();
    init_i2c1(); // I2C peripheral is enabled in i2c_init();

    while(1)
    {
        while(gpio_read_pin(GPIOC, GPIO_PIN_13));
        spin(100000);
        while(!gpio_read_pin(GPIOC, GPIO_PIN_13));
        spin(100000);

        if(send)
        {
            i2c_master_send(&g_i2c1_handle, send_buffer, 1, SLAVE_ADDR);
        }
        else
        {
            i2c_master_receive(&g_i2c1_handle, recv_buffer, 1, SLAVE_ADDR);
        }

        send = !send;
    }
}