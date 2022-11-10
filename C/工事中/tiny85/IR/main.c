/* 赤外線受信・送信                                     */


// まだ虫食い状態です。



/*                                                      */
/* 上間つよし＠沖縄県西原町＠久米島出身                 */
/* 令和4年10月27日（2022年10月27日）                    */
/*                                                      */
/*                                                      */
/*  AVR ATtiny85  8[MHz]                                */
/*                     ----------                       */
/*                  --|Reset  VCC|--------------5[v]    */
/*  0[v]--[680]--LED--|PB3    PB2|--LED--[680]--0[v]    */
/*  IR----------------|PB4    PB1|--LED--[680]--0[v]    */
/*  0[v]--------------|GND    PB0|--LED--[680]--0[v]    */
/*                    ----------                        */
/*                      Tiny85                          */
/*                                                      */
/*                                                      */
/*           --                                         */
/*  0[v]----|  |                                        */
/*  IR------|  | IRモジュール                           */
/*  5[v]----|  |                                        */
/*           --                                         */
/*                                                      */
/*                                                      */
/*  IR:High = 5[v]                                      */
/*  IR:Low  = 0[v]                                      */
/*                                                      */
/*  IR: 待機状態の時　High=5[v]                         */
/*                                                      */
/*  state =  0b0001   IR=Hige をカウントアップスタート  */
/*  state =  0b0010   IR=Hige をカウントアップ中        */
/*  state =  0b0100   IR=Low  をカウントアップスタート  */
/*  state =  0b1000   IR=Low  をカウントアップ中        */
/*                                                      */
/*                                                      */
/*  state =  0b0001 の時、以前のLow  カウンター確定     */ 
/*  state =  0b0100 の時、以前のHigh カウンター確定     */
/*                                                      */
/*                                                      */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "soft_timer.h"
#include "sw.h"  /* インクルードするが今は使わない */

#define IR    ( ( PINB >> PB4 ) & 1 )
#define High      1 
#define Low       0 

volatile unsigned char f_200us = 0;  /* CTC interrupt 200[us] flag */
volatile unsigned char state   = 0;  /* IR:High,Low count up state */
unsigned char high_low_counter = 0;  /* High counter and/or Low counter */
unsigned char  samp_value[200];
unsigned char send_value[100];
unsigned int  samp_value_idx  = 0;

int  main( void );            /* Main関数 */
void initialize( void );      /* 一般初期化 */
void kaiseki( void );
ISR( TIMER0_COMPA_vect );     /* 比較一致A割込み関数 */

int  main( void )
{
	initialize();  /* 一般初期化 */
	t_ini();       /* ソフトタイマー初期化 */

	sei();

	while( 1 ) {
		
		t_main();

		if( t_tup( 1 ) ) {  /* Low->High:1[s]later */
			t_can( 1 );
			kaiseki();
		}

		while( f_200us < 5 ) ;  /* 200[us] x 5 = 1[ms] */
		f_200us = 0;
	}

	return 0;
}

void initialize( void )
{
	TCCR0A = 1 << WGM01;        /* CTC動作        */
	TCCR0B = 0b010 << CS00;     /* ck/8           */
	OCR0A  = 8000000/8/5000-1; /* 200[us]          */
	TIMSK  = 1 << OCIE0A;       /* 比較割込み許可 */
	TCNT0  = 0;                 /* タイマー＝０   */

	DDRB |= 0x0f;
	asm( "nop" );

	state = 0b1000; /* 最初IR==Low,カウントアップ中とする */
}

void kaiseki( void )
{
	;
}


ISR( TIMER0_COMPA_vect )
{
	static unsigned char mode = 0;  /* アイドル中 */
	f_200us++;

	if( IR == High ) {
		if( state == 0b0100 ) {
			samp_value[samp_value_idx++] = high_low_counter;
			high_low_counter = 0;
			state = 0b0001;
		}
		if( state == 0b0001 ) state = 0b0010;
		++high_low_counter;
	} else if( IR == Low ) {
		t_req( 1 );  /* １秒、IR休止中、リクエスト */
		if( state == 0b0001 ) {
			samp_value[samp_value_idx++] = high_low_counter;
			high_low_counter = 0;
			state = 0b0100;
		}
		if( state == 0b0100 ) state = 0b1000;
		++high_low_counter;
	}
/*  state =  0b0001   IR=Hige をカウントアップスタート  */
/*  state =  0b0010   IR=Hige をカウントアップ中        */
/*  state =  0b0100   IR=Low  をカウントアップスタート  */
/*  state =  0b1000   IR=Low  をカウントアップ中        */
/*                                                      */
/*                                                      */
/*  state =  0b0001 の時、以前のLow  カウンター確定     */ 
/*  state =  0b0100 の時、以前のHigh カウンター確定     */
}
