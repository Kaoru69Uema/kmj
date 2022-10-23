/////////////////////////////////////////////////////////////////////////////
//
//   圧電スピーカー + タクトスイッチ +　LED
//
//   令和4年10月13日（2022年10月13日）
//   上間つよし＠沖縄県西原町＠久米島出身
//   tiny85  クロック内蔵8[MHz]
//
//   ヒューズを設定し直してください
//
//   
/////////////////////////////////////////////////////////////////////////////
//
//   使い方：
//
//   スイッチをダブルプッシュした後、１プッシュでトーンを切り替えていきます。
//   トーン１〜N番をLEDの点滅回数で表します。
//
//   スイッチ１プッシュでON:　LED＿OFF
//   また、１プッシュでOFF:　LED_N回＿点滅
//
/////////////////////////////////////////////////////////////////////////////
//
//                           Tiny85
//                          ---------
//                       --|Reset VCC|--5[v]
//       0[v]--[330]--LED--|PB3   PB2|--
//       0[v]-------SW-----|PB4   PB1|--
//       0[v]--------------|GND   PB0|--Sounder--[330]--5[v]
//                          ---------
//
//                                      PWM-PORT : PB0
//
//   実験回路は上を使いました。
//
/////////////////////////////////////////////////////////////////////////////
//
//   ホビー機器に組み込むとき
//   圧電スピーカー回路は下を参考に組んだ方が良いと思います。
//
//                         C     E
//   5[v]--+--Sounder--+----     --------------+--------0[v]
//         +--[10k]----+    \   /              |
//         +--Diode----+     --- 2SC1815       |
//            (1S2076A)       |B               |
//                            +-----[56k]------+
//                            |
//                            +-----[4.7k]---------------Port
//
//
/////////////////////////////////////////////////////////////////////////////



#if 0

Tiny85  AVRマイコン F_CPU = 8000000UL [Hz]

/* 周波数生成定数 */

0: 396Hz　マザー(原母)
1: 417Hz 　トランス(許容への酔い)
2: 528Hz　セルフ(奇跡の輝き)
3: 639Hz　アーティスト(高度な自己)
4: 741Hz　クリアリング(清め)
5: 852Hz　インスパイヤ(天の智)
6: 888Hz  
7: 432Hz　リアルA(真の基音)
8: 444Hz　蘇生(復活)
9: 153Hz　レリーフ(安寧)












#endif

/* ヘッダファイル */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "sw.h"
#include "soft_timer.h"

/* グローバル変数 */
volatile unsigned char f_1ms = 0;
volatile unsigned int  sound_ptn = 0;
volatile unsigned char sound_on = 0;
volatile unsigned char tone_no = 0;


/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIM0_COMPA_vect);
ISR(TIM1_COMPA_vect);

void Tone000( void );
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
void Tone2000( void );
void Tone4000( void );

void sound_main( void );
void tone_change( unsigned char x );


int main( void )
{
	unsigned char sw_click = 0;
	unsigned char mode = 0;

	initialize();  /* 初期化 */
	t_ini();       /* 初期化 */
	sw_ini();      /* 初期化 */

	sei();

	while( 1 ) {
		sw_main();
		t_main();
		sound_main();


		if( sw_dn & ( 1<<PB4 ) ) {
			sound_ptn = 0b1000000000000000;
			if( ! t_tup( 0 ) ) t_req( 0 );
			sw_click++;
		}

		if( t_tup( 0 ) ) {
			if( sw_click == 1 ) {
				if( mode >=  1 ) {
					tone_no = 1;
					mode = 0;
				} else {
					if( ++tone_no > 10 ) tone_no = 0;
					tone_change( tone_no );
				}
				if( tone_no == 0 ) sound_ptn = 0b1010101000000000;
			}

			if( sw_click == 2 ) {
				mode = 1;
				tone_no = 1;
				t_req( 1 );
				sound_ptn = 0b1010101010101011;
			}
			if( sw_click == 3 ) {
				mode = 2;
				tone_no = 0;
				sound_ptn = 0b1111111100010101;
			}

			sw_click = 0;
			t_can( 0 );
		}


		if( mode == 1 ) {
			if( t_tup( 1 ) ) {
				t_req( 1 );
				if( ++tone_no > 10 ) tone_no = 1;
				tone_change( tone_no );
			}
		}


		if( tone_no ) PORTB |= 1<<PB3;




		while( f_1ms < 5 ) ; /* 0.2 x 5 = 1[ms] 待機     */
		f_1ms = 0;            /* フラグリセット */
	}
	return 0;
}

