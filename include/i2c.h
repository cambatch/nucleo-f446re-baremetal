#ifndef DRIVER_I2C_H
#define DRIVER_I2C_H

#include "f446re.h"

typedef enum i2c_fmduty_t
{
    I2C_FMDUTY_2 = 0,
    I2C_FMDUTY_16_9 = 1
} i2c_fmduty_t;

typedef enum i2c_ack_t
{
    I2C_ACK_DI = 0,
    I2C_ACK_EN = 1
} i2c_ack_t;

typedef enum i2c_mode_t
{
    I2C_MODE_SM = 0,
    I2C_MODE_FM = 1
} i2c_mode_t;

typedef struct
{
    uint32_t scl_speed;
    uint8_t device_addr;
    i2c_ack_t ack;
    i2c_fmduty_t duty;
} i2c_config_t;

typedef struct
{
    i2c_regdef_t *i2cx;
    i2c_config_t config;
} i2c_handle_t;

// I2C Flags
#define I2C_FLAG_SB         (1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR       (1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF        (1 << I2C_SR1_BTF)
#define I2C_FLAG_STOPF      (1 << I2C_SR1_STOPF)
#define I2C_FLAG_TXE        (1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE       (1 << I2C_SR1_RXNE)
#define I2C_FLAG_BERR       (1 << I2C_SR1_BERR)
#define I2C_FLAG_ARLO       (1 << I2C_SR1_ARLO)
#define I2C_FLAG_AF         (1 << I2C_SR1_AF)
#define I2C_FLAG_OVR        (1 << I2C_SR1_OVR)
#define I2C_FLAG_TIMEOUT    (1 << I2C_SR1_TIMEOUT)
#define I2C_FLAG_BUSY       (1 << I2C_SR2_BUSY)

// MISC
#define I2C_SCL_SPEED_SM    100000U
#define I2C_SCL_SPEED_FM2K  200000U
#define I2C_SCL_SPEED_FM4K  400000U

void i2c_init(i2c_handle_t *handle);

void i2c_clock_control(i2c_regdef_t *i2cx, bool status);

void i2c_peripheral_control(i2c_regdef_t *i2cx, bool status);

bool i2c_flag_status1(i2c_regdef_t *i2cx, uint32_t flag);
bool i2c_flag_status2(i2c_regdef_t *i2cx, uint32_t flag);

void i2c_generate_start(i2c_regdef_t *i2cx);
void i2c_generate_stop(i2c_regdef_t *i2cx);
void i2c_address_phase_transmit(i2c_regdef_t *i2cx, uint8_t slave_addr);
void i2c_address_phase_receive(i2c_regdef_t *i2cx, uint8_t slave_addr);
void i2c_clear_addr(i2c_regdef_t *i2cx);

void i2c_master_send(i2c_handle_t *handle, uint8_t *buffer, uint32_t bytes, uint8_t slave_addr);
void i2c_master_receive(i2c_handle_t *handle, uint8_t *buffer, uint32_t bytes, uint8_t slave_addr);

#endif
