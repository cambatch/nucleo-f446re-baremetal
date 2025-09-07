#include "dwt.h"
#include "f446re.h"

static inline void barrier(void)
{
    __asm volatile ("dsb");
    __asm volatile ("isb");
}

bool dwt_init(void)
{
    volatile uint32_t *demcr = DEMCR_BASEADDR;

    // Enable DWT and ITM units
    *demcr |=  (1U << DEMCR_TRCENA);

    // Reset clock cycle counter value
    DWT->cyccnt = 0;

    // Enable clock cycle counter
    DWT->ctrl |= (1U << DWT_CTRL_CYCCNTENA);

    barrier();

    uint32_t a = DWT->cyccnt;
    uint32_t b = DWT->cyccnt;
    return (a != b);
}

// Since we are using uint32_t for cycles, the delay can be no longer than 23 seconds at 180 MHz
void dwt_delay_cycles(uint32_t cycles)
{
    uint32_t start = DWT->cyccnt;
    while((DWT->cyccnt - start) < cycles);
}

void dwt_delay_us(uint32_t us)
{
    uint64_t cycles = (rcc_system_clock_hz() / 1000000UL) * us;
    dwt_delay_cycles((uint32_t)cycles);
}

void dwt_delay_ms(uint32_t ms)
{
    uint64_t cycles = (rcc_system_clock_hz() / 1000UL) * ms;
    dwt_delay_cycles((uint32_t)cycles);
}