void initialize( void )
{
	TCCR0A = 1 << WGM01;
	TCCR0B = 0b011 << CS00;  /* 8000000/64 */
	OCR0A  = 8000000/64/5000-1; /* 124 */
	TCNT0=0;
	DDRB = ( 1 << PB3 ) | ( 1<<PB1 )  ;  /* LED */
	asm( "nop" );
	PORTB |= 1<<PB4;

	PLLCSR |= 1<<PLLE;                     /* PLL許可             */
	_delay_us( 100 );                      /*100[us]待機          */
	while( !( PLLCSR & ( 1<<PLOCK ) ) ) ;  /* PLOCK == 1 まで待機 */
	PLLCSR |= 1<<PCKE;
	asm( "nop" );
	Tone000();

	TIMSK  = ( 1 << OCIE0A ) | ( 1<<OCIE1A );
}

void sound_main( void )
{
	static unsigned char count = 50;
	static unsigned char temp;

	if( --count == 0 ) {
		count = 50;
		if( sound_ptn > 0 ) {
			Tone000();
			sound_on = ( sound_ptn & 0x8000 ) ? 1 : 0;
			sound_ptn <<= 1;
		} else {
			sound_on = 0;
			tone_change( tone_no );
		}

	}
}


ISR(TIM0_COMPA_vect)
{
	++f_1ms;
	if( sound_on == 1 ) {
		PORTB ^= 1<<PB1;
		PORTB &= ~( 1<<PB3 );
	} else {
		PORTB |= 1<<PB3;
	}
}
ISR(TIM1_COMPA_vect)
{
}


void tone_change( unsigned char tone )
{

	switch( tone ) {
		case 0:
			Tone000();
			break;
		case 1:
			Tone396();
			break;
		case 2:
			Tone417();
			break;
		case 3:
			Tone528();
			break;
		case 4:
			Tone639();
			break;
		case 5:
			Tone741();
			break;
		case 6:
			Tone852();
			break;
		case 7:
			Tone888();
			break;
		case 8:
			Tone432();
			break;
		case 9:
			Tone444();
			break;
		case 10:
			Tone153();
			break;
		default:
			break;
	};
}


/* トーン関数 */
void Tone000( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 0<<COM1A0 ) | ( 0b0000<<CS10 );
	OCR1C = 0;
}
void Tone396( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1011<<CS10 );
	OCR1C = 78;
}

void Tone417( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1011<<CS10 );
	OCR1C = 74;
}

void Tone528( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1011<<CS10 );
	OCR1C = 58;
}

void Tone639( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1011<<CS10 );
	OCR1C = 48;
}

void Tone741( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1011<<CS10 );
	OCR1C = 41;
}

void Tone852( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1101<<CS10 );
	OCR1C = 8;
}

void Tone888( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1011<<CS10 );
	OCR1C = 34;
}

void Tone432( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1101<<CS10 );
	OCR1C = 17;
}

void Tone444( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1100<<CS10 );
	OCR1C = 34;
}

void Tone153( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1101<<CS10 );
	OCR1C = 50;
}

void Tone2000( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1000<<CS10 );
	OCR1C = 124;
}

void Tone4000( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | ( 0b1100<<CS10 );
	OCR1C = 3;
}

