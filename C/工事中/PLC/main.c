// 上間つよし＠沖縄県西原町＠久米島出身 令和4年9月26日


// ヘッダファイル

#include<avr/io.h>
#include <avr/interrupt.h>


// グローバル変数

////////////////////////////////////////////////
typedef union {
  struct {
    unsigned B7:1;
    unsigned B6:1;
    unsigned B5:1;
    unsigned B4:1;
    unsigned B3:1;
    unsigned B2:1;
    unsigned B1:1;
    unsigned B0:1;
  } BIT;
  unsigned char FG;
} FLGS;
////////////////////////////////////////////////

FLGS flags;

////////////////////////////////////////////////
#define f1ms flags.BIT.B0

#define TRUE  1
#define FALSE 0

////////////////////////////////////////////////

uint8_t _7SEG[] = {
	0b00111111,  // 0
	0b00000110,  // 1
	0b01011011,  // 2
	0b01001111,  // 3
	0b01100110,  // 4
	0b01101101,  // 5
	0b01111101,  // 6
	0b00100111,  // 7
	0b01111111,  // 8
	0b01101111,  // 9
};

ISR(TIMER0_COMPA_vect)
{
	f1ms = TRUE;
}

int main(void){
  TCCR0A = 1<<WGM01;  // CTCタイマー
  TCCR0B = 1<<CS01;   // CK/8
  TIMSK0 = 1<<OCIE0A; // OCR0Aコンペアマッチ割込み
  OCR0A  = 1000000/8/1000-1; // 周期0.001[s]==1000[Hz]
  DDRC = 0b00001111;
  DDRB = 0b11111111;
  sei();
  TCNT0 = 0;
  while(1){
	  while( f1ms == FALSE ) ;
	  f1ms = FALSE;
	  PORTC = 0b00001110;
	  PORTB = 0b11111111;
  }
  return 0;
}


