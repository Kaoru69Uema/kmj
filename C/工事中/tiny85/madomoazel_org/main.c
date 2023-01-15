/////////////////////////////////////////////////////////////////////////////
//
//   圧電スピーカー 
//
//   市販のレスキューCDの周波数を圧電スピーカーで出す。
//
//   令和4年10月19日（2022年10月19日） 
//
//   上間つよし＠沖縄県西原町＠久米島出身
//
//   tiny85  クロック内蔵8[MHz]
//   ( Tiny85  AVRマイコン F_CPU = 8000000UL [Hz] )
//
//   ヒューズを設定し直してください
//   
//   タイマークロック：内蔵クロック PLL ６４MHｚを選択
//
//   
/////////////////////////////////////////////////////////////////////////////
//
//   使い方：
//   
//   電源投入時：LED_ON、圧電スピーカーOFF
//
//   １クリック：Tone0--Tone1--...--Tone10--..--Tone0... 繰り返し。。。
//
//   ２クリック：現在値が、Tone0の場合
//   　　　　　　　　Tone1--...--Tone10--..--Tone1... 繰り返し。。。
//               現在値が、Tone0以外の場合
//   　　　　　　　　現在値Toneから.順番に... 繰り返し。。。
//   
//   ３クリック：電源投入時に戻す（リセット）
//
//   ＊備考：　トーンとSW音は圧電スピーカーを共用
//   ＊備考：　トーンはTimer1CTC:Portトグル、SW音はPort入りきり
//
//ソルフェジオ６音
//     Tone0---  0Hz    OFF                      
//     Tone1---  396Hz　マザー(原母)             
//     Tone2---  417Hz 　トランス(許容への酔い)、細胞の修復
//     Tone3---  528Hz　セルフ(奇跡の輝き)       
//     Tone4---  639Hz　アーティスト(高度な自己) 
//     Tone5---  741Hz　クリアリング(清め)       
//     Tone6---  852Hz　インスパイヤ(天の智)     
//     Tone7---  888Hz  852Hzと同じ？                         
//     Tone8---  432Hz　リアルA(真の基音)        
//     Tone9---  444Hz　蘇生(復活)               
//     Tone10--- 153Hz　レリーフ(安寧),忘れる
//     Tone11--- 2160Hz 疲れを感じにくい
//
//   
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








/* ヘッダファイル */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "sw.h"
#include "soft_timer.h"

/* グローバル変数 */
volatile unsigned char f_1ms     = 0;
volatile unsigned int  sound_ptn = 0;
volatile unsigned char sound_on  = 0;
volatile unsigned char tone_no;  /* initializeで設定 */
	 unsigned char sw_click  = 0;
	 unsigned char repeat;  /* トーン繰り返し *//* initializeで設定 */
	 unsigned char LED_ON   = 1;  /*  */
static unsigned char EEMEM EEPROM_V1;  /* EEPROM_V1 　を宣言 */


/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR(TIM0_COMPA_vect);
ISR(TIM1_COMPA_vect);

void Tone0( void ); /* 0Hz    OFF                      */
void Tone1( void ); /* 396Hz　マザー(原母)             */
void Tone2( void ); /* 417Hz 　トランス(許容への酔い)  */
void Tone3( void ); /* 528Hz　セルフ(奇跡の輝き)       */
void Tone4( void ); /* 639Hz　アーティスト(高度な自己) */
void Tone5( void ); /* 741Hz　クリアリング(清め)       */
void Tone6( void ); /* 852Hz　インスパイヤ(天の智)     */
void Tone7( void ); /* 888Hz  852より天に近い          */
void Tone8( void ); /* 432Hz　リアルA(真の基音)        */
void Tone9( void ); /* 444Hz　蘇生(復活)               */
void Tone10( void );/* 153Hz　レリーフ(安寧) 忘れる    */
void Tone11( void );/* 2160Hz 疲労感を感じにくい       */

void sound_main( void );
void tone_change( void );


int main( void )
{

	initialize();  /* 初期化： 一般                 */
	t_ini();       /* 初期化： ソフトタイマー       */
	sw_ini();      /* 初期化： ２回照合スイッチ入力 */
	eeprom_busy_wait();
	LED_ON=eeprom_read_byte( &EEPROM_V1 );
	//eeprom_write_byte(&EEPROM_V1, LED_ON );

	sei();

	while( 1 ) {
		sw_main();
		t_main();
		sound_main();


//   電源投入時：LED_ON、圧電スピーカーOFF
//
//   １クリック：Tone0--Tone1--...--Tone10--..--Tone0... 繰り返し。。。
//
//   ２クリック：現在値が、Tone0の場合
//   　　　　　　　　Tone1--...--Tone10--..--Tone1... 繰り返し。。。
//               現在値が、Tone0以外の場合
//   　　　　　　　　現在値Toneから.順番に... 繰り返し。。。
//   
//   ３クリック：電源投入時に戻す（リセット）
//
		if( sw_dn & ( 1<<PB4 ) ) {
			sound_ptn = 0b1000000000000000;
			//if( ( ! t_tup( 0 ) ) && ( sw_click == 0 ) ) t_req( 0 );
			if( ( ! t_tup( 0 ) ) ) t_req( 0 );
			sw_click++;
		}


		if( t_tup( 0 ) ) {
			if( ( sw_click == 1 ) ) {
				repeat = 0;
				if( ++tone_no > 11 ) tone_no = 0;
				if( tone_no == 0 ) sound_ptn = 0b1010001111000000;
			} else if( ( sw_click == 2 )  ) {
				repeat = 1;
				t_req( 1 );
				sound_ptn = 0b1010101010101010;
				if( tone_no == 0 ) tone_no = 1;
			} else if( ( sw_click == 3 ) ) {
				repeat = 0;
				tone_no = 0;
				sound_ptn = 0b1111000000110000;
			} else if( ( sw_click == 5 ) ) {
				sound_ptn = 0b1000000000000001;
				if( LED_ON )
					LED_ON = 0;
				else
					LED_ON = 1;
				eeprom_busy_wait();
				eeprom_write_byte(&EEPROM_V1, LED_ON );
			}
			sw_click = 0;
			t_can( 0 );
			tone_change();
		}


		if( repeat == 1 ) {
			if( t_tup( 1 ) ) {
				t_req( 1 );
				if( ++tone_no > 10 ) tone_no = 1;
			}
		}


		if( LED_ON )  PORTB |= 1<<PB3;
		else          PORTB &= ~( 1<<PB3 );

		while( f_1ms < 5 ) ; /* 0.2 x 5 = 1[ms] 待機     */
		f_1ms = 0;            /* フラグリセット */
	}
	return 0;
}

