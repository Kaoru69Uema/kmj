//   圧電スピーカー（Sounder）
//
//////////////////////////////////////////////////////////////////
// ソルフェジオ６音
//      396Hz  マザー(原母)
//      417Hz   トランス(許容への酔い)
//      528Hz  セルフ(奇跡の輝き)
//      639Hz  アーティスト(高度な自己)
//      741Hz  クリアリング(清め)
//      852Hz  インスパイヤ(天の智)
//      
//      432Hz  リアルA(真の基音)
//      444Hz  蘇生(復活)
//      888Hz  741Hzと同じで今世とあの世の堺の周波数
//
//      153Hz  レリーフ(安寧) 今回は鳴らさない
//
//////////////////////////////////////////////////////////////////
///
//   令和4年10月10日（2022年10月10日）
//   上間つよし＠沖縄県西原町＠久米島出身
//   tiny85  クロック内蔵8[MHz]
//
//
//           Tiny85
//          ---------
//       --|Reset VCC|--5[v]
//       --|PB3   PB2|--
//       --|PB4   PB1|--
//   0[v]--|GND   PB0|--Sounder--[330]--5[v]
//          ---------
//
//   圧電スピーカー実験回路は上を使いました。
//   ホビー機器に組み込むときは下の参考回路で実現したほうが良いと思います。
//
//                       C     E
//   5[v]--+--Sounderr--+----     --------------+--------0[v]
//         +--[10k]-----+    \   /              |
//         +--Diode-----+     --- 2SC1815       |
//            (1S2076A)        |B               |
//                             +-----[56k]------+
//                             |
//                             +-----[4.7k]---------------PWM_Port
//
//
//
//
#include <avr/io.h>
#include <avr/interrupt.h>

#define buzer_on   (TCCR0B |=  0b01000011)
#define buzer_off  (TCCR0B &= ~0b00000011)
/* TCCR0B = 0b011 << CS00;  */             /* CK/1, 741+741[Hz] */

/* グローバル変数 */
volatile unsigned char tflag;

/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIMER0_COMPA_vect);

int main( void )
{
	initialize();
	sei();

	while( 1 ) {
		// 何もしない
		while( !tflag ) ; /* 1[ms]時間待ち      */
		tflag = 0;        /* フラグクリア       */
	}
	return 0;
}

#define Tone396 157  //  マザー(原母)
#define Tone417 149  // トランス(許容への酔い)
#define Tone528 117  // セルフ(奇跡の輝き)
#define Tone639 97   // アーティスト(高度な自己)
#define Tone741 83   // クリアリング(清め)
#define Tone852 72   //  インスパイヤ(天の智)
#define Tone432 143  // リアルA(真の基音)
#define Tone444 140  // 蘇生(復活)
#define Tone888 69   // インスパイヤ(天の智)2

void initialize( void )
{
	TCCR0A = (1 << COM0A0)|(1 << WGM01);      /* CTC-Timer         */
	TCCR0B |=  0b01000011;   /* toggle, ck/64 */
	//OCR0A  = Tone528;
	OCR0A  = Tone417;
	TIMSK  = 1 << OCIE0A;                      /* 比較一致割込み    */
	TCNT0=0;
	DDRB |= 1 << PB0;
}

ISR(TIMER0_COMPA_vect)
{
	tflag = 1;
}

