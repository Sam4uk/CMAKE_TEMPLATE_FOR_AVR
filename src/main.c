// SIMPLE BLINK FOR EXAMPLE

#include <avr/io.h>
#include <util/delay.h>
#define LED_BIT _BV(PB5)

int main()
{
	DDRB |= LED_BIT; // OUTPUT

	while (1)
	{
		PORTB |= LED_BIT; // HIGH
		_delay_ms(1000);
		PORTB &= ~LED_BIT; // LOW
		_delay_ms(1000);
	}
}