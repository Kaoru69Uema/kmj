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
#include "sw.h"

#define LED       PD5
//#define SW        PD6

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
  sw_ini();   // sw 初期化
  initialize();
  sei();
  while(1) {
    while( ! f_1ms ) ;    // 1[ms] wait
    f_1ms = 0;            // フラグクリア

    sw_main();

    if( sw_dn & _BV( SW ) ) {     // 押した？
      led_on();
    }

    if( sw_up & _BV( SW ) ) {     // 離した？ 
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
  PORTD |= _BV(SW);    // SW Pull UP
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
