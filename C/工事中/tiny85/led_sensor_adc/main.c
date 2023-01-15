// 令和4年10月6日（2022年10月6日）
// 上間つよし＠沖縄県西原町＠久米島出身
// ATtiny85 1MHz 工場出荷時
//
//                 +      
//            +--||----------+                 
//            | 10uF         |           ATtiny85
//            +--|Diode>-----+      _________________
//            |              |   --|.Reset        VCC|--5[v]
//      0[v]--+-Relay--------+-----|PB3tiny       PB2|--
//     0[v]--[330]--LED(Sensor)----|PB4(ADC2)     PB1|--
//     0[v]------------------------|GND           PB0|--
//                                  -----------------
//
//                  基準電圧:内部1.1[v]
//                  ADC2の値によってLEDの点灯・消灯
//                  OPTION: Relay
//                                                     


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

	ADCSRA = ( 1 << ADEN  ) | ( 0b111  << ADPS0 );  /* VCC、 CK/128 */
	ADMUX  = ( 1 << ADLAR ) | ( 0b0010 << MUX0 );   /* 左詰め、ADC0( PB4 ) */

	DDRB |= 1 << PB3;



	sei();

	while(1)
	{
		DDRB  |=  ( 1 << PB4 );
		PORTB &= ~( 1 << PB4 );   /* discharge */
		asm( "nop" );
		asm( "nop" );
		asm( "nop" );
		asm( "nop" );
		asm( "nop" );
		asm( "nop" );
		asm( "nop" );
		asm( "nop" );
		asm( "nop" );
		asm( "nop" );

		DDRB  &=  ~( 1 << PB4 );
		PORTB &= ~( 1 << PB4 );   /* no pullup */

		ADCSRA |= 1 << ADIF;    /* 変換完了フラグクリア */
		ADCSRA |= 1 << ADSC;    /* 変換開始 */
		while( !( ADCSRA & ( 1 << ADIF ) ) ) ; /* 変換待ち */

		TCNT0 = 0;
		counter = 0;
		while( counter ) ;

		if( ADCH < 1 ) {
			DDRB  |= 1 << PB4 ;
			PORTB |= 1 << PB4 ;
			PORTB |= 1 << PB3 ;

			TCNT0 = 0;
			counter = 10;
			while( counter ) ;
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




