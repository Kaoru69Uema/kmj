#if 0
                                      +--||-----------0[v]
                    .---------        |  0.1[uF]
  22[pF]x2      -- |~RST   VCC|-------+---------------5[v]
            _   -- | PD0   PB7|--
  0[v]--||-|1|  -- | PD1   PB6|--
           |0|---- | XTAL2 PB5|--
  0[v]--||-|M|---- | XTAL1 PB4|--
            -   -- | PD2   PB3|--
                -- | PD3   PB2|--
                -- | PD4   PB1|--
5[v]--[680]--LED-- | PD5   PB0|--
                -- | GND   PD6|------SW---------------0[v]
                    ----------
                    AVR AT90S2313


#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "soft_timer.h"

#define LED       PD5

volatile unsigned char f_1ms;

//====================================================
int main( void );
void initialize( void );
void led_on( void );
void led_off( void );
ISR( TIMER0_OVF0_vect );
//====================================================

int main( void )
{
  t_ini();   // soft_timer 初期化
  initialize();
  sei();
  t_req( 0 );    // soft_timer_1 = 500[ms] リクエスト
  while(1) {
    while( ! f_1ms ) ;    // 1[ms] wait
    f_1ms = 0;            // フラグクリア

    t_main();            // soft_timerメイン

    if( t_tup( 0 ) ) {     // タイムアップ？
      t_can( 0 );          // キャンセル
      t_req( 1 );          // タイマー１リクエスト
      led_on();
    }

    if( t_tup( 1 ) ) {     // タイムアップ？
      t_can( 1 );          // キャンセル
      t_req( 0 );          // タイマー１リクエスト
      led_off();
    }
  }
  return 0;
}

void led_on( void )
{
  PORTD &= ~(_BV(LED));
}

void led_off( void )
{
  PORTD |= (_BV(LED));
}

void initialize( void )
{
  f_1ms = 0;
  DDRD = _BV(LED);
  PORTD |= _BV(LED);   // LED OFF
  TCCR0 = ( 0b010 << CS00 );  // ck/8
  TIMSK = _BV( TOIE0 );    // OVF interrupt
  TCNT0  = 256 - ( 10000000/8/5000 );   // 1/5000[s] == 200[us] interrupt
}

ISR( TIMER0_OVF0_vect )
{
  static unsigned char cnt = 0;
  TCNT0  = 256 - ( 10000000/8/5000 );

  if( ++cnt == 5 ) {
    cnt = 0;
    f_1ms = 1;
  }
}
