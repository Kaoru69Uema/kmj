#include "timer0.h"
#include <avr/io.h>
volatile unsigned char f_1ms;

void t0_ini( void )
{
	TCCR0A = 1 << WGM01;
	TCCR0B = 3 << CS00;
	OCR0A  = 8000000/64/1000-1;
	TIMSK  = 1 << OCIE0A;
	TCNT0  = 0;
	f_1ms  = 0;
}

ISR( TIMER0_COMPA_vect )
{
	f_1ms = 1;
}
