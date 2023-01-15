#if !(defined(__SW__))
#define __SW__

#include <avr/io.h>

#define SW    PD6

extern unsigned char sw_conter;
extern unsigned char sw_kak;
extern unsigned char sw_z;
extern unsigned char sw_zz;
extern unsigned char sw_up;
extern unsigned char sw_dn;

//------------------------------------
// デジタル入力メイン（１ｍｓ毎に呼び出される）
//------------------------------------

//------------------------------------
// デジタル入力初期化
//------------------------------------
void sw_ini();

//------------------------------------
// デジタル（スイッチ）入力メイン
//------------------------------------
void sw_main();

//------------------------------------
// ３回照合（↑↓）
// 引数：今回入力値(8bit)
//------------------------------------
void sw_3bud(unsigned char new);

#endif


