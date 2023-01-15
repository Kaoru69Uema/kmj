#include <avr/io.h>
#include "freq_counter.h"

unsigned char hz = 0; /* 周波数　値 */
int main( void );
void init( void );

int main( void )
{
	freq_counter_initialize();
	init();

	sei();

	while( 1 ) {
		if( freq_val != 0 ){
			hz = freq_val;
			freq_val = 0;
			asm( "nop;" );
			//PORTB &= 0b00001011;
			if( hz ) PORTB |= 1<<PB3;
			asm( "nop;" );
		}

	}

	return 0;
}

void init( void )
{
	DDRB |= ( 1<<PB0 ) | ( 1<<PB1 ) | ( 1<<PB3 );
}
