//
//   令和4年10月14日（2022年10月14日）
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
//                     Tiny85
//                    ---------
//                 --|Reset VCC|--5[v]
// 0[v]--[330]--LED--|PB3   PB2|--
// 0[v]---------SW---|PB4   PB1|--
// 0[v]--------------|GND   PB0|--
//                    ---------
//
//
//
//
#include <avr/io.h>


int main( void )
{

	initialize();

	while( 1 ) {
		PORTB = ( PINB & ( 1 << PB4 ) ) >> 1;
	}
	return 0;
}

void initialize( void )
{
	DDRB  = 1 << PB3;
	PORTB = 1 << PB4;
}
