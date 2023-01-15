#include "sw.h"

unsigned char sw_conter;   	// 10[ms]毎にスイッチサンプリング：ダウンカウンタ
unsigned char sw_kak;		// スイッチ確定値
unsigned char sw_z;			// 前回サンプリング値
unsigned char sw_zz;		// 前々回サンプリング値
unsigned char sw_up;		// スイッチ立ち上がりフラグ：離した時
unsigned char sw_dn;		// スイッチ立ち下がりフラグ：押した時

//------------------------------------
// デジタル入力初期化
//------------------------------------
void sw_ini()
{
	PORTD |=  _BV(SW);	// PullUP
	sw_conter = 10;		// １０ｍｓカウンタセット
	sw_kak = 0xff;		// 電源投入時は未押下（今回負論理）とする
	sw_z = 0xff;		// 前回サンプリング値
	sw_zz = 0xff;		// 前々回サンプリング値
}

//----------------------------------------------
// デジタル入力メイン（１ｍｓ毎に呼び出される）
//----------------------------------------------
void sw_main()
{
	sw_up = 0x00; 						// 立ち上がりフラグクリア
	sw_dn = 0x00; 						// 立ち下がりフラグクリア
	if (--sw_conter == 0)				// １０ｍｓ毎にスイッチ入力実行
	{
		sw_conter = 10; 				// １０ｍｓカウンタ再セット
		sw_3bud(PIND & _BV( SW )); 		// ３回照合（ＳＷ１本）
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
	sw_kak = (sw_zz & sw_z & new) | ((sw_zz | sw_z | new) & sw_kak); // 新確定値
	sw_up = ~wk & sw_kak; 				// 立ち上がり
	sw_dn = wk & ~sw_kak; 				// 立ち下がり
	sw_zz = sw_z; 						// 前々回 ← 前回
	sw_z = new; 						// 前回 ← 今回
}
