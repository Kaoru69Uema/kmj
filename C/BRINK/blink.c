#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main( void )
{
	DDRB  = 0b00011111;
	PORTB = 0b00011111;
	for(;;) {
		_delay_ms( 1000 );
		PORTB ^= 0b00011111;
	}
	return 0;
}