void initialize( void )
{
//   電源投入時：LED_ON、圧電スピーカーOFF
//   Timer0:CTC動作: main() を0.2[ms] x 5 = 1[ms] で実行
//   Timer1:CTC動作: クロックを内部 PLL=64[MHz] を選択：圧電スピーカー
	TCCR0A = 1 << WGM01;
	TCCR0B = 0b011 << CS00;  /* 8000000/64 */
	OCR0A  = 8000000/64/5000-1; /* 24 */
	TCNT0=0;
	DDRB = ( 1 << PB3 ) | ( 1<<PB1 )  ;  /* LED, 圧電スピーカー */
	asm( "nop" );
	PORTB |= 1<<PB4;   /* タクトスイッチ：プルアップ */

	/* タイマークロック：PLL == 64[MHz] へ切り替え */
	PLLCSR |= 1<<PLLE;                     /* PLL許可             */
	_delay_us( 100 );                      /*100[us]待機          */
	while( !( PLLCSR & ( 1<<PLOCK ) ) ) ;  /* PLOCK == 1 まで待機 */
	PLLCSR |= 1<<PCKE;
	asm( "nop" );
	Tone0();
	/* タイマークロック：PLL == 64[MHz] へ切り替え: ここまで */

	TIMSK  = ( 1 << OCIE0A ) | ( 1<<OCIE1A );
	t_req( 1  ); /* トーン繰り返し、タイマー時間リクエスト */
	repeat  = 1;  /* トーン繰り返し */
	tone_no = 1;  /* 最初のトーン選択 */
}

void sound_main( void )
{
	/* 50[ms] 周期で実行 */
	static unsigned char count = 50;

	if( --count == 0 ) {
		count = 50;
		if( sound_ptn > 0 ) {
			Tone0();
			sound_on = ( sound_ptn & 0x8000 ) ? 1 : 0;
			sound_ptn <<= 1;
		} else {
			sound_on = 0;
			tone_change();
		}

	}
}


ISR(TIM0_COMPA_vect)
{
	++f_1ms;
	if( sound_on == 1 ) {
		PORTB ^= 1<<PB1;
		PORTB |= 1<<PB3;
	} else {
		PORTB &= ~( 1<<PB3 );
	}
}
ISR(TIM1_COMPA_vect)
{
}


void tone_change( void )
{

	switch( tone_no ) {
		case 0:
			Tone0();
			break;
		case 1:
			Tone1();
			break;
		case 2:
			Tone2();
			break;
		case 3:
			Tone3();
			break;
		case 4:
			Tone4();
			break;
		case 5:
			Tone5();
			break;
		case 6:
			Tone6();
			break;
		case 7:
			Tone7();
			break;
		case 8:
			Tone8();
			break;
		case 9:
			Tone9();
			break;
		case 10:
			Tone10();
			break;
		case 11:
			Tone11();
			break;
		default:
			break;
	};
}


#define DIV_OFF    0b0000
#define DIV_2      0b0001
#define DIV_4      0b0011
#define DIV_8      0b0100
#define DIV_16     0b0101
#define DIV_32     0b0110
#define DIV_64     0b0111
#define DIV_128    0b1000
#define DIV_256    0b1001
#define DIV_512    0b1010
#define DIV_1024   0b1011
#define DIV_2048   0b1100
#define DIV_4096   0b1101
#define DIV_8192   0b1110
#define DIV_16384  0b1111

/* トーン関数 */
/*  0[Hz] OFF  */
void Tone0( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 0<<COM1A0 ) | ( 0b0000<<CS10 );
	OCR1C = 0;
}

/*  396[Hz]  */
void Tone1( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_512; 
	OCR1C = 157;
}

/*  417[Hz]  */
void Tone2( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_512;
	OCR1C = 149;
}

/*  528[Hz]  */
//     Tone3---  528Hz　セルフ(奇跡の輝き)       
void Tone3( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_256;
	OCR1C = 236;
}

/*  639[Hz]  */
void Tone4( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_256;
	OCR1C = 195;
}

/*  741[Hz]  */
void Tone5( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_256;
	OCR1C = 168;
}

/*  852[Hz]  */
void Tone6( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_256;
	OCR1C = 146;
}

/*  888[Hz]  */
void Tone7( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_256;
	OCR1C = 140;
}

/*  432[Hz]  */
void Tone8( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_256;
	OCR1C = 144;
}

/*  444[Hz]  */
void Tone9( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_512;
	OCR1C = 140;
}

/*  153[Hz]  */
void Tone10( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_1024;
	OCR1C = 203;
}

/*  2160[Hz]  */
void Tone11( void ) {
	TCCR1 = ( 1<<CTC1 ) | ( 1<<COM1A0 ) | DIV_64;
	OCR1C = 230;
}


