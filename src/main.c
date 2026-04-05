#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart_hal.h>
#include <i2c_hal.h>
#include <mcp7940.h>

int main ()
{
	mcp7940_time time;
	uint8_t mcp7940_status = 0;
	uint8_t seconds = 0;

	uart_init(9600, 0);
	i2c_init(100000);

	DDRB |= (1 << PB0);

	sei();

	uint8_t str[] = "Microcontroller started";

	uart_send_stringln(str);

	mcp7940_status = mcp7940_device_start();
	uart_send_number(mcp7940_status);
	uart_send_byte('\r');
	uart_send_byte('\n');


	while(1) 
	{
		time = mcp7940_now();

		if(time.seconds != seconds)
		{
			seconds = time.seconds;
			uart_send_number(time.seconds);
			uart_send_byte(' ');
			PORTB ^= (1 << PORTB0);
		}
	}
}