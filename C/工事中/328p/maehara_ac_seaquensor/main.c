//
//   令和4年10月27日（2022年10月27日）
//   上間つよし＠沖縄県西原町＠久米島出身
//
//                               
//                ----------     
//             --|RESET  PC5|--          XTAL1       XTAL2
//             --|PD0    PC4|--            |           |
//             --|PD1    PC3|--            +--20[MHz]--+
//             --|PD2    PC2|--            |           |
//             --|PD3    PC1|--           ---         ---
//             --|PD4    PC0|--           ---         ---
//             --|VCC    GND|--            |           |
//             --|GND   AREF|--           0[v]        0[v]
//             --|XTAL1 AVCC|--
//             --|XTAL2  PB5|--
//             --|PD5    PB4|--      PD7---------7SEG_P
//             --|PD6    PB3|--      PD6---------7SEG_G
//             --|PD7    PB2|--      PD5---------7SEG_F
//             --|PB0    PB1|--      PD4---------7SEG_E
//                ----------         PD3---------7SEG_D
//                                   PD2---------7SEG_C
//                                   PD1---------7SEG_B
//                                   PD0---------7SEG_A
//
//
//
//
//
//
//
//
//

#include <avr/io.h>
#include <avr/interrupt.h>

/* グローバル変数 */
volatile unsigned char f_1ms = 0;

/* プロトタイプ宣言 */
int main( void );
void initialize( void );
ISR( TIMER1_COMPA_vect );

int main( void )
{
	initialize();
	sei();

	while( 1 ) {

		while( ! f_1ms ) ;
		f_1ms = 0;
	}
	return 0;
}

void initialize( void )
{
	TCCR1B = ( 1 << WGM12 ) | ( 1 << CS11 ); /* CTC動作、 ck/8 */
	OCR1A  = 20000000/8/1000-1;  /* 1/1000 = 1[ms]タイマー */
	TIMSK1 = 1 << OCIE1A;        /* 比較一致割込み許可 */
	TCNT1  = 0;
}

ISR( TIMER1_COMPA_vect )
{
	f_1ms = 1;
}
