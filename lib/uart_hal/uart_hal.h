#ifndef UART_HAL_H
#define UART_HAL_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define RX_BUFFER_SIZE 128

void uart_init(uint32_t baud, uint8_t high_speed);
void uart_send_byte(uint8_t data);
void uart_send_stringln(uint8_t *str);
uint16_t uart_read_count();
uint8_t uart_read();

#endif