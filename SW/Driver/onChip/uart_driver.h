#ifndef __UART_DRIVER_H
#define __UART_DRIVER_H
#include "gpio_drive.h"
#include "stm32f1xx.h"
#include <stdint.h>
void init_uart(uint8_t uart, uint32_t BRR);
uint32_t generated_baudrate(uint8_t usart, uint32_t baud);
uint8_t rx_data(uint8_t usart);
void tx_data(uint8_t usart, uint8_t c);
#endif
