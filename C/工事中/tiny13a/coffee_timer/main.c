/////////////////////////////////////////////////////////////////////////////
//
//   コーヒーメーカーOFFタイマー
//
//   コーヒーが煮詰まるのを防ぐためのOFFタイマー
//
/////////////////////////////////////////////////////////////////////////////
//
//   令和4年10月23日（2022年10月23日）
//
//   上間つよし＠沖縄県西原町＠久米島出身
//
//   tiny13A  クロック内蔵 1.2[MHz]
//
/////////////////////////////////////////////////////////////////////////////
//
//   使い方：
//   
//   ボタン１プッシュ：スタートボタン、停止ボタン
//   ボタン５ブッシュ：OFFタイマー設定値モードへ
//   　　　　　　　　：続く、１プッシュの回数で値を設定
//   　　　　　　　　：５秒経過で、設定完了
//
/////////////////////////////////////////////////////////////////////////////
//
//   回路図
//
//                         C     E
//   5[v]--+--Relay----+----     --------------+--------0[v]
//         +--[10k]----+    \   /              |
//         +--Diode----+     --- 2SC1815       |
//            (1S2076A)       |B               |
//                            +-----[56k]------+
//                            |
//     +--------[4.7k]--------+                  0.01uF
//     |                                  +----||--------0[v]
//     |               --------------     |
//     |              |Reset      VCC|----+--------------5[v]
//     +--------------|PB3        PB2|---LED---[1K]------0[v]  ( POWER )
//     0[v]------SW---|PB4        PB1|---LED---[1K]------0[v]  ( RUN   )
//     0[v]-----------|GND        PB0|------------------------+
//                     --------------                         |
//                         Tiny13A                            |
//                                                            |
//                         C     E                            |
//   5[v]--+--Sounder--+----     --------------+--------0[v]  |
//         +--[10k]----+    \   /              |              |
//         +--Diode----+     --- 2SC1815       |              |
//            (1S2076A)       |B               |              |
//                            +-----[56k]------+              |
//                            |                               |
//                            |                               |
//                            |                               |
//                            +-----------[4.7k]--------------+
//
//
/////////////////////////////////////////////////////////////////////////////

/****************************************************************/
/*		START						*/
/****************************************************************/

/* ヘッダファイル */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "sw.h"
#include "soft_timer.h"

/* グローバル変数 など */
volatile unsigned char f_1ms = 0;        /* Timer0: 1[ms] フラグ */
static   unsigned char EEMEM EE_TIME;  /* EE_TIME OFF時間        */
unsigned long TIME;                      /* Coffee OFF Timer OFF時間 */

#define SOUNDER        PB0
#define LED_RUN        PB1
#define LED_POWER      PB2
#define RELAY          PB3
#define SW             PB4

#define ON    1
#define OFF   0


/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIM0_COMPA_vect);

