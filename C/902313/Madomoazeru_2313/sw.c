#include "sw.h"

unsigned char sw_conter;
unsigned char sw_kak;
unsigned char sw_z;
unsigned char sw_zz;
unsigned char sw_up;
unsigned char sw_dn;


//------------------------------------
// デジタル入力メイン（１ｍｓ毎に呼び出される）
//------------------------------------

#define SW    PD6


//------------------------------------
// デジタル入力初期化
//------------------------------------
void sw_ini()
{
	PORTD |=  _BV(PD6);      // PullUP
	asm( "nop" );
	sw_conter = 10;							// １０ｍｓカウンタセット
	sw_kak = 0xff;						// 電源投入時は未押下（今回負論理）とする
	sw_z = 0xff;
	sw_zz = 0xff;
}

//------------------------------------
// デジタル（スイッチ）入力メイン
//------------------------------------
void sw_main()
{
	sw_up = 0x00; 						// 立ち上がりフラグクリア
	sw_dn = 0x00; 						// 立ち下がりフラグクリア
	if (--sw_conter == 0)					// １０ｍｓ毎にスイッチ入力実行
	{
		sw_conter = 10; 					// １０ｍｓカウンタ再セット
		asm( "nop" );
		sw_3bud(PIND & _BV( SW )); 				// ３回照合（ＳＷ１本:p4_2）
		asm( "nop" );
	}
}
//------------------------------------
// ３回照合（↑↓）
// 引数：今回入力値(8bit)
//------------------------------------
void sw_3bud(unsigned char new)
{
	unsigned char wk; 					// ワーク

	wk = sw_kak; 						// 現在確定値保存
	sw_kak = (new & sw_z & sw_zz) | ((new | sw_z | sw_zz) & sw_kak); // 新確定値
	sw_up = ~wk & sw_kak; 				// 立ち上がり
	sw_dn = wk & ~sw_kak; 				// 立ち下がり
	sw_zz = sw_z; 					// 前々回←前回
	sw_z = new; 						// 前回←今回
}

