//   令和4年10月8日（2022年10月8日）
//   上間つよし＠沖縄県西原町＠久米島出身
//   ソフトタイマ
//
//          Tiny85
//        ----------
//     --|Reset  VCC|--VCC
//     --|PB3    PB2|--
//     --|PB3    PB1|--LED--[330]--GND
//  GND--|GND    PB0|--
//        ----------
//
//
//
//
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include "soft_timer.h"

/* グローバル変数 */
volatile unsigned char flag_1ms;

/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIM0_COMPA_vect);

int main( void )
{
	initialize();
	sei();

	/* [1000,5000,10000,60000] */

	
	t_ini();     /* soft_timer 初期化   */
	t_req( 0 );  /* 1000 x 0.001 = 1[s] */

	while( 1 ) {
		t_main();            /* ソフトタイマメイン */

		while( !flag_1ms ) ; /* 1[ms]時間待ち      */
		flag_1ms = 0;        /* フラグクリア       */

		if( t_tup( 0 ) ) {
			t_set( 0, 1000 );
			//t_req( 0 );
			PORTB ^= 1 << PB1;
		}
	}
	return 0;
}

void initialize( void )
{
	TCCR0A = 1 << WGM01;             /* CTC-Timer      */
	TCCR0B = 1 << CS01;              /* CK/8, 1000[Hz] */
	OCR0A  = 1000000 / 8 / 1000 - 1; /* T = 1[ms]      */
	TIMSK  = 1 << OCIE0A;            /* 比較一致割込み */
	TCNT0=0;
	DDRB = 1 << PB1;
}

ISR(TIM0_COMPA_vect)
{
	flag_1ms = 1;
}
