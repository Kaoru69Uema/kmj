//   
//   工事中
//
//   レスキュートーン
//
//
//   令和4年10月10日（2022年10月10日）
//   上間つよし＠沖縄県西原町＠久米島出身
//   tiny85  クロック内蔵8[MHz]
//
//   741[Hz]の音を発生させるため741+741=1482[Hz]:CTC-Timerを使う。
//
//   音のパターンは16bit-ON-OFF-変数で表現
//
//   PWM出力ピンを使わず、普通の出力ピンで行います。
//
//
//           Tiny85
//          ---------
//       --|Reset VCC|--5[v]
//       --|PB3   PB2|--
//       --|PB4   PB1|--
//   0[v]--|GND   PB0|--Buzer--[330]--5[v]
//          ---------
//
//   ブザー実験回路は上を使いました。
//   ホビー機器に組み込むときは下の参考回路で実現したほうが良いと思います。
//
//                       C     E
//   5[v]--+--Buzer--+----     --------------+--------0[v]
//         +--[10k]--+    \   /              |
//         +--Diode--+     --- 2SC1815       |
//            (1S2076A)     |B               |
//                          +-----[56k]------+
//                          |
//                          +-----[4.7k]---------------Port
//
//
//
#include <avr/io.h>
#include <avr/interrupt.h>

#define buzer_on  (TCCR0B = 0b01000001)
#define buzer_off (TCCR0B = 0b10000000)

/* グローバル変数 */
volatile unsigned char flag_1ms;

/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIMER0_COMPA_vect);

int main( void )
{
	initialize();
	sei();

	while( 1 ) {
		while( !flag_1ms ) ; /* 1[ms]時間待ち      */
		flag_1ms = 0;        /* フラグクリア       */
		PORTC = 0xff;
	}
	return 0;
}

void initialize( void )
{
	TCCR1A = 1 << WGM11;                    /* CTC-Timer         */
	TCCR1B = 0b01000001;                    /* CK/1, 741+741[Hz] */
	OCR1A  = 1000000 / ( 741+741 )  - 1;    /* 14821[Hz]         */
	TIMSK1  = 1 << OCIE1A;                  /* 比較一致割込み    */
	TCNT1=0;
	DDRB = 1 << PB1;
	DDRC = 0x00001111;
}

ISR(TIMER1_COMPA_vect)
{
	flag_1ms = 1;
}
