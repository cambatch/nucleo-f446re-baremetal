#include "f446re.h"

#include <string.h>

#define LOW 0
#define BTN_PRESSED LOW

void spin(volatile long count)
{
    while(count--) (void)0;
}

void init_button(void)
{
    // PC13 user button, pressed => LOW
    gpio_handle_t button;
    button.gpiox = GPIOC;
    button.config.pin_num = GPIO_PIN_13;
    button.config.mode = GPIO_MODE_IT_FT;
    button.config.otype = GPIO_OTYPE_PP;
    button.config.speed = GPIO_SPEED_LOW;
    button.config.pupd = GPIO_PUPD_PU;

    gpio_init(&button);
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
    spi_handle_t spi;
    spi.spix = SPI2;
    spi.config.device_mode = SPI_MODE_MASTER;
    spi.config.bus_config = SPI_BUS_FULL_DUPLEX;
    spi.config.baud = SPI_BAUD_DIV8;
    spi.config.cpol = SPI_CPOL_LOW;
    spi.config.cpha = SPI_CPHA_1EDGE;
    spi.config.ssm = SPI_SSM_HARDWARE;
    spi.config.df = SPI_DF_8BIT;
    spi.config.ff = SPI_FF_MSB_FIRST;

    spi_init(&spi);
    spi_ssoe_control(SPI2, ENABLE);
}

int main(void)
{
    init_button();
    init_spi2();

    irq_enable(IRQ_EXTI15_10);

    while(1);
}

void send_message()
{
    char data[] = "Hello world!\n";
    uint8_t data_len = (uint8_t)strlen(data);

    spi_peripheral_control(SPI2, ENABLE);
    spi_send(SPI2, &data_len, 1);
    spi_send(SPI2, (uint8_t *)data, data_len);
    while(spi_flag_status(SPI2, SPI_FLAG_BUSY));
    spi_peripheral_control(SPI2, DISABLE);
}

void EXTI15_10_Handler(void)
{
    gpio_irq_handler(GPIO_PIN_13);
    send_message();
}
