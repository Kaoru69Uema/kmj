// 令和4年10月6日（2022年10月6日）
// 上間つよし＠沖縄県西原町＠久米島出身
// ATtiny85 1MHz 工場出荷時
//
//                               
//       +--|Diode>--+
//       |  10u      |
//       |     +     |     ATtiny85
//       +---||------+
//       |  10uF     |  ________________
//       |           |  |.RESET    ---VCC|---5[V]
// 0[v]--+--Relay-  -+--|PB3    PB2(AIN1)|-----R330----|LED＞-----0[V]
//                      |PB2          PB1|
// 0[V]-----------------|GND          PB0|
//                       ----------------
//
// 内部基準電圧1.1[v]とAIN1電圧比較
// OPTION: Relay


#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int counter = 0;
         unsigned char cnt_on = 0;

ISR(TIM0_COMPA_vect);
int main(void);


int main(void)
{
	TCCR0A = 1 << WGM01;
	TCCR0B = 1 << CS01;
	OCR0A  = 1000000 / 8 / 1000 - 1;
	TIMSK  = 1 << OCIE0A;
	TCNT0=0;
	ACSR |= ( 1 << ACBG ); /* 内部1.1v */

	DDRB |= 1 << PB3;
	sei();

	while(1)
	{
		DDRB  |=   ( 1 << PB1 );
		PORTB &=  ~( 1 << PB1 );   /* discharge */
		asm( "nop;" );
		asm( "nop;" );
		asm( "nop;" );

		DDRB  &=   ~( 1 << PB1 );
		PORTB &=  ~( 1 << PB1 );   /* no pullup */

		TCNT0 = 0;
		counter = 1;
		while( counter ) ;  /* 約1[ms]待つ */


		if(  !( ACSR & ( 1 <<ACO ) ) ) {
			DDRB  |= 1 << PB1;
			PORTB |= 1 << PB1;
			PORTB |= 1 << PB3;
			TCNT0 = 0;
			counter = 10;
			while( counter ) ; /* 約10[ms]待つ */
		} else {
			PORTB &= ~( 1 << PB3 );
		}
	}
}

/*--------------------------------------------------*/
/*        タイマー割り込み０CTC動作                 */
/*--------------------------------------------------*/
ISR(TIM0_COMPA_vect)
{
	if( counter ) counter--;
}




