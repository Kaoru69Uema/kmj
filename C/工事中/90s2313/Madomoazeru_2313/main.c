#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "sw.h"
#include "soft_timer.h"

//   ソルフェジオ６音
//   396Hzマザー(原母)
//   417Hzトランス(許容への酔い)
//   528Hzセルフ(奇跡の輝き)
//   639Hzアーティスト(高度な自己)
//   741Hzクリアリング(清め)
//   852Hzインスパイヤ(天の智)
//   432HzリアルA(真の基音)
//   444Hz蘇生(復活)
//   153Hzレリーフ(安寧)

unsigned int tone[11] = { 396, 417, 528, 639, 741, 852, 888, 432, 444, 153, 2160, };
unsigned long cntr = 0;
unsigned int  index = 0;

int main( void );
void def_tone( unsigned int hz );


int main( void )
{
	asm( "nop" );
	
	DDRB = _BV(PB3);

	index = 0;
	def_tone( 396 );

	_delay_ms( 100 );

	while(1) {
		if( ++cntr == 60*3 ) {
			cntr = 0;
			if( ++index >= 11 ) index = 0;
			def_tone( tone[ index] );
		}
		_delay_ms( 1000 );
	}
	return 0;
}

void def_tone( unsigned int hz )
{
	DDRB = _BV(PB3);

	TCCR1A = ( 1 << COM1A0 );
	TCCR1B = ( 1 << CTC1 ) | ( 1 << CS10 );
	OCR1A =  ( 10000000/1/( hz + hz ) );
}
