#include <util/delay.h>
#include "freq_counter.h"

unsigned char freq_val;  /* 周波数計測値 */

ISR( INT0_vect );
ISR( TIM1_COMPA_vect );
void freq_counter_initialize( void )
{
	///////////////////////
	/* INT0割込み: PB2   */
	/* PIN: LOWレベル    */
	/* 外部割り込み0許可 */
	///////////////////////
	GIMSK |= 1<<INT0;

	/* タイマークロック：PLL == 64[MHz] へ切り替え */
        PLLCSR |= 1<<PLLE;                     /* PLL許
可             */
        _delay_us( 100 );                      /*100[us]待機          */
        while( !( PLLCSR & ( 1<<PLOCK ) ) ) ;  /* PLOCK == 1 まで待機 */
        PLLCSR |= 1<<PCKE;
        asm( "nop" );
        /* タイマークロック：PLL == 64[MHz] へ切り替え: ここまで */

	PORTB |= 1<<PB2;




}


ISR( INT0_vect )
{
	GIMSK &= ~( 1<<INT0 ); /* INT0:割込み禁止 */
//	PORTB |= 1<<PB3;

	////////////////////////////////
	/* Timer1:CTC:64MHz/256:1[ms] */
	////////////////////////////////
	TCCR1 = ( 1<<CTC1 )| ( 0b1001<<CS10 );   /* CK/256 */
	OCR1C = 64000000/256/1000-1; /* 249: 100[ms] */
	TIMSK = 1<<OCIE1A;
	TIFR |= 1<<TOV1;
	TCNT1 = 0;

	////////////////////////////////
	/* Timer0:OVF:T0入力          */
	////////////////////////////////
	TCCR0A = 0;
	TCCR0B = 0b110<<CS00; /* T0入力 */
	TIFR   |= 1<<TOV0;    /* リセット */
	TCNT0  = 0;
}

ISR( TIM1_COMPA_vect )
{
	static unsigned char cntr = 0;
	if( ++cntr == 100 ) {
		cntr = 0;
	} else {
		return;
	}
	if( TIFR & ( 1<<TOV0 ) ) freq_val = 0xff;
	else freq_val = TCNT0;
	GIMSK |= 1<<INT0;
	GIFR  |= INTF0;
}
