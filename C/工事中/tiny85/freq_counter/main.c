///////////////////////////////////////////////////////////////////
//
//                           Tiny85
//                          ---------
//                       --|Reset VCC|--5[v]
//       0[v]--[330]--LED--|PB3   PB2|--
//       0[v]-------SW-----|PB4   PB1|--
//       0[v]--------------|GND   PB0|--Sounder--[330]--5[v]
//                          ---------
//
//                                      PWM-PORT : PB0
//
///////////////////////////////////////////////////////////////////



/* ヘッダファイル */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "soft_timer.h"

/* グローバル変数 */
volatile unsigned char f_1ms = 0;


/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIM0_COMPA_vect);
ISR(TIM1_COMPA_vect);



int main( void )
{

	initialize();  /* 初期化： 一般                 */
	t_ini();       /* 初期化： ソフトタイマー       */

	sei();

	while( 1 ) {

		while( f_1ms < 2 ) ;
		f_1ms = 0;

		PORTB |= 0x0f;
		asm( "nop" );
	}
	return 0;
}

void initialize( void )
{
	TCCR0A = 1 << WGM01;
	TCCR0B = 0b011 << CS00;  /* 8000000/64 */
	OCR0A  = 8000000/64/5000-1; /* 24 */
	TCNT0=0;
	DDRB = ( 1 << PB3 ) | ( 1<<PB1 )  ;
	asm( "nop" );
	PORTB |= 1<<PB4;

	PLLCSR |= 1<<PLLE;
	_delay_us( 100 );
	while( !( PLLCSR & ( 1<<PLOCK ) ) ) ;
	PLLCSR |= 1<<PCKE;
	asm( "nop" );
	TCCR1 = ( 1<<CTC1 ) | ( 0b0001<<CS10 );
	OCR1C = 127;

	TIMSK  = ( 1 << OCIE0A ) | ( 1<<OCIE1A );
}


ISR(TIM0_COMPA_vect)
{
}
ISR(TIM1_COMPA_vect)
{
	++f_1ms;
}

