#if 1
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#endif

#include "lcd.h"

unsigned char ovf=0;
unsigned int  freq_val_t0 = 0;  // Hz 測定値 t0
unsigned int  freq_val_t1 = 0;  // Hz 測定値 t1
unsigned int  freq_val_t2 = 0;  // Hz 測定値 t2
unsigned int  freq_val_t3 = 0;  // Hz 測定値 t3
unsigned int  freq_val_t4 = 0;  // Hz 測定値 t4
unsigned int  freq_val_t5 = 0;  // Hz 測定値 t5
unsigned int  freq_val_t6 = 0;  // Hz 測定値 t6
unsigned int  freq_val_t7 = 0;  // Hz 測定値 t7
unsigned int  freq_val_t8 = 0;  // Hz 測定値 t8
unsigned int  freq_val_t9 = 0;  // Hz 測定値 t9
unsigned char freq_val_count=0; // freq_val_tx 保存回数。１０回以上は、上がらない。


char valx[8] ;
int target = 1234;

int  main( void );
void initialize( void );
void disp( void );
ISR( TIMER0_OVF0_vect );
ISR( TIMER1_COMP1_vect );
void add_arr( void );

int main( void )
{

	_delay_ms( 100 );

	initialize();         // ポート設定
	lcd_init();          // LCD初期化


	sei();

	

	while( 1 ) {

		disp();
	}
	return 0;
}


void initialize( void )
{
	LCD_DIR = (0xf0) + (1<<LCD_E) + (1<<LCD_RS);
	LCD     = ( 1<<LCD_RS );
	DDRD    = ( 1<<PD6 );

	TCCR0 = 0b111 << CS00;
	TCCR1B = (0b010 << CS10) | _BV( CTC1 );
	OCR1A  = 10000000/8/1000;
	TIMSK  = _BV( OCIE1A ) | ( _BV( TOIE0 ) );

}

void disp( void )
{
	unsigned long tmp;
	int  i ;
	unsigned char notZero = 0;
	

	lcd_cmd_8bit( 0x80 );

	if( ( freq_val_t0 <= freq_val_t9 + 1 ) && ( freq_val_t0 >= freq_val_t9 - 1 ) ) {
		lcd_put_8bit( 'G' );
		lcd_put_8bit( 'o' );
		lcd_put_8bit( 'o' );
		lcd_put_8bit( 'd' );
		lcd_put_8bit( '.' );
	} else {
		lcd_put_8bit( '!' );
		lcd_put_8bit( ' ' );
		lcd_put_8bit( ' ' );
		lcd_put_8bit( ' ' );
		lcd_put_8bit( ' ' );
	}

	tmp = freq_val_t0 + freq_val_t1 + freq_val_t2 + freq_val_t3 + freq_val_t4 +
	           freq_val_t5 + freq_val_t6 + freq_val_t7 + freq_val_t8 + freq_val_t9;


	tmp *= 10;
	tmp /= freq_val_count;



	for ( i = 7 ; i > -1 ; i-- ) {
		*(valx+i) = (tmp % 10) + '0' ;
		tmp /= 10 ;
	}

	lcd_cmd_8bit( 0x80 + 0x40 );  // Line 2

	for ( i = 0 ; i < 8 ; i++ ) {
		if( i==7 ) lcd_put_8bit('.');
		if( *(valx+i) != '0' ) notZero = 1;
		if( ( notZero==0 ) && ( *(valx+i)=='0') ) lcd_put_8bit( ' ' );
		else lcd_put_8bit( *(valx+i) );
	}

	lcd_put_8bit( ' ' );
	lcd_put_8bit( 'H' );
	lcd_put_8bit( 'z' );
}

ISR( TIMER0_OVF0_vect )
{
	ovf++;
}

ISR( TIMER1_COMP1_vect )
{
	static unsigned int cntr=0;
	       unsigned int hz=0;

	PORTD ^= _BV( PD6 );

	
	if( ++cntr < 1000 ) return;
	cntr = 0;

	hz = ( (ovf * 256) + TCNT0 );
	ovf = 0;
	TCNT0 = 0;

  if( ++freq_val_count > 10 ) freq_val_count = 10;
	freq_val_t0 = freq_val_t1;
	freq_val_t1 = freq_val_t2;
	freq_val_t2 = freq_val_t3;
	freq_val_t3 = freq_val_t4;
	freq_val_t4 = freq_val_t5;
	freq_val_t5 = freq_val_t6;
	freq_val_t6 = freq_val_t7;
	freq_val_t7 = freq_val_t8;
	freq_val_t8 = freq_val_t9;
	freq_val_t9 = hz;
}


