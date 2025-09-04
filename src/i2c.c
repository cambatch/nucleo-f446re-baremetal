#include "i2c.h"

#include <assert.h>

void i2c_init(i2c_handle_t *handle)
{
    uint32_t apb1_clock_hz = rcc_apb1_clock_hz();
    uint8_t freq = (uint8_t)(apb1_clock_hz / 1000000U);

    // Enable peripheral clock
    i2c_clock_control(handle->i2cx, ENABLE);

    // Disable I2C peripheral before configuration
    handle->i2cx->cr1 &= ~(0x1U << I2C_CR1_PE);

    // ===== Configure peripheral clock frequency =====
    handle->i2cx->cr2 &= ~(0x3FU << I2C_CR2_FREQ);
    handle->i2cx->cr2 |= (freq << I2C_CR2_FREQ);

    // ===== Configure CCR value =====
    uint16_t ccr_value = 0;
    // Clear CCR, FS, and DUTY bits
    handle->i2cx->ccr &= ~((0xFFFU << I2C_CCR_CCR) | (0x1 << I2C_CCR_FS) | (0x1 << I2C_CCR_DUTY));

    if(handle->config.scl_speed <= I2C_SCL_SPEED_SM)
    {
        // Standard mode
        ccr_value = (uint16_t)(apb1_clock_hz / (2 * handle->config.scl_speed));
    }
    else
    {
        // Fast mode
        // Configure controller mode
        handle->i2cx->ccr |= (0x1 << I2C_CCR_FS);

        // Configure fast mode duty cycle
        handle->i2cx->ccr |= (handle->config.duty << I2C_CCR_DUTY);

        if(handle->config.duty == I2C_FMDUTY_2)
        {
            ccr_value = (uint16_t)(apb1_clock_hz / (3 * handle->config.scl_speed));
        }
        else
        {
            ccr_value = (uint16_t)(apb1_clock_hz / (25 * handle->config.scl_speed));
        }
    }

    if(ccr_value < 1) ccr_value = 1;
    handle->i2cx->ccr |= (ccr_value & 0xFFFU);

    uint8_t trise = 0;

    // ===== Configure TRISE =====
    if(handle->config.scl_speed <= I2C_SCL_SPEED_SM)
    {
        // SM max rise time 1000 ns
        trise = (uint8_t)((apb1_clock_hz / 1000000U) + 1);
    }
    else
    {
        // FM max rise time 300 ns
        trise = (uint8_t)(((apb1_clock_hz / 1000000) * 300) / 1000 + 1);
    }
    if(trise > 63) trise = 63;
    handle->i2cx->trise = trise;

    // ===== Configure the ACK bit =====
    // I2C peripheral needs to be enabled to set ACK bit
    i2c_peripheral_control(handle->i2cx, ENABLE);
    handle->i2cx->cr1 &= ~(0x1U << I2C_CR1_ACK);
    handle->i2cx->cr1 |= (handle->config.ack << I2C_CR1_ACK);
}

void i2c_clock_control(i2c_regdef_t *i2cx, bool status)
{
    if(status)
    {
        if(i2cx == I2C1) I2C1_PCLK_EN();
        else if(i2cx == I2C2) I2C2_PCLK_EN();
        else if(i2cx == I2C3) I2C3_PCLK_EN();
    }
    else
    {
        if(i2cx == I2C1) I2C1_PCLK_DI();
        else if(i2cx == I2C2) I2C2_PCLK_DI();
        else if(i2cx == I2C3) I2C3_PCLK_DI();
    }
}

void i2c_peripheral_control(i2c_regdef_t *i2cx, bool status)
{
    if(status)
    {
        i2cx->cr1 |= (1U << I2C_CR1_PE);
    }
    else
    {
        i2cx->cr1 &= ~(1U << I2C_CR1_PE);
    }
}

bool i2c_flag_status1(i2c_regdef_t *i2cx, uint32_t flag)
{
    return (i2cx->sr1 & flag) != 0;
}

bool i2c_flag_status2(i2c_regdef_t *i2cx, uint32_t flag)
{
    return (i2cx->sr2 & flag) != 0;
}

void i2c_generate_start(i2c_regdef_t *i2cx)
{
    i2cx->cr1 |= (0x1U << I2C_CR1_START);
}

void i2c_generate_stop(i2c_regdef_t *i2cx)
{
    i2cx->cr1 |= (0x1U << I2C_CR1_STOP);
}

