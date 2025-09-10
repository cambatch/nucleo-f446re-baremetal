# STM32F446RE Bare-Metal Drivers

This repository provides a collection of **bare-metal peripheral drivers** for the STM32 Nucleo-F446RE board.

---

## Included Drivers

### GPIO
- Configure pins as input, output, alternate function, or analog.
- Control output type (push-pull, open-drain), speed, and pull-up/pull-down.
- Atomic pin read/write/toggle operations.
- Full port writes and masked toggles.
- EXTI interrupt handling support.

### SPI
- Master/slave configuration with full-/half-duplex or RX-only modes.
- Configurable baud rate, clock polarity/phase, and data format (8/16-bit).
- Polling-based send/receive.
- Interrupt-driven transfers with callback support.
- Error detection (OVR, MODF).

### I2C
- Master mode with standard-mode (100 kHz) and fast-mode (400 kHz) support.
- Configurable ACK and fast-mode duty cycle.
- Start/stop generation helpers.
- Address-phase handling for transmit and receive.
- Blocking master send/receive APIs.

### USART
- Configurable baud rates (1.2 k – 3 M).
- Modes: TX only, RX only, TX/RX.
- Stop bits, parity, word length, and hardware flow control.
- Blocking send and receive.
- Clock and peripheral control functions.

### RCC Helpers
- Enable/disable/reset peripheral clocks (GPIO, SPI, I2C, USART).
- Query system clock source and frequency.
- Compute AHB/APB prescalers and derived bus clocks.

### DWT Timing
- Initialize and use the Data Watchpoint & Trace (DWT) cycle counter.
- Busy-wait delays in **cycles**, **microseconds**, or **milliseconds**.

### IRQ Management
- Enable/disable NVIC interrupts.
- Set interrupt priority (0 = highest, 15 = lowest).

### MCU Definitions
- Register definitions and base addresses for RCC, GPIO, EXTI, SYSCFG, SPI, I2C, USART, and DWT.
- Interrupt numbers for STM32F446.
- Register bit-position macros for peripheral control.

---

## Toolchain

- **Compiler:** [arm-none-eabi-gcc](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- **Flashing:** [STM32_Programmer_CLI](https://www.st.com/en/development-tools/stm32cubeprog.html)
- **Board:** STM32 Nucleo-F446RE

---

## References

[Reference manual](https://www.st.com/resource/en/reference_manual/rm0390-stm32f446xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)

[Datasheet](https://www.st.com/resource/en/datasheet/stm32f446re.pdf)

[ARM Cortex-M4 Generic User Guide](https://developer.arm.com/documentation/dui0553/latest/)
