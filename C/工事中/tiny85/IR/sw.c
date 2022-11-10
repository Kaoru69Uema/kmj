#include "sw.h"

/* プロジェクトごとに設定 */
#define PULLUP ( PORTB |=  1 << PB4 )
#define INPUT  ( (PINB & ( 1 << PB4 )) )


unsigned char sw_conter;
unsigned char sw_kak;    /* 確定値 　*/
unsigned char sw_z;      /* 前回値　 */
unsigned char sw_zz;     /* 前前回値 */
unsigned char sw_up;     /* 離した   */
unsigned char sw_dn;     /* 押した   */


//------------------------------------
// デジタル入力メイン（１ｍｓ毎に呼び出される）
//------------------------------------

//------------------------------------
// デジタル入力初期化
//------------------------------------
void sw_ini()
{
	PULLUP;
	asm( "nop" );
	sw_conter = 10;	// １０ｍｓカウンタセット
	sw_kak = 0xff;	// 電源投入時は未押下
	sw_z = 0xff;    // 前回値
	sw_zz = 0xff;   // 前前回値
}

//------------------------------------
// デジタル（スイッチ）入力メイン
//------------------------------------
void sw_main()
{
	sw_up = 0x00; 	// 立ち上がりフラグクリア
	sw_dn = 0x00; 	// 立ち下がりフラグクリア
	if (--sw_conter == 0)	// １０ｍｓ毎にスイッチ入力実行
	{
		sw_conter = 10; // １０ｍｓカウンタ再セット
		sw_3bud( INPUT ); // ３回照合
	}
}
//------------------------------------
// ３回照合（↑↓）
// 引数：今回入力値(8bit)
//------------------------------------
void sw_3bud(unsigned char new)
{
	unsigned char wk; // ワーク

	wk = sw_kak; // 現在確定値保存
	sw_kak =
		(new & sw_z & sw_zz) |
		((new | sw_z | sw_zz) & sw_kak); // 新確定値
	sw_up = ~wk & sw_kak; // 立ち上がり
	sw_dn = wk & ~sw_kak; // 立ち下がり
	sw_zz = sw_z; // 前々回 ←  前回
	sw_z = new;   // 前回 ←  今回
}



