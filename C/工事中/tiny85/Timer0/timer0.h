#ifndef __TIMER0__
#define __TIMER0__

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile unsigned char f_1ms;

void t0_ini( void );

ISR( TIMER0_COMPA_vect );


#endif
