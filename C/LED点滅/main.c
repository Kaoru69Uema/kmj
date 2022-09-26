// 上間つよし＠沖縄県西原町＠久米島出身 令和4年9月26日

#define F_CPU 1000000UL

// ヘッダファイル
#include <avr/io.h>
#include <util/delay.h>

// グローバル定義・変数
#define EXIT_SUCCESS 0
#define True  1
#define False 0

int main( void )
{

	DDRB=1<<PB0;                // PB0出力
	while( True ) {             // 無限ループ
		PORTB ^= 1<<PB0;    // PB0(LED)点滅
		_delay_ms( 500 );   // 500[ms]待機
	}
	return EXIT_SUCCESS;
}

// 回路図
//                    --------
//            Reset--|        |--
//                 --|        |--
//                 --|        |--
//                 --|        |--
//                 --| AVR    |--
//                 --| ATmega |--
//              VCC--| 328p   |--GND--||--+   0.1[uF]
//              GND--|        |--         |
//                 --|        |--VCC------+
//                 --|        |--SCK
//                 --|        |--MISO
//                 --|        |--MOSI
//                 --|        |--
// GND--[330]--LED --|        |--
//                    --------
