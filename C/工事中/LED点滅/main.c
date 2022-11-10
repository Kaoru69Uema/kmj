//
// LED点滅　ソフトタイマー使用
//
// 上間つよし＠沖縄県西原町＠久米島出身
//
// tiny85 内蔵１MHz
//


#include <avr/io.h>
#include <avr/interrupt.h>

#include "soft_timer.h"

volatile unsigned char f_1ms = 0;

#define LED    PB0


int main( void );
void initialize( void );
ISR( TIM0_COMPA_vect );

int main( void )
{
	t_ini();
	initialize();
	sei();

	t_req( 0 );
	while( 1 ) {
		t_main();

		if( t_tup( 0 ) ) {
			t_req( 0 );
			PORTB ^= 1 << LED;
		}

		while( ! f_1ms ) ;
		f_1ms = 0;
	}
	return 0;
}

void initialize( void )
{
	TCCR0A  = 1 << WGM01;
	TCCR0B  = 0b010 << CS00;
	OCR0A   = 1000000/8/1000-1;
	TIMSK   = 1 << OCIE0A;
	TCNT0   = 0;
	DDRB    = 1 << LED;
}

ISR( TIM0_COMPA_vect )
{
	f_1ms = 1;
}
