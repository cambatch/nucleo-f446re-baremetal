#ifndef DRIVER_USART_H
#define DRIVER_USART_H

#include "f446re.h"


typedef enum usart_mode_t
{
    USART_MODE_TX_ONLY = 0,
    USART_MODE_RX_ONLY,
    USART_MODE_TXRX
} usart_mode_t;

typedef enum usart_parity_t
{
    USART_PARITY_DISABLE = 0,
    USART_PARITY_EN_EVEN,
    USART_PARITY_EN_ODD
} usart_parity_t;

typedef enum usart_wordlen_t
{
    USART_WORDLEN_8 = 0,
    USART_WORDLEN_9
} usart_wordlen_t;

typedef enum usart_stopbits_t
{
    USART_STOPBITS_1 = 0,
    USART_STOPBITS_0_5,
    USART_STOPBITS_2,
    USART_STOPBITS_1_5
} usart_stopbits_t;

typedef enum usart_hwflow_t
{
    USART_HWFLOW_NONE = 0,
    USART_HWFLOW_CTS,
    USART_HWFLOW_RTS,
    USART_HWFLW_CTS_RTS
} usart_hwflow_t;

#define USART_BAUD_1200					1200U
#define USART_BAUD_2400					2400U
#define USART_BAUD_9600					9600U
#define USART_BAUD_19200 				19200U
#define USART_BAUD_38400 				38400U
#define USART_BAUD_57600 				57600U
#define USART_BAUD_115200 				115200U
#define USART_BAUD_230400 				230400U
#define USART_BAUD_460800 				460800U
#define USART_BAUD_921600 				921600U
#define USART_BAUD_2M 					2000000U
#define SUART_BAUD_3M 					3000000U

typedef struct
{
    usart_mode_t mode;
    uint32_t baud;
    usart_stopbits_t stop_bits;
    usart_wordlen_t word_len;
    usart_parity_t parity;
    usart_hwflow_t hw_control;
} usart_config_t;

typedef struct
{
    usart_regdef_t *usartx;
    usart_config_t config;
} usart_handle_t;


#define USART_FLAG_RXNE (1U << USART_SR_RXNE)
#define USART_FLAG_TC (1U << USART_SR_TC)
#define USART_FLAG_TXE (1U << USART_SR_TXE)



void usart_clock_control(usart_regdef_t *usartx, bool enable);
void usart_peripheral_control(usart_regdef_t *usartx, bool enable);

void usart_init(usart_handle_t *handle);
void usart_set_baudrate(usart_regdef_t *usartx, uint32_t baud_rate);

void usart_send(usart_handle_t *handle, uint8_t *buffer, uint32_t len);
void usart_receive(usart_handle_t *handle, uint8_t *buffer, uint32_t len);

#endif