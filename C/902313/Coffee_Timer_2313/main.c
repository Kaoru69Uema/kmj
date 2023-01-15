/////////////////////////////////////////////////////////////////////////////
//
//   コーヒーメーカーOFFタイマー
//
//   コーヒーが煮詰まるのを防ぐためのOFFタイマー
//
/////////////////////////////////////////////////////////////////////////////
//
//   令和4年11月9日（2022年11月9日）
//
//   上間つよし＠沖縄県西原町＠久米島出身
//
//   AT90S2313  クロック 10[MHz]
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
//     +--------[4.7k]--------+                  0.1uF
//     |                                     +----||--------0[v]
//     |                  --------------     |
//     |                 |Reset      VCC|----+--------------5[v]
//     +-----------------|PD0        PB7|--
//                 --  --|PD1        PB6|--
//     0[v]--||---|10|---|XTAL2      PB5|--                        
//     0[v]----||-|M |---|XTAL1      PB4|---LED---[1K]------0[v]  ( POWER )                       
//          22px2  --  --|PD2        PB3|--                       
//                     --|PD3        PB2|---LED---[1K]------0[v]  ( RUN   )                       
//     0[v]----SW--------|PD4        PB1|--                       
//                     --|PD5        PB0|---------------------+
//     0[v]--------------|GND        PD6|--                   |
//                        ==============                      |
//                          AT90S2313                         |
//                                                            |
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
static   unsigned char EEMEM EE_TIME;    /* EE_TIME OFF時間        */
unsigned long TIME;                      /* Coffee OFF Timer OFF時間 */

#define SOUNDER        PB0
#define LED_RUN        PB2
#define LED_POWER      PB4
#define RELAY          PD0
#define SW             PD4

#define ON    1
#define OFF   0


/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIMER1_COMP1_vect);

/* Main関数 */
int main( void )
{

	unsigned char sw_push  = 0;             /* SWプッシュ回数 */
	unsigned char set_mode = 0;             /* 設定中フラグ   */
	unsigned char set_mode_cntr = 0;        /* カウンター     */

	initialize();  /* 初期化： 一般                 */
	t_ini();       /* 初期化： ソフトタイマー       */
	sw_ini();      /* 初期化： ３回照合スイッチ入力 */

	sei();

	while( 1 ) {
		sw_main();
		t_main();

		if( sw_dn & ( 1<<SW ) ) {
		}

		if( t_tup( 0 ) ) {
		}


		while( ! f_1ms ) ;    /* 1[ms] 待機     */
		f_1ms = 0;            /* フラグリセット */
	}
	return 0;
}

void initialize( void )
{
	eeprom_busy_wait();
	TIME    = eeprom_read_byte( &EE_TIME );
}

ISR(TIMER1_COMP1_vect)
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

