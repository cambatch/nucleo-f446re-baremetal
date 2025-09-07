#ifndef DRIVER_DWT_H
#define DRIVER_DWT_H

#include <stdint.h>
#include <stdbool.h>

bool dwt_init(void);
void dwt_delay_cycles(uint32_t cycles);
void dwt_delay_us(uint32_t us);
void dwt_delay_ms(uint32_t ms);

#endif