// SIMPLE BLINK FOR EXAMPLE

#include <avr/io.h>
#include <util/delay.h>
#define LED_BIT _BV(PD0)

int main()
{
	DDRD |= LED_BIT; // OUTPUT

	while (1)
	{
		PORTD |= LED_BIT; // HIGH
		_delay_ms(1000);
		PORTD &= ~LED_BIT; // LOW
		_delay_ms(1000);
	}
}