/* このファイルはDoxygen自動資料生成用に準備されています。*/
/*! \file *********************************************************************
 *
 * \brief
 *		ATtiny x61/ATmegax8に基づく周波数計数器: 多忙待ち版
 *
 *		このファイルは実演を動かすのに必要なドライバを含み、使用者またはお客様の応用での使用に利用可能です。
 *
 *
 * \par Documentation
 *		包括的なコード資料、支援コンパイラ、コンパイラ設定、支援デバイスについてはreadme.htmlをご覧ください。
 *
 * \author
 *		Atmel Corporation.
 *		支援Eメール: avr@atmel.com
 *
 * 著作権 (c) 2010, Atmel Corporation 全権利予約済み。
 *
 * 変更の有りまたはなしでソースと2進の形式での使用と再配布は以下の条件に合っていれ
 * ば許されます。
 *
 * 1. ソース コードの再配布は上の著作権通知、この条件一覧、それと以下のお断りを維持し
 *	  なければなりません。
 *
 * 2. 2進形式での再配布はこの配布で提供された資料や他の素材で、上の著作権通知、
 *	  この条件一覧、それと以下のお断りを再現しなければなりません。
 *
 * 3. ATMELの名称は先に書かれた許諾の指定なしにこのソフトウェアから派生した販促製品や
 *	  裏書(保証)に使用できないかもしれません。
 *
 * このソフトウェアはどんな明示的または暗示的な保証、包含物の制限なしの"現状そのまま"
 * でATMELによって提供され、特定目的のための市場性と適合性の暗黙的な保証は明白
 * 且つ明確に放棄されます。たとえATMELがそのような損害賠償の可能性を進言された
 * としても、本資料を使用できない、または使用以外で発生する(情報の損失、事業中
 * 断、または利益の損失に関する制限なしの損害賠償を含み)、直接、間接、必然、偶
 * 然、特別、または付随して起こる如何なる損害賠償に対しても決してATMELに責任が
 * ないでしょう。
 *****************************************************************************/

#include <avr/io.h>
#include <compat/ina90.h>		// ここでは_NOP, _CLI, _SEIが定義されます。
#include <avr/interrupt.h>
#include "freq_meter.h"


// FREQ_CNTRを16ビット、tiny x61に対してT0、mega x8に対してT1のピン上でやって来る正(上昇)端での計数に初期化
void freq_cntr_init(void)
{
// T0入力ピンにやって来る正(上昇)端を計数する16ビットFREQ_CNTR初期化
#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)
  FREQ_CNTR_CTRLA = (1<<TC_COUNTMODE);		// tiny x61: タイマ/カウンタ0を16ビット動作形態に設定
#endif
#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
  FREQ_CNTR_CTRLA = 0;						// mega x8: 16ビット動作形態は既定形態、OVFは$FFFF
#endif
}

// freq_div_by_10変数内に10で割った入力信号の周波数を返します。
unsigned int freq_cntr_get_frequency(void)
{
  unsigned int freq_cntr_freq_divd_by_10;

  _CLI();
  FREQ_CNTR_COUNT_HIGH = 0x00;							// 計数器下位8ビット解除
  FREQ_CNTR_COUNT_LOW = 0x00;							// 計数器上位8ビット解除
  // FREQ_CNTRを外部T0ピンに接続して計数開始
  FREQ_CNTR_CTRLB = (1<<CLOCK_SEL_2)|(1<<CLOCK_SEL_1);	// FREQ_CNTR入力クロック選択

  // タイマ/カウンタ0の最初のクロックまで待機
  while (FREQ_CNTR_COUNT_LOW == 0) _NOP();
  // 今やFREQ_CNTRをどの位動かすかを決めるソフトウェア計時器を開始
  delay100ms(1);
  FREQ_CNTR_CTRLB = 0;									// (0<<CLOCK_SEL_00);	// 計数器停止

  // 16ビット溢れに対するTIFR検査、溢れで範囲越え($FFFF)を返します。
  if ((FREQ_CNTR_INT_FLAG_REG & (1<<FREQ_CNTR_OVF_FLAG)) !=0) {
	// 16ビット溢れに対するTIFR検査、溢れで=$FFFF
	FREQ_CNTR_INT_FLAG_REG = (1<<FREQ_CNTR_OVF_FLAG);	// OVFフラグ解除
	freq_cntr_freq_divd_by_10 = 0xFFFF;					// これはOVF条件を返すためです。
  }
  else {
	freq_cntr_freq_divd_by_10 = FREQ_CNTR_COUNT_LOW+(FREQ_CNTR_COUNT_HIGH<<8);
  }
  return(freq_cntr_freq_divd_by_10);
}