/* Main関数 */
int main( void )
{

	unsigned char sw_push  = 0;        /* SWプッシュ回数 */
	unsigned char set_mode = 0;        /* 設定中フラグ */
	unsigned char set_mode_cntr = 0;        /* カウンター */

	initialize();  /* 初期化： 一般                 */
	t_ini();       /* 初期化： ソフトタイマー       */
	sw_ini();      /* 初期化： ３回照合スイッチ入力 */

	sei();

	while( 1 ) {
		sw_main();
		t_main();

		if( sw_dn & ( 1<<SW ) ) {
			sw_push++;
			t_req( 0 );
		}

		if( t_tup( 0 ) ) {
			if( ( sw_push == 1 ) && ( t_run( 1 ) ) ) {   /* カウントダウン中 && １プッシュ  */
				t_can( 1 );                          /* OFF_TIMER_時間キャンセル        */
			} else if( ( sw_push == 1 ) && ( t_off( 1 ) ) ) {   /* 休止中 && １プッシュ     */
				t_set( 1, TIME );                           /* OFF_TIMER_時間リクエスト */
			} else if( ( sw_push == 1 ) && ( set_mode == ON ) ) {   /* 設定中 && １プッシュ */
				++TIME;
				eeprom_busy_wait();
				eeprom_write_byte(&EE_TIME, TIME );
				t_req( 2 );
			}

			if( ( sw_push == 5 ) && ( set_mode == OFF ) ) {  /* 設定モードへ */
				set_mode = ON;
				t_req( 2 );  /* 設定モードからの自動復帰タイマーリクエスト */
				TIME = 0;
			}

			sw_push = 0;
			t_can( 0 );
		}

		if( t_run( 1 ) ) PORTB |= 1<<RELAY;     /* カウントダウン中 */
		else        PORTB &= ~( 1<<RELAY );     /* 休止中           */


		/*****/if( t_counter[ 1 ] > ( TIME * ( 3/4 ) ) ) {
			PORTB |= 1<<LED_RUN;                       /* LED_RUN = ON */
			if( t_counter[ 1 ] > ( TIME * ( 7/8 ) ) )
				PORTB &= ~( 1<<LED_RUN );           /* LED_RUN =OFF */
		} else if( t_counter[ 1 ] > ( TIME * ( 2/4 ) ) ) {
			PORTB |= 1<<LED_RUN;                       /* LED_RUN = ON */
			if( t_counter[ 1 ] > ( TIME * ( 5/8 ) ) )
				PORTB &= ~( 1<<LED_RUN );           /* LED_RUN =OFF */
		} else if( t_counter[ 1 ] > ( TIME* ( 1/4 ) ) ) {
			PORTB |= 1<<LED_RUN;                       /* LED_RUN = ON */
			if( t_counter[ 1 ] > ( TIME * ( 2/8 ) ) )
				PORTB &= ~( 1<<LED_RUN );           /* LED_RUN =OFF */
		} else {

		}

		if( set_mode ) {
			PORTB |= 1<<LED_RUN;
			if( ++set_mode_cntr > 127 ) PORTB &= ~( LED_RUN );
		}

		if( t_tup( 2 ) ) {
			t_can( 2 );
			set_mode = 0;
		}
		

		while( ! f_1ms ) ;    /* 1[ms] 待機     */
		f_1ms = 0;            /* フラグリセット */
	}
	return 0;
}

void initialize( void )
{
	TCCR0A  = 1 << WGM01;          /* Timer:CTC:1[ms] */
	TCCR0B  = 0b010 << CS00;       /* CK/8            */
	OCR0A   = 1200000/8/1000-1;    /* 1/1000=1[ms]    */
	TCNT0   = 0;                   /* カウンタ=0      */
	TIMSK0  = 1 << OCIE0A;         /* 割込みセット    */

	DDRB  = ( 1 << SOUNDER   ) | 
		   ( 1 << LED_RUN   ) | 
	  	     ( 1 << LED_POWER ) | 
		       ( 1 << RELAY );
	PORTB = 1 << SW;    /* Pull_UP */
	eeprom_busy_wait();
	TIME    = eeprom_read_byte( &EE_TIME );
}

ISR(TIM0_COMPA_vect)
{
	f_1ms = 1;
}


/****************************************************************/
/*		END						*/
/****************************************************************/






#if 0
#include <avr/eeprom.h>
static unsigned char EEMEM EE_TIME;            /* EE_TIME OFF時間 */
	eeprom_busy_wait();
	//TIME    = eeprom_read_byte( &EE_TIME );


//      static unsigned char EEMEM EE_TIME;            /* EE_TIME OFF時間 */
//	eeprom_busy_wait();
//	briteT1=eeprom_read_byte( &EE_TIME );
//	eeprom_busy_wait();
//	briteT2=eeprom_read_byte( &EEPROM_V2 );
//	eeprom_write_byte(&EE_TIME, briteT1 );
//	eeprom_write_byte(&EEPROM_V2, briteT1 );
#endif

