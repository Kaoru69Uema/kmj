/* このファイルはDoxygen自動資料生成用に準備されています。*/
/*! \file *********************************************************************
 *
 * \brief
 *		ATtiny x61/ATmega x8に基づく周波数計数器: 多忙待ち版
 *
 *		2つのタイマ/カウンタと2つの割り込み源を用いた周波数計数器: FREQ_CNTRはT0上の上昇入力を数えます。
 *		T0のピン変化割り込みが入力の上昇端遷移の計数を開始します。
 *		delay100ms(1)関数は入力信号が計数される時間長を決める0.1秒基準時間を生成します。
 *		GATE_CNTRはそれが溢れる時にそのような値で予め設定され、OVF割り込みが生成されて計数器が停止されます。
 *		これは16ビットのFREQ_CNTRの結果に従ってfreq_cntr_freq_divd_by_10と記されたSRAM位置に書かれます。
 *
 *		この周波数測定算法の精度は99%またはそれ以上で、AVRの8MHzクロック精度の関数
 *		です。各応用はこの指示経由でPB5のHighとPB5のLowの測定によってより高い精
 *		度に調整することができます。
 *
 *		#define DELAY_VAL_100_MS  0xFFFF - (5560 * AVR_Clk_freq) // 0.1秒開門時間の大小に対しては5560の数を調整してください。
 *
 * \par Documentation
 *		包括的なコード資料、支援コンパイラ、コンパイラ設定、支援デバイスについてはreadme.htmlをご覧ください。
 *
 * \author
 *		Atmel Corporation.
 *		支援Eメール: avr@atmel.com
 *
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

/*! \page doc_page1 AVRへの信号源接続と結果観測の方法\n
 * tiny x61が望まれたAVRなら、このAVR Studioプロジェクトが開かれた後で、
 *	- Project⇒Configuration Options⇒Device attiny 261, tiny461またはattiny861をクリックしてください。
 *	- 測定されるべきデジタル信号をT0と記されたAVRピンに接続してください。
 *	- 信号の振幅はAVRのVihとVilの電圧に適合させるべきです。
 *	- JTAGICEmkⅡデバッガを使用し、この.cコードで各々の結果が決められた後に中断点を設定することが示唆されます。\n
 *
 * MEGAが望まれたAVRなら、このAVR Studioプロジェクトが開かれた後で、
 *	- Project⇒Configuration Options⇒Device mega48, mega88またはmega168をクリックしてください。
 *	- 測定されるべきデジタル信号をAVRのPD7ピンに接続してください。
 *	- 信号の振幅はAVRのVihとVilの電圧に適合させるべきです。
 *	- JTAGICEmkⅡデバッガを使用し、この.cコードで各々の結果が決められた後に中断点を設定することが示唆されます。\n
*/

#include <avr/io.h>
#include <compat/ina90.h>		// ここでは_NOP, _CLI, _SEIが定義されます。
#include <avr/interrupt.h>
#include "freq_meter.h"


/* freq_cntr_freq_divd_by_10()関数は0.1秒区間に渡って測定された16ビットの入力周波数値を返します。
 * これは0.1秒区間に渡って入力信号、PB6での遷移を計数するFREQ_CNTRを用いて行われます。
 *
 * 以下のCコードに関して割り込みが禁止されることが推奨され、また0.1秒測定は測定されつつある周波数の精度を大小し得ます。
 *
 * 出力ピンPB5は0.1秒計数周期の始めにHigh、後でLowに設定されます。
 * コード性能での変化なしを望む場合、PB5に関連する命令が取り去られるかもしれません。
 *
 * この周波数測定算法の精度は99%またはそれ以上で、AVRの8MHzクロック精度の関数です。
 * 各応用はこの指示経由でPB5のHighとPB5のLowの測定によってより高い精度に調整することができます。
 *
 * #define DELAY_VAL_100_MS  0xFFFF - (5560 * AVR_Clk_freq) // 0.1秒開門時間の大小に対しては5560の数を調整してください。
 *
 * 開門計数器はここでは使用されません。開門タイミングは簡単なソフトウェア遅延繰り返し(delay100ms(遅延))によって成し遂げられます。
 */

#include <avr/io.h>
#include <compat/ina90.h>
#include <avr/interrupt.h>
#include "freq_meter.h"

unsigned int freq_div_by_10;

// ここで定義された原型
unsigned int	freq_cntr_get_frequency(void);
void			delay100ms(unsigned char);

int main() {
  freq_cntr_init();

  users_init();			// 使用者は以下で定義されるこの関数内にそれら自身のコードを挿入してください。

  while (1) {
	freq_div_by_10 = freq_cntr_get_frequency();
	delay100ms(2);		// この遅延はAVRコードの残りによって使用される時間を偽装します。
  }
} // main終了


void users_init(void)
{
  _NOP();				// 使用者は初期化コードをここに挿入してください。
}

// 以下の関数は関数名で指定される遅延を生成するように調整され、8MHzで動く8ビットAVRで動作します。
void delay100ms(unsigned char count)
{
  unsigned int delay_counter = 1;;

  while (count > 0){
  delay_counter = DELAY_VAL_100_MS;
  while (delay_counter > 0) delay_counter++;
  count--;
  }
}
