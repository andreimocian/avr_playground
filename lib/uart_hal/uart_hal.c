#include "uart_hal.h"

volatile static uint8_t is_busy = 0;
volatile static uint16_t count = 0;
volatile static uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};

ISR(USART_RX_vect)
{
    volatile static uint16_t write_pos = 0;

    rx_buffer[write_pos] = UDR0;
    count++;
    write_pos++;
    if(count >= RX_BUFFER_SIZE)
    {
        write_pos = 0;
    }
}

ISR(USART_TX_vect)
{
    is_busy = 0;
}

void uart_init(uint32_t baud, uint8_t high_speed)
{
    uint32_t ubrr = (F_CPU/(16 * baud)) - 1;

    if(high_speed != 0) 
    {
        ubrr = (F_CPU/(8 * baud)) - 1;
        UCSR0A |= (1 << U2X0); 
    } 

    UBRR0H = ubrr >> 8;
    UBRR0L = ubrr;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << TXCIE0);
}

void uart_send_byte(uint8_t data)
{
    while(is_busy == 1);
    is_busy = 1;
    UDR0 = data;
}

void uart_send_stringln(uint8_t *str)
{
    uint16_t i = 0;
    while(str[i] != '\0')
    {
        uart_send_byte(str[i]);
        i++;
    }
    uart_send_byte(str[i]);
    uart_send_byte('\r');
    uart_send_byte('\n');
}

uint16_t uart_read_count()
{
    return count;
}

uint8_t uart_read()
{
    static uint16_t read_pos = 0;
    uint8_t data = 0;

    data = rx_buffer[read_pos];
    read_pos++;
    count--;
    if(read_pos >= RX_BUFFER_SIZE)
    {
        read_pos = 0;
    }

    return data;
}