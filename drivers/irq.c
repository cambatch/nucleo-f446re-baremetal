#include "irq.h"

void irq_enable(irq_num_t irq_num)
{
    uint8_t reg_index = irq_num / 32U;
    uint32_t mask = 1U << (irq_num % 32U);
    NVIC_ISER_BASE[reg_index] = mask;
}

void irq_disable(irq_num_t irq_num)
{
    uint8_t reg_index = irq_num / 32U;
    uint32_t mask = 1U << (irq_num % 32U);
    NVIC_ICER_BASE[reg_index] = mask;
}

void irq_set_priority(irq_num_t irq_num, uint32_t priority)
{
    if(irq_num < 240U)
    {
        volatile uint8_t *nvic_ipr = (volatile uint8_t*)NVIC_IPRO_BASEADDR;
        nvic_ipr[irq_num] = (priority & 0x0FU) << 4U;
    }
}
