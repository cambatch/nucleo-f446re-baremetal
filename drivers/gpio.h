#ifndef DRIVER_GPIO_H
#define DRIVER_GPIO_H

#include <stdbool.h>

#include "f446re.h"

typedef struct
{
    uint8_t pin_num;
    uint8_t mode;
    uint8_t otype;
    uint8_t speed;
    uint8_t pupd;
    uint8_t altfn;
} gpio_config_t;

typedef struct
{
    gpio_regdef_t *gpiox;
    gpio_config_t config;
} gpio_handle_t;

// GPIO pin numbers
#define GPIO_PIN_NUM_0  0
#define GPIO_PIN_NUM_1  1
#define GPIO_PIN_NUM_2  2
#define GPIO_PIN_NUM_3  3
#define GPIO_PIN_NUM_4  4
#define GPIO_PIN_NUM_5  5
#define GPIO_PIN_NUM_6  6
#define GPIO_PIN_NUM_7  7
#define GPIO_PIN_NUM_8  8
#define GPIO_PIN_NUM_9  9
#define GPIO_PIN_NUM_10 10
#define GPIO_PIN_NUM_11 11
#define GPIO_PIN_NUM_12 12
#define GPIO_PIN_NUM_13 13
#define GPIO_PIN_NUM_14 14
#define GPIO_PIN_NUM_15 15

// GPIO modes
#define GPIO_MODE_INPUT  0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_ALTFN  2
#define GPIO_MODE_ANALOG 3
#define GPIO_MODE_IT_RT  4 // Rising edge trigger
#define GPIO_MODE_IT_FT  5 // Falling edge trigger
#define GPIO_MODE_IT_RFT 6 // Rising and falling edge trigger

// GPIO output type
#define GPIO_OTYPE_PP 0 // Push-pull
#define GPIO_OTYPE_OD 1 // Open-drain

// GPIO output speed
#define GPIO_SPEED_LOW  0
#define GPIO_SPEED_MED  1
#define GPIO_SPEED_FAST 2
#define GPIO_SPEED_HIGH 3

// GPIO pull-up/pull-down
#define GPIO_PUPD_DI 0 // No pull-up, pull-down
#define GPIO_PUPD_PU 1
#define GPIO_PUPD_PD 2

/**
 * @brief Initializes the given GPIO pin with the configuration provided.
 *
 * This function sets up the mode, output type, speed, pull-up/pull-down,
 * and alternate function (if applicable) for the specified GPIO pin.
 *
 * @param handle Pointer to GPIO handle containing the port, pin number,
 *               and all configuration parameters.
 */
void gpio_init(gpio_handle_t *handle);

/**
 * @brief Enables or disables the GPIO peripheral clock
 *
 * Activates or deactivates the clock for the specified GPIO port.
 *
 * @param gpiox Pointer to the GPIO peripheral.
 * @param status ENABLE to turn the clock on, DISABLE to turn it off.
 */
void gpio_clock_control(gpio_regdef_t *gpiox, bool status);

/**
 * @brief Read the value of the GPIO pin from the given port.
 *
 * Returns the logic level (0 or 1) currently present on the specified pin.
 *
 * @param gpiox Pointer to the GPIO port.
 * @param pin Pin number to be read from (0-15).
 * @return Logic level of the pin: 0 (low) or 1 (high).
 */
uint8_t gpio_read_pin(gpio_regdef_t *gpiox, uint8_t pin);

/**
 * @brief Writes a value to a GPIO output pin.
 *
 * Sets or resets the specified GPIO pin using the atomic BSRR register.
 *
 * @param gpiox Pointer to GPIO the port.
 * @param pin Pin number to write to (0-15).
 * @param value Value to write to pin: 0 (low) or 1 (high).
 */
void gpio_write_pin(gpio_regdef_t *gpiox, uint8_t pin, uint8_t value);

/**
 * @brief Toggle the value of a GPIO output pin.
 *
 * Flips the current output state of the specified pin.
 *
 * @param gpiox Pointer to GPIO the port.
 * @param pin Pin to be toggled (0-15).
 */
void gpio_toggle_pin(gpio_regdef_t *gpiox, uint8_t pin);

/**
 * @brief Writes a 16-bit value to the entire GPIO port.
 *
 * Each bit in the input value corresponds to the logic level of a pin.
 * This operation is atomic using the BSSR register.
 *
 * @param gpiox Pointer to GPIO the port.
 * @param value 16-bit value to write to the port (bit 0 = pin 0, ... bit 15 = pin 15).
 */
void gpio_write_port(gpio_regdef_t *gpiox, uint16_t value);

/**
 * @brief Toggles multiple GPIO output pins simultaneously.
 *
 * Applies XOR on the ODR register using a pin mask. Each 1 in the mask
 * will toggle the corresponding pin.
 *
 * @param gpiox Pointer to the GPIO port.
 * @param mask  Bitmask indicating which pins to toggle (e.g., 0x00FF for pins 0–7).
 */
void gpio_toggle_port(gpio_regdef_t *gpiox, uint16_t mask);

/**
 * @brief Enable or disable an NVIC line that services a GPIO/EXTI interrupt
 * 
 * @param irq_num The IRQ number (not EXTI line) e.g., EXTI0 = 6
 * @param status true => enable interrupt, false => disable interrupt
 */
void gpio_irq_config(uint8_t irq_num, bool status);

/**
 * @brief Set the NVIC priority for a given IRQ number.
 * 
 * @param irq_num IRQ number. e.g., EXTI0 = 6
 * @param priority Priority level (0 = highest, 15=lowest)
 */
void gpio_irq_priority_config(uint8_t irq_num, uint32_t priority);

/**
 * @brief Clears the pending interrupt bit for the triggered GPIO line.
 * 
 * This function should be called from the corresponding IRQ handler e.g., EXTIx_Handler
 * to acknowledge and clear the interrupt for the EXTI line associated with the GPIO pin.
 * 
 * @param pin The GPIO pin number (0-15) on which the interrupt occurred.
 * 
 */
void gpio_irq_handler(uint8_t pin);

#endif
