#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart_hal.h>
#include <i2c_hal.h>

void uart_send_hex(uint8_t val)
{
    const uint8_t hex[] = "0123456789ABCDEF";
    uart_send_byte(hex[(val >> 4) & 0x0F]);
    uart_send_byte(hex[val & 0x0F]);
}

int main ()
{
	// uint8_t read_data;
	uint8_t i2c_response;
	// uint8_t data[20] = {0x10, 0x20};

	uint8_t mcp7490_addr = 0x6F;
	uint8_t register_addr = 0x00;
	uint8_t data;
	uint8_t osc_run_status;

	uart_init(9600, 0);
	i2c_init(100000);

	DDRB |= (1 << PB0);

	sei();

	uint8_t str[] = "Microcontroller started";

	uart_send_stringln(str);

	i2c_response = i2c_byte_write(mcp7490_addr, register_addr, 0x80);
	// i2c_response = i2c_random_read(mcp7490_addr, 0x03, &data);
	// uart_send_byte('0' + i2c_response);
    // uart_send_byte('\r');
	// uart_send_byte('\n');


	while(1) 
	{
		// i2c_response = i2c_byte_write(mcp7490_addr, register_addr, 0x80);
		// uart_send_byte('0' + i2c_response);
		i2c_response = i2c_random_read(mcp7490_addr, 0x00, &data);
		osc_run_status = data & 0x20;

		uart_send_byte('0' + i2c_response);
		uart_send_hex(data);
    	uart_send_byte('\r');
		uart_send_byte('\n');
		

		_delay_ms(500);

		/* if(uart_read_count() > 0)
		{
			read_data = uart_read();

			if (read_data == 'o') {
				PORTB |= (1 << PORTB0);
			} else if (read_data == 'f') {
				PORTB &= ~(1 << PORTB0);
			}

			uart_send_byte(read_data);
		} */
	}
}