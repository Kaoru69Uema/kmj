#include <avr/io.h>
#include <compat/ina90.h>		// ここでは_NOP, _CLI, _SEIが定義されます。
#include <avr/interrupt.h>
//#include "avr205_frequency_measure_interrupt.h"
#include "freq_meter.h"

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
unsigned int 	x48_cnt_to_four_ints;
#endif

unsigned int	freq_cntr_freq_divd_by_10;

/*! \brief AVR周辺機能の初期化関数
 *
 * この関数は接続された周辺機能に命令バイトを送り、返される状態符号を待ちます。
 *
 *  \note 周辺機能は先に初期化されていなければなりません。
 *
 */
void freq_cntr_init(void) {
  // T0入力ピンにやって来る正(上昇)端を数える16ビットFREQ_CNTRを初期化
#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)
  FREQ_CNTR_CTRLA = (1<<TC_COUNTMODE);		// タイマ/カウンタ0を16ビット動作形態に設定
#endif
#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
  FREQ_CNTR_CTRLA = 0;						// $FFFFでのOVFの標準16ビット動作形態
#endif
  // このコードが正しく動くには割り込みが許可されなければなりません。
}

//! GATE_CNTRは0.1秒採取開門用の時間基準として使用されます。

/*! \brief 割り込みが使用される時に、この関数は測定処理を始めるのに呼ばれます。\n
 * \T0のピン変化の状態は16ビット周波数計数器を始めさせます。\n
 *
 *  - この関数は多忙待ち(ポーリング)中に使用されません。\n
 *
 */
void freq_cntr_start_measurement(void){
  //! 最小遅延でPB6上の上昇端を検知するため、今やピン変化割り込みが許可されます。
#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)
  PIN_CHG_MASK = (1<<PIN_CHG_INT);
  GEN_INT_MASK = (1<<PIN_CHG_INT_ENAB);
#endif
#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
  PIN_CHG_MASK = (1<<PIN_CHG_INT);// for megax8;
  GEN_INT_MASK = (1<<PIN_CHG_INT_ENAB);
#endif
}

/*! \brief 主プログラムに10で割られた周波数を返します。\n
 * \freq_cntr_freq_divd_by_10が0でない時に、それがこの関数によって主プログラムに送られます。
 */
unsigned int	freq_cntr_get_result(void)
{
  return freq_cntr_freq_divd_by_10;
}

/*! \brief 次の周波数計数を準備するために結果を解除\n
 */
void	freq_cntr_clear_result(void)
{
  freq_cntr_freq_divd_by_10 = 0;
}

ISR(PIN_CHG_vect) {
  if ((PIN & (1<<PIN_NO)) !=0)
  {
	PIN_CHG_MASK	= (0<<PIN_CHG_INT);
	GEN_INT_MASK &= ~(1<<PIN_CHG_INT_ENAB);
	GATE_CNTR_INT_FLAG_REG	 |= (1<<GATE_CNTR_OVF_FLAG);	// GATE_CNTR溢れフラグ解除

	// tiny x61:PB6またはmega x8:PD5にやって来る信号を計数するためにFREQ_CNTRを許可
	FREQ_CNTR_COUNT_HIGH  = 0x00;							// 計数器上位8ビット解除
	FREQ_CNTR_COUNT_LOW  = 0x00;							// 計数器下位8ビット解除
	FREQ_CNTR_CTRLB = (1<<CLOCK_SEL_2)|(1<<CLOCK_SEL_1);	// FREQ_CNTR入力クロック選択

	// 予め設定される以下の計数器は0.1秒/512μs区間=195.3回によって決められました。
	// TCNT1=255-195;	// 0.1秒の遅延を生成するためにGATE_CNTR上昇計数器はこの値で設定されます。
#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)
	GATE_CNTR_COUNT = GATE_CNTR_PRELOAD;
	GATE_CNTR_COUNT_HI = 3;
	GATE_CNTR_INT_MASK_REG = (1<<GATE_CNTR_OVF_INT_ENAB);
#endif
#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
	GATE_CNTR_COUNT = 0;
#endif

	GATE_CNTR_INT_MASK_REG = (1<<GATE_CNTR_OVF_INT_ENAB);
#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)
	GATE_CNTR_CTRLB = (1<<CLOCK_SEL_13)|(1<<CLOCK_SEL_12)|(1<<CLOCK_SEL_10);//start GATE_CNTR
#endif
#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
	GATE_CNTR_CTRLB = (1<<CLOCK_SEL_02)|(1<<CLOCK_SEL_00);	// 1/1024、mega x8では最大分周が可能
	x48_cnt_to_four_ints = 0;								// 門を閉じる前に4つの割り込みを数えるx48計数器を解除
#endif
  }
  // 下降端ピン変化割り込み検知、故にこれを無視
}

////////////////////////////////

ISR(GATE_CNTR_OVF_vect) {

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
  x48_cnt_to_four_ints++;
  if(x48_cnt_to_four_ints == 3)GATE_CNTR_COUNT = 0xff-13;
  if(x48_cnt_to_four_ints >= 4)
  {
	FREQ_CNTR_CTRLB = 0;					// FREQ_CNTR停止
	GATE_CNTR_CTRLB = 0;

//	ovf_test_and_return_result();			// 結果をfreq_cntr_freq_divd_by_10に配置
	// 16ビット溢れに関してTIFR検査、溢れ範囲で$FFFFを返す。
	if ((FREQ_CNTR_INT_FLAG_REG & (1<<FREQ_CNTR_OVF_FLAG)) !=0) {
	  // 16ビット溢れに関してGATE_CNTR_OVF_FLAGを検査
	  freq_cntr_freq_divd_by_10 = 0xFFFF;	// これはOVF状態を返します。
	}
	else {
	  freq_cntr_freq_divd_by_10 = FREQ_CNTR_COUNT_LOW+(FREQ_CNTR_COUNT_HIGH<<8);
	}
  }
#endif

#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)
  {
	FREQ_CNTR_CTRLB = 0;
	GATE_CNTR_CTRLB = 0;

//	ovf_test_and_return_result();			// 結果をfreq_cntr_freq_divd_by_10に配置
	// 16ビット溢れに関してTIFR検査、溢れ範囲で$FFFFを返す。
	if ((FREQ_CNTR_INT_FLAG_REG & (1<<FREQ_CNTR_OVF_FLAG)) !=0) {
	  // 16ビット溢れに関してGATE_CNTR_OVF_FLAGを検査
	  freq_cntr_freq_divd_by_10 = 0xFFFF;	// これはOVF状態を返します。
	}
	else {
	  freq_cntr_freq_divd_by_10 = FREQ_CNTR_COUNT_LOW+(FREQ_CNTR_COUNT_HIGH<<8);
	}
  }
#endif
}
