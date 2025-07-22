#ifndef DRIVER_IRQ_H
#define DRIVER_IRQ_H

#include "f446re.h"

/**
 * @brief Enables an interrupt in the NVIC.
 *
 * Sets the corresponding bit in the NVIC ISER register to enable
 * the given interrupt line.
 *
 * @param irq_num Interrupt request number (see irq_num_t).
 */
void irq_enable(irq_num_t irq_num);

/**
 * @brief Disables an interrupt in the NVIC.
 *
 * Clears the corresponding bit in the NVIC ICER register to disable
 * the given interrupt line.
 *
 * @param irq_num Interrupt request number (see irq_num_t).
 */
void irq_disable(irq_num_t irq_num);

/**
 * @brief Sets the priority of an interrupt in the NVIC.
 *
 * Only the upper 4 bits of the 8-bit priority field are implemented
 * in Cortex-M4 (see RM0390 §10.1.1).
 *
 * @param irq_num Interrupt request number (see irq_num_t).
 * @param priority Priority level (0 = highest, 15 = lowest).
 */
void irq_set_priority(irq_num_t irq_num, uint32_t priority);

#endif
