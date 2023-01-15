#if 0
                                      +--||-----------0[v]
                    .---------        |  0.1[uF]
  22[pF]x2      -- |~RST   VCC|-------+---------------5[v]
            _   -- | PD0   PB7|--
  0[v]--||-|1|  -- | PD1   PB6|--
           |0|---- | XTAL2 PB5|--      +--[Diode>--+
  0[v]--||-|M|---- | XTAL1 PB4|--      +--[10K]----+
            -   -- | PD2   PB3|--[2k]--+--Sounder--+--5[v]
                -- | PD3   PB2|--
                -- | PD4   PB1|--
5[v]--[680]--LED-- | PD5   PB0|--
                -- | GND   PD6|------SW---------------0[v]
                    ----------
                    AVR AT90S2313

     音発生器：空気清浄機：市販の「サウンド・レスキューCD」の周波数
     tone[]={ 周波数、、、、、]  コード参照

#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "sw.h"
#include "soft_timer.h"

#define LED       PD5
#define Sounder   PB3
#define SW        PD6

unsigned int tone[] = { 0, 396, 417, 528, 639, 741, 852, 888, 432, 444, 153, 2160, 2160*2 };
volatile unsigned char tone_no;
volatile unsigned char f_1ms;
unsigned char click;
unsigned int sound_ptn;
unsigned char sound_on;
unsigned char sound_cnt = 50;
unsigned char led_x = 1;
static unsigned char EEMEM EEPROM_LED;  /* EEPROM_LED 　を宣言 */

//====================================================
int main( void );
void initialize( void );
void led_on( void );
void led_off( void );
void led_toggle( void );
void def_tone( unsigned int hz );
void sound_main( void );
ISR( TIMER0_OVF0_vect );
//====================================================

//   ソルフェジオ６音
//   396Hzマザー(原母)
//   417Hzトランス(許容への酔い)
//   528Hzセルフ(奇跡の輝き)
//   639Hzアーティスト(高度な自己)
//   741Hzクリアリング(清め)
//   852Hzインスパイヤ(天の智)
//   888Hz マドモアゼル・愛さん、Youtubeで言及。741Hzと同じ性質
//   432HzリアルA(真の基音)
//   444Hz蘇生(復活)
//   153Hzレリーフ(安寧)
//   2160Hz マドモアゼル・愛さん、Youtubeで言及。疲れは精神的なものでもある。疲れにくい。

int main( void )
{
  asm( "nop" ); asm( "nop" ); asm( "nop" ); asm( "nop" ); asm( "nop" ); 
  _delay_ms( 500 );
  t_ini();    // soft_timer 初期化
  sw_ini();   // sw 初期化
  initialize();
  sei();
  while(1) {
    while( ! f_1ms ) ;    // 1[ms] wait
    f_1ms = 0;            // フラグクリア

    sw_main();
    t_main();
    sound_main();

    if( sw_dn & _BV( SW ) ) {
      def_tone( 0 );
      click++;
      sound_ptn = 0b1000000000000000;
      t_req( 0 );
      t_req( 2 );
    }

    if( t_tup( 0 ) ) {
      if( click == 1 ) {
        //  sound_ptn = 0b1001000000000000;
        if( ++tone_no >= 12 ) tone_no = 0;
        if( tone_no == 0 ) sound_ptn = 0b1111000100000000;
        t_can( 1 );
      }
      if( click == 2 ) {
        sound_ptn = 0b1010101010101010;
        if( tone_no == 0 ) tone_no = 1;
        t_req( 1 );
        t_req( 2 );
      }
      if( click == 3 ) {
        sound_ptn = 0b1111000100000000;
        tone_no = 0;
        t_can( 1 );
      t_req( 2 );
      }
      if( click == 5 ) {
        sound_ptn = 0b1111000100000001;
      if(led_x == 1) led_x = 0;
      else led_x = 1;
      eeprom_busy_wait();
      eeprom_write_byte( &EEPROM_LED, led_x );
      t_req( 2 );
      }
      t_can( 0 );
      click = 0;
    }
    if( t_tup( 2 ) ) {
      t_can( 2 );
      def_tone( tone_no );
    }
    if( t_tup( 1 ) ) {
      t_req( 1 );
      if( ++tone_no >= 12 ) tone_no = 1;
      def_tone( tone_no );
    }
    if(led_x == 1) led_on();
    else  led_off();
  }
    return 0;
}

void sound_main( void )
{
  if( --sound_cnt == 0x00 ) {
    sound_cnt = 50;

    if( sound_ptn == 0x0000 ) {
      sound_on = 0;
      return;
    }
    sound_on = ( sound_ptn & 0x8000 ) ? 1 : 0;
    sound_ptn <<= 1;
  }
}

void led_on( void )
{
  PORTD &= ~(_BV(LED));
}

void led_off( void )
{
  PORTD |= (_BV(LED));
}

void led_toggle( void )
{
  PORTD ^= (_BV(LED));
}

void initialize( void )
{
  f_1ms = 0;
  DDRB = _BV(Sounder) | _BV(LED);
  DDRD = _BV(LED);
  PORTD = _BV(SW);
  TCCR0 = ( 0b010 << CS00 );
  TIMSK = _BV( TOIE0 );
  TCNT0  = 256 - ( 10000000/8/5000 );

	eeprom_busy_wait();
	led_x=eeprom_read_byte( &EEPROM_LED );

  click = 0;
  tone_no = 1;          // 
  def_tone( tone_no );  // 
  t_req( 1 );          //  設定することによってリピートモードになります。
  //led_off();
}

void def_tone( unsigned int no )
{
  unsigned int tmp = tone[no];

  if( no == 0 ) {
    TCCR1A =  0;
    TCCR1B =  0;
    OCR1A  =  0;

  } else {
    TCCR1A = ( 1 << COM1A0 );
    TCCR1B = ( 1 << CTC1 ) | ( 1 << CS10 );
    OCR1A =  ( 10000000/1/( tmp + tmp ) );
  }
}

ISR( TIMER0_OVF0_vect )
{
  static unsigned char cnt = 0;
  TCNT0  = 256 - ( 10000000/8/5000 );

  if( ++cnt == 5 ) {
    cnt = 0;
    f_1ms = 1;
  }
  if( ! t_off( 2 ) ) {
    if( sound_on ) {
      def_tone( 12 );
      led_toggle();
    } else {
      def_tone( 0 );
      led_off();
    }
  }
}


