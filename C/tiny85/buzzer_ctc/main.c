//   ブザー
//
//   令和4年10月13日（2022年10月13日）
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
//           Tiny85
//          ---------
//       --|Reset VCC|--5[v]
//       --|PB3   PB2|--
//       --|PB4   PB1|--
//   0[v]--|GND   PB0|--Buzer--[330]--5[v]
//          ---------
//
//   実験回路は上を使いました。
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

#if 0

/* 周波数生成定数 */

0: 396Hz　マザー(原母)              CK/64     157
1: 417Hz 　トランス(許容への酔い)   CK/64     149
2: 528Hz　セルフ(奇跡の輝き)        CK/64     117
3: 639Hz　アーティスト(高度な自己)  CK/64      97
4: 741Hz　クリアリング(清め)        CK/64      83
5: 852Hz　インスパイヤ(天の智)      CK/64      72
6: 888Hz                            CK/64      69
7: 432Hz　リアルA(真の基音)         CK/64     144
8: 444Hz　蘇生(復活)                CK/64     140
9: 153Hz　レリーフ(安寧)            CK/256    243

#endif


/* グローバル変数 */
volatile unsigned int counter = 0;
volatile unsigned char on_off = 0;


/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIM0_COMPA_vect);
void Tone396( void );
void Tone417( void );
void Tone528( void );
void Tone639( void );
void Tone741( void );
void Tone852( void );
void Tone888( void );
void Tone432( void );
void Tone444( void );
void Tone153( void );


int main( void )
{

	initialize();
	sei();

	while( 1 ) {
	}
	return 0;
}

void initialize( void )
{
	TCCR0A = ( 1 << COM0A0 ) | ( 1 << WGM01 );
        Tone417();
	TIMSK  = 1 << OCIE0A;
	TCNT0=0;
	DDRB = 1 << PB0;
}

ISR(TIM0_COMPA_vect)
{
}


void Tone396( void ) { TCCR0B = 0b011 << CS00;  OCR0A =  157; }
void Tone417( void ) { TCCR0B = 0b011 << CS00;  OCR0A =  149; }
void Tone528( void ) { TCCR0B = 0b011 << CS00;  OCR0A =  117; }
void Tone639( void ) { TCCR0B = 0b011 << CS00;  OCR0A =   97; }
void Tone741( void ) { TCCR0B = 0b011 << CS00;  OCR0A =   83; }
void Tone852( void ) { TCCR0B = 0b011 << CS00;  OCR0A =   72; }
void Tone888( void ) { TCCR0B = 0b011 << CS00;  OCR0A =   69; }
void Tone432( void ) { TCCR0B = 0b011 << CS00;  OCR0A =  144; }
void Tone444( void ) { TCCR0B = 0b011 << CS00;  OCR0A =  140; }
void Tone153( void ) { TCCR0B = 0b111 << CS00;  OCR0A =  243; }
