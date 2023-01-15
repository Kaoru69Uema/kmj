/***************************************************************************************************/
/* 8TAB                                                                                            */
/*                     --------                      ------------                                  */
/*                    |        |                    |            |                                 */
/*  +5[V]------------ |   A PB7|--------LCD_DATA_7--|            |--+5[V]                          */
/*  GND-------------- |   T PB6|--------LCD_DATA_6--|    L       |----GND                          */
/*                    |   t PB5|--------LCD_DATA_5--|            |                                 */
/*                    |   i PB4|--------LCD_DATA_4--|    C       |----VO---GND                     */
/*                    |   n    |                    |            |----R/W--GND                     */
/*                    |   y    |                    |    D       |                                 */
/*                    |   2    |                    |            |                                 */
/*                    |   3 PB1| --------LCD_RS-----|            |----LCD_DATA_3--OPEN             */
/*                    |   1 PB0| --------LCD_E------|            |----LCD_DATA_2--OPEN             */
/*                    |   3    |                    |            |----LCD_DATA_1--OPEN             */
/*                    |   A    |                    |            |----LCD_DATA_0--OPEN             */
/*                    |        |                     ------------                                  */
/*                    |        |                                                                   */
/*                     --------                                                                    */
/*                                                                                                 */
/*                                                                                                 */
/*                                                                                                 */
/*                                                                                                 */
/*                                                                                                 */
/*                      回路図                                                                     */
/*                                                                                                 */
/*                                                                                                 */
/***************************************************************************************************/

/*-------------------------------------------------------------------------------------------------*/
/*                                                                                                 */
/*                                                                                                 */
/*         ヘッダファイル宣言                                                                      */
/*                                                                                                 */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

/*-------------------------------------------------------------------------------------------------*/
/*                                                                                                 */
/*                                                                                                 */
/*         グローバル変数・定数・宣言                                                              */
/*                                                                                                 */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/

typedef   signed char schar;    // 8bit
typedef unsigned char uchar;

typedef   signed int  sint ;    // 16bit
typedef unsigned int  uint ;

typedef   signed long slong;    // 32bit
typedef unsigned long ulong;

const uchar a[] PROGMEM = "Tsuyoshi@@@0123Tsuyoshi@@@0123Tsuyoshi@@@0123Tsuyoshi@@@0123Tsuyoshi@@@0123Tsuyoshi@@@0123\0";

unsigned char ovf=0;
unsigned int  freq_val=0;

#define    LCD        PORTB
#define    LCD_DIR    DDRB
#define    LCD_E      PB1
#define    LCD_RS     PB0
#define    LCD_COL    16             // 20 or 16

/*-------------------------------------------------------------------------------------------------*/
/*                                                                                                 */
/*                                                                                                 */
/*         プロトタイプ宣言                                                                        */
/*                                                                                                 */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/
int  main( void );
void initialize( void );
void lcd_init( void );
void lcd_put_4bit( uchar );
void lcd_put_8bit( uchar );
void lcd_cmd_4bit( uchar );
void lcd_cmd_8bit( uchar );
ISR( TIMER0_OVF0_vect );
ISR( TIMER1_COMP1_vect );

int main( void )
{
	unsigned int tmp;
	int  i ;
	char valx[8] ;
	int target = 1234;

	initialize();         // ポート設定
	lcd_init();          // LCD初期化


	sei();

	lcd_cmd_8bit( 0x01 );       // Line 1
				    //lcd_cmd_8bit( 0x80 + 0x40 );  // Line 2
	lcd_put_8bit( 'G' );
	lcd_put_8bit( 'o' );
	lcd_put_8bit( 'o' );
	lcd_put_8bit( 'd' );
	lcd_put_8bit( '!' );
	

	while( 1 ) {
		lcd_cmd_8bit( 0x80 + 0x40 );  // Line 2


		//tmp = target ;
		tmp = freq_val;
		for ( i = 7 ; i > -1 ; i-- ) {
			*(valx+i) = (tmp % 10) + '0' ;
			tmp /= 10 ;
		}

		for ( i = 0 ; i < 8 ; i++ ) {
			lcd_put_8bit( *(valx+i) );
		}

	}
	return 0;
}



void initialize( void )
{
	LCD_DIR = (0xf0) + (1<<LCD_E) + (1<<LCD_RS);         // LCD_DATA_(7,6,5,4),  LCD_E,  LCD_RS  .
	LCD     = ( 1<<LCD_RS );                              // HIGH==データ（デフォルト）。LOW==コマンド
	DDRD    = ( 1<<PD6 );                                  // TEST_LED( PD6 ) .

	TCCR0 = 0b111 << CS00;
	TCCR1B = (0b010 << CS10) | _BV( CTC1 );
	OCR1A  = 10000000/8/1000;
	TIMSK  = _BV( OCIE1A ) | ( _BV( TOIE0 ) );

}

ISR( TIMER0_OVF0_vect )
{
	ovf++;
}

ISR( TIMER1_COMP1_vect )
{
	static unsigned int cntr = 0;

	PORTD ^= _BV( PD6 );

	if( ++cntr < 1000 ) return;
	cntr = 0;

	freq_val = ovf * 256 + TCNT0;
	ovf = 0;
	TCNT0 = 0;
}

void lcd_init( void )
{
	_delay_ms( 500 );
	lcd_cmd_4bit( 0x30 );
	_delay_ms(5);
	lcd_cmd_4bit( 0x30 );
	_delay_ms(5);
	lcd_cmd_4bit( 0x30 );
	_delay_ms(1);
	lcd_cmd_4bit( 0x20 );
	lcd_cmd_8bit( 0x28 );
	lcd_cmd_8bit( 0x0e );
	lcd_cmd_8bit( 0x06 );
	lcd_cmd_8bit( 0x01 );
	_delay_ms( 15 );
}

void lcd_put_4bit( uchar d )
{
	LCD = LCD & 0x0F;
	LCD = LCD | ( d & 0xf0 );
	LCD |=  (1<<LCD_E);             // HIGH
	_delay_us( 20 );
	LCD &= ~(1<<LCD_E);             // LOW
	_delay_us( 20 );
}

void lcd_put_8bit( uchar d )
{
	lcd_put_4bit( d & 0xf0 );
	lcd_put_4bit( d << 4 );
	_delay_ms( 10 );
}

void lcd_cmd_4bit( uchar c )
{
	LCD &= ~(1<<LCD_RS);            // LOW
	lcd_put_4bit( c );
	LCD |=  (1<<LCD_RS);            // HIGH
}

void lcd_cmd_8bit( uchar c )
{
	LCD &= ~(1<<LCD_RS);            // LOW
	lcd_put_8bit( c );
	LCD |=  (1<<LCD_RS);            // HIGH
}
