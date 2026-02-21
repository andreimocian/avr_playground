#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart_hal.h>
#include <i2c_hal.h>

int main ()
{
	uint8_t read_data;
	uint8_t i2c_response;
	uint8_t data[20] = {0x10, 0x20};
	uart_init(9600, 0);
	i2c_init(100000);

	DDRB |= (1 << PB0);

	sei();

	uint8_t str[] = "Microcontroller started";

	uart_send_stringln(str);

	while(1) 
	{
		i2c_response = i2c_write(0x27, data[0], 1);
		uart_send_byte('0' + i2c_response);

		_delay_ms(1000);

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