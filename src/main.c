#include "f446re.h"

#include <string.h>

#define LOW 0
#define BTN_PRESSED LOW

spi_handle_t spi_handle;
volatile uint8_t tx_data[4] = { 0xA0, 0xA1, 0xA2, 0xA3 };
volatile uint8_t rx_data[sizeof(tx_data)];

typedef enum
{
    MODE_TX = 0, MODE_RX, MODE_TXRX
} test_mode_t;

volatile test_mode_t mode = MODE_TX;

void spi_event_callback(spi_handle_t *handle, spi_event_t event)
{
    switch(event)
    {
        case SPI_EVENT_TX_DONE:
        case SPI_EVENT_RX_DONE:
        case SPI_EVENT_TXRX_DONE:
            while(spi_flag_status(handle->spix, SPI_FLAG_BUSY));
            spi_peripheral_control(handle->spix, DISABLE);
            gpio_toggle_pin(GPIOA, GPIO_PIN_5);
            break;
        case SPI_EVENT_OVR_ERR:
        case SPI_EVENT_MODF_ERR:
            spi_peripheral_control(handle->spix, DISABLE);
            handle->state = SPI_STATE_READY;
            handle->op = SPI_OP_NONE;
            break;
        default:
            break;
    }
}

void spin(volatile uint32_t count)
{
    while(count--) (void)0;
}

void init_button(void)
{
    // PC13 user button, pressed => LOW
    gpio_handle_t button;
    button.gpiox = GPIOC;
    button.config.pin_num = GPIO_PIN_13;
    button.config.mode = GPIO_MODE_INPUT;
    button.config.otype = GPIO_OTYPE_PP;
    button.config.speed = GPIO_SPEED_LOW;
    button.config.pupd = GPIO_PUPD_PU;

    gpio_init(&button);
}

void init_led(void)
{
    gpio_handle_t led;
    led.gpiox = GPIOA;
    led.config.pin_num = GPIO_PIN_5;
    led.config.mode = GPIO_MODE_OUTPUT;
    led.config.otype = GPIO_OTYPE_PP;
    led.config.speed = GPIO_SPEED_FAST;
    led.config.pupd = GPIO_PUPD_DI;
    gpio_init(&led);
}

void init_spi2(void)
{
    // PB12 => SPI2 NSS
    // PB13 => SPI2 SCK
    // PB14 => SPI2 MISO
    // PB15 => SPI2 MOSI
    gpio_handle_t spi_gpio;
    spi_gpio.gpiox = GPIOB;
    spi_gpio.config.mode = GPIO_MODE_ALTFN;
    spi_gpio.config.altfn = 5;
    spi_gpio.config.otype = GPIO_OTYPE_PP;
    spi_gpio.config.pupd = GPIO_PUPD_DI;
    spi_gpio.config.speed = GPIO_SPEED_FAST;

    // NSS
    spi_gpio.config.pin_num = GPIO_PIN_12;
    gpio_init(&spi_gpio);

    // SCK
    spi_gpio.config.pin_num = GPIO_PIN_13;
    gpio_init(&spi_gpio);

    // MISO
    spi_gpio.config.pin_num = GPIO_PIN_14;
    gpio_init(&spi_gpio);

    // MOSI
    spi_gpio.config.pin_num = GPIO_PIN_15;
    gpio_init(&spi_gpio);

    // Configure SPI2
    spi_handle.spix = SPI2;
    spi_handle.config.device_mode = SPI_MODE_MASTER;
    spi_handle.config.bus_config = SPI_BUS_FULL_DUPLEX;
    spi_handle.config.baud = SPI_BAUD_DIV8;
    spi_handle.config.cpol = SPI_CPOL_LOW;
    spi_handle.config.cpha = SPI_CPHA_1EDGE;
    spi_handle.config.ssm = SPI_SSM_HARDWARE;
    spi_handle.config.df = SPI_DF_8BIT;
    spi_handle.config.ff = SPI_FF_MSB_FIRST;

    spi_init(&spi_handle);
    spi_ssoe_control(SPI2, ENABLE);
    spi_handle.event_callback = spi_event_callback;
}

int main(void)
{
    init_button();
    init_led();
    init_spi2();

    irq_enable(IRQ_SPI2);

    while(1)
    {
        while(gpio_read_pin(GPIOC, GPIO_PIN_13));
        spin(100000);
        while(!gpio_read_pin(GPIOC, GPIO_PIN_13));
        spin(100000);

        if(spi_handle.state == SPI_STATE_READY)
        {
            // spi_transfer_it(&spi_handle, (uint8_t *)tx_data, (uint8_t *)rx_data, sizeof(tx_data));
            spi_transfer_it(&spi_handle, (uint8_t *)tx_data, NULL, sizeof(tx_data));
        }
    }
}

void SPI2_Handler(void)
{
    spi_irq_handler(&spi_handle);
}
