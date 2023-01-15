// マドモアゼル・愛・レスキューCD//   396Hzマザー(原母)
// マドモアゼル・愛・レスキューCD//   417Hzトランス(許容への酔い)
// マドモアゼル・愛・レスキューCD//   528Hzセルフ(奇跡の輝き)
// マドモアゼル・愛・レスキューCD//   639Hzアーティスト(高度な自己)
// マドモアゼル・愛・レスキューCD//   741Hzクリアリング(清め)
// マドモアゼル・愛・レスキューCD//   852Hzインスパイヤ(天の智)
// マドモアゼル・愛・Youtube言及 //   888Hz 852Hzと同じ？
// マドモアゼル・愛・レスキューCD//   432HzリアルA(真の基音)
// マドモアゼル・愛・レスキューCD//   444Hz蘇生(復活)
// マドモアゼル・愛・レスキューCD//   153Hzレリーフ(安寧)
// マドモアゼル・愛・Youtube言及 //   2160Hz 疲れにくい

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main( void );
void initialize();

int main( void )
{
  initialize();
  sei();

  while ( 1 ) {

  }
  return 0;
}

#define DIV_OFF     0b0000
#define DIV_1       0b0001
#define DIV_2       0b0010
#define DIV_4       0b0011
#define DIV_8       0b0100
#define DIV_16      0b0101
#define DIV_32      0b0110
#define DIV_64      0b0111
#define DIV_128     0b1000
#define DIV_256     0b1001
#define DIV_512     0b1010
#define DIV_1024    0b1011
#define DIV_2048    0b1100
#define DIV_4096    0b1101
#define DIV_8192    0b1110
#define DIV_16384   0b1111

void initialize()
{
  TCCR1 = _BV( COM1A1 )|_BV( CTC1 )|_BV( PWM1A )|DIV_64;
  //GTCCR |= _BV( PSR1 );
  OCR1C = 210;
  OCR1A = 210/2;
  DDRB = _BV( PB1 );
#if 0
DIV = 64,
X = 209.86369770580296 ;
DIV = 128,
X = 104.43184885290148 ;
DIV = 256,
X = 51.71592442645074 ;
DIV = 512,
X = 25.35796221322537 ;
DIV = 1024,
X = 12.178981106612685 ;
DIV = 2048,
X = 5.589490553306343 ;
DIV = 4096,
X = 2.2947452766531713 ;
DIV = 8102,
X = 0.6656722603272514 ;
#endif


}
