//
//   令和4年10月29日（2022年10月29日）
//   上間つよし＠沖縄県西原町＠久米島出身
//   tiny85  クロック内蔵8[MHz]
//
//   ヒューズを設定し直してください
//
//   
//
//   
//
//
//                     Tiny85
//                    ---------
//                 --|Reset VCC|--5[v]
// 0[v]--[330]--LED--|PB3   PB2|--
// 0[v]---------SW---|PB4   PB1|--
// 0[v]--------------|GND   PB0|--
//                    ---------
//
//
//
//

//  インクルードファイル
#include <avr/io.h>
#include <avr/interrupt.h>
#include "sw.h"

// グローバル変数など
#define SW     PB4
#define LED    PB3
volatile unsigned char f_1ms = 0;

void initialize( void );
int main( void );
ISR( TIM0_COMPA_vect );

// Main関数
int main( void )
{

	initialize();     /* 一般初期化     */
	sw_ini();         /* スイッチ初期化 */

	while( 1 ) {
		sw_main();
		if( sw_dn & ( 1 << SW ) )       /* 押した */
			PORTB |=  1 << LED;     /* オン */
		if( sw_up & ( 1  <<  SW ) )     /* 離した */
			PORTB &= ~( 1 << LED ); /* オフ */

		while( ! f_1ms ) ;
		f_1ms = 0;
	}
	return 0;
}

// 一般初期化
void initialize( void )
{
	TCCR0A  = 1 << WGM01;  /* CTC Timer0 */
	TCCR0B  = 0b011 << CS00; /* (ck/64) */
	OCR0A   = 8000000/64/1000-1; /* 1[ms] */
	TIMSK   = 1 << OCIE0A;   /* comp A inturrupt */
	TCNT0   = 0;  /* counter = 0 */
	DDRB  = 1 << LED;
	PORTB = 1 << SW;
}

ISR( TIM0_COMPA_vect )
{
	f_1ms = 1;
}
