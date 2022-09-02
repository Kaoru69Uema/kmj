// 2022/09/02 10:20   上間つよし＠沖縄県西原町＠久米島出身
//
// tiny85  internal 8[MHz]
// 内部基準電圧1.1[v]とPB1の電圧を比較した結果をPB4(LED)に表示する
// アナログコンパレーターはデフォルトで、常に働いている。
// 設定を、基準電圧を1.1[v]にしただけ。

#include <avr/io.h>

int main( void )
{
	DDRB  = 1<<PB4;
	ACSR  = 1<<ACBG;           // 内部基準電圧1.1[v]選択

	for(;;) {
		if( ACSR & (1<<ACO) )
			PORTB = 0<<PB4;
		else
			PORTB = 1<<PB4;
	}
	return 0;
}