void i2c_address_phase_transmit(i2c_regdef_t *i2cx, uint8_t slave_addr)
{
    uint8_t addr_byte = slave_addr << 1;
    addr_byte &= (uint8_t)~(0x1U);
    i2cx->dr = addr_byte;
}

void i2c_address_phase_receive(i2c_regdef_t *i2cx, uint8_t slave_addr)
{
    uint8_t addr_byte = slave_addr << 1;
    addr_byte |= (uint8_t)(0x1U);
    i2cx->dr = addr_byte;
}

void i2c_clear_addr(i2c_regdef_t *i2cx)
{
    volatile uint32_t temp;
    temp = i2cx->sr1;
    temp = i2cx->sr2;
    (void)temp;
}

// TODO: Add repeated starts. Timeouts for flag checks. Error checks (AF, BERR, ARLO) while waiting for ADDR flag.
void i2c_master_send(i2c_handle_t *handle, uint8_t *buffer, uint32_t bytes, uint8_t slave_addr)
{
    while(i2c_flag_status2(handle->i2cx, I2C_FLAG_BUSY));

    // Generate Start condition
    i2c_generate_start(handle->i2cx);

    while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_SB));

    // Address phase
    i2c_address_phase_transmit(handle->i2cx, slave_addr);

    while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_ADDR));

    // Clear ADDR flag
    i2c_clear_addr(handle->i2cx);

    while(bytes > 0)
    {
        while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_TXE))
        {
            if(i2c_flag_status1(handle->i2cx, I2C_FLAG_AF))
            {
                // Acknowledge failure => stop communication
                handle->i2cx->sr1 &= ~(0x1U << I2C_SR1_AF);
                i2c_generate_stop(handle->i2cx);
                return;
            }
        }

        handle->i2cx->dr = *buffer;
        ++buffer;
        --bytes;
    }

    while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_TXE));
    while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_BTF));

    i2c_generate_stop(handle->i2cx);
}

// TODO: Add timeouts
void i2c_master_receive(i2c_handle_t *handle, uint8_t *buffer, uint32_t bytes, uint8_t slave_addr)
{
    while(i2c_flag_status2(handle->i2cx, I2C_FLAG_BUSY));

    // Generate start condition
    i2c_generate_start(handle->i2cx);

    while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_SB));

    // Address phase
    i2c_address_phase_receive(handle->i2cx, slave_addr);

    while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_ADDR));

    if(bytes == 1)
    {
        handle->i2cx->cr1 &= ~(0x1U << I2C_CR1_ACK);

        i2c_clear_addr(handle->i2cx);

        i2c_generate_stop(handle->i2cx);

        while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_RXNE));

        *buffer = (uint8_t)handle->i2cx->dr;
    }
    else if(bytes == 2)
    {
        handle->i2cx->cr1 &= ~(0x1U << I2C_CR1_ACK);
        handle->i2cx->cr1 |= (0x1U << I2C_CR1_POS);

        i2c_clear_addr(handle->i2cx);

        while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_BTF));

        i2c_generate_stop(handle->i2cx);

        *buffer++ = (uint8_t)handle->i2cx->dr;
        *buffer = (uint8_t)handle->i2cx->dr;
    }
    else
    {
        i2c_clear_addr(handle->i2cx);

        while(bytes > 3)
        {
            while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_RXNE));

            *buffer++ = (uint8_t)handle->i2cx->dr;
            --bytes;
        }

        // Data N-2 in DR, data N-1 in shift reg, SCL stretched low until N-2 is read
        while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_BTF));

        handle->i2cx->cr1 &= ~(0x1U << I2C_CR1_ACK);

        *buffer++ = (uint8_t)handle->i2cx->dr;

        // Data N-1 in DR, data N in shift reg, SCL stretched low until N-1 is read
        while(!i2c_flag_status1(handle->i2cx, I2C_FLAG_BTF));

        i2c_generate_stop(handle->i2cx);

        *buffer++ = (uint8_t)handle->i2cx->dr;
        *buffer = (uint8_t)handle->i2cx->dr;
    }

    // Restore POS and ACK to configured states
    handle->i2cx->cr1 &= ~(0x1U << I2C_CR1_POS);
    if(handle->config.ack == I2C_ACK_EN)
    {
        handle->i2cx->cr1 |= (0x1U << I2C_CR1_ACK);
    }
}
