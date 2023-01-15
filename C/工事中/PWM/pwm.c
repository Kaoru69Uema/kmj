#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

void a( void );
void b( void );
void c( void );
int main( void );


int main( void )
{
	DDRB  = 0b00011111;
	PORTB = 0b00011111;
	for(;;) {
		_delay_ms( 1000 );
		a();
		_delay_ms( 1000 );
		c();
	}
	return 0;
}

void a( void ) {
	PORTB = 0b00011111;
	_delay_ms( 0.5 );
	PORTB = 0b00000000;
	_delay_ms( 20-0.5 );
}
void b( void ) {
	PORTB = 0b00011111;
	_delay_ms( 1.45 );
	PORTB = 0b00000000;
	_delay_ms( 20-1.45 );
}
void c( void ) {
	PORTB = 0b00011111;
	_delay_ms( 2.4 );
	PORTB = 0b00000000;
	_delay_ms( 20-2.4 );
}
