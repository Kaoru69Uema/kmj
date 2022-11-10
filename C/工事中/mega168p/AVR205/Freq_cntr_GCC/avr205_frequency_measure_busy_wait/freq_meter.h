/* このファイルはDoxygen自動資料生成用に準備されています。*/
/*! \file ********************************************************************
*
* Atmel Corporation
*
* ファイル			: Frequency_counter.h
* コンパイラ		: WinAVR GCC
* 改訂			: $20100110
* 日付			: $Date: 17 Jan 2011
* 更新者		: $Author: Marv Kausch
*
* 支援メール		: avr@atmel.com
*
* 支援デバイス	: この例はATTiny261/461/861,ATmega48/88/168用に書かれています。
*
* 応用記述		: AVR205 - ATMEL tinyAVRとATMEL megaAVRで容易にさせる周波数測定
*
* 説明			: Frequency_counter用ヘッダ ファイル
*
****************************************************************************/

/*! \page 許諾
著作権 (c) 2009 Atmel Corporation. 全権利予約済み。

変更の有りまたはなしでソースと2進の形式での使用と再配布は以下の条件に合っていれば
許されます。

1. ソース コードの再配布は上の著作権通知、この条件一覧、それと以下のお断りを維持しな
   ければなりません。

2. 2進形式での再配布はこの配布で提供された資料や他の素材で、上の著作権通知、こ
   の条件一覧、それと以下のお断りを再現しなければなりません。

3. ATMELの名称は先に書かれた許諾の指定なしにこのソフトウェアから派生した販促製品や裏
   書(保証)に使用できないかもしれません。

4. このソフトウェアはATMEL AVR製品との繋がりでだけ再配布そして使用されるかもしれませ
   ん。

このソフトウェアはどんな明示的または暗示的な保証、包含物の制限なしの"現状そのまま"で
ATMELによって提供され、特定目的のための市場性と適合性の暗黙的な保証は明白且つ明
確に放棄されます。たとえATMELがそのような損害賠償の可能性を進言されたとしても、
本資料を使用できない、または使用以外で発生する(情報の損失、事業中断、または利益
の損失に関する制限なしの損害賠償を含み)、直接、間接、必然、偶然、特別、または付
随して起こる如何なる損害賠償に対しても決してATMELに責任がないでしょう。
*/

// これはコード資料に使用されるべき/され得る追加項目を持つ我々の標準ヘッダです。全
// ての付箋についてはDoxygenでインストールされるDoxygen資料をご覧ください。Doxygenの
// 使用法の記述についてはComment_sourcecode_doxygen.DOCをご覧ください。

// mainpageの例:
// (main.cまたは独立した資料.hのどちらかの最後に配置)

/* Doxygen資料mainpage ********************************************/
/*! \mainpage
 * \section intro 序説
 * これはAVR205応用記述用のソフトウェアの資料です。
 *
 * \section compinfo コンパイラ情報
 * このソフトウェアはWinAVR GCC 20100110でコンパイルされました。
 *
 * \section deviceinfo デバイス情報
 * mega48,mega88,mega168とtiny261,tiny461,tiny861のようなATmegaxxx8/Tinyx61
 *
 * \section codesize コード量
 * 368プログラム バイト、2 SRAMバイト
 *
 * \section contactinfo 問い合わせ情報
 * ATMEL AVRについてのより多くの情報についてはhttp://www.atmel.com/products/AVR/を訪ねてください。\n
 * 応用記述についてはhttp://www.atmel.com/dyn/products/app_notes.asp?family_id=607を訪ねてください。\n
 * 支援メール: avr@atmel.com
 */

//! 原型

/*! \brief AVR周辺機能の初期化関数
 *
 * この関数はやって来る信号の計数に使用されるタイマ/カウンタを初期化します。
 *
 *  \note 周辺機能は先にまたは実演走行で初期化されなければなりません。
 *
 */
void freq_cntr_init(void);

/*! \brief 使用者の初期化関数
 *
 * これは使用者がそれらの周辺機能と変数を初期化する実演コード内に置くことを示唆されます。
 *
 *  \note 周辺機能は先に初期化されなければなりません。
 *
 */
void users_init(void);

/*! \brief 割り込みが使用される時に、この関数は測定処理を始めるのに呼ばれます。\n
 * \T0のピン変化の状態が16ビット周波数計数器を始めさせます。\n
 *
 *  - この関数は多忙待ち(ポーリング)中に使用されません。\n
 *
 */
void freq_cntr_start_measurement(void);

/*! \brief 多忙待ち(USE_INTERRUPTS FALSE)が使用される時に、この関数は測定処理を始めるのに呼ばれます。\n
 * \T0のピン変化の状態が16ビット周波数計数器を始めさせます。\n
 *
 *  - この関数は割り込み使用時に使用されません。\n
 *
 */
unsigned int freq_cntr_get_frequency(void);

/*! \brief 多忙待ち(ポーリング)例が使用される時に、この関数はCPU待機繰り返しです。\n
 * \unsigned char内に値を設定することによって100ms単位の遅延を指定することができます。\n
 */
void delay100ms(unsigned char);

/*! \brief 周波数計数が$FFFFの大きさでないことを調べます。\n
 * \溢れでない結果なら、freq_cntr_freq_divd_by_10の16ビット符号なし変数で返されます。\n
 * \$FFFFに達していたなら、これは溢れ状態と見做され、故に結果は$FFFFとして返されます。\n
 */
void ovf_test_and_return_result(void);

/*! \brief 割り込みが使用される時に、この関数は測定処理を始めるのに呼ばれます。\n
 * \T0のピン変化の状態が16ビット周波数計数器を始めさせます。\n
 *
 *  - この関数は多忙待ち(ポーリング)中に使用されません。\n
 */
unsigned int freq_cntr_get_result(void);

/*! \brief これらの定義は含まれる実演コードの多忙待ち版と割り込み版の選択に使用されます。\n
 * 割り込み版についてはINTERRUPT変数をTRUEに、多忙待ち(ポーリング)についてはINTERRUPT変数をFALSEに設定してください。\n
 */
#define TRUE				1
#define FALSE				0

#ifndef F_CPU
/*! \brief これが既に定義されていない場合の既定CPU周波数定義 */
#define F_CPU 8000000UL
#endif

/* 100msの遅延を生成するために実験的に数値5560が決められ、違うF_CPUクロック速度には変更されなければなりません。\n
 */
#define DELAY_VAL_100_MS	0xFFFF-(unsigned int)((F_CPU/1000000)*5560L)

#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)	// これはGCCで動きます。

// 以下はタイマ/カウンタ0を16ビット計数器として使用し、タイマ/カウンタ1を開門計時器として使用して100msで動くように設定します。
#define FREQ_CNTR_CTRLA			TCCR0A
#define FREQ_CNTR_CTRLB			TCCR0B
#define GATE_CNTR_CTRLB			TCCR1B
#define TC_COUNTMODE			TCW0
#define PIN_CHG_MASK			PCMSK1
#define PIN_CHG_INT				PCINT14
#define GEN_INT_MASK			GIMSK
#define PIN_CHG_INT_ENAB		PCIE1
#define PIN						PINB
#define PIN_NO					PB6
#define	FREQ_CNTR_COUNT_LOW		TCNT0L
#define FREQ_CNTR_COUNT_HIGH	TCNT0H
#define FREQ_CNTR_INT_FLAG_REG	TIFR	// 独立したTIFR0とRIFR1はありません。
#define GATE_CNTR_INT_FLAG_REG	TIFR
#define	FREQ_CNTR_OVF_FLAG		TOV0
#define GATE_CNTR_OVF_FLAG		TOV1
#define	GATE_CNTR_COUNT			TCNT1
#define	GATE_CNTR_COUNT_HI		TC1H	// tiny x61のみ
#define	GATE_CNTR_PRELOAD		255-(unsigned int)(F_CPU/(4096L*10L))	// 255
#define GATE_CNTR_INT_MASK_REG	TIMSK
#define GATE_CNTR_OVF_INT_ENAB	TOIE1
#define CLOCK_SEL_00			CS00
#define CLOCK_SEL_1				CS01
#define CLOCK_SEL_2				CS02
#define CLOCK_SEL_10			CS10
#define CLOCK_SEL_12			CS12
#define CLOCK_SEL_13			CS13
#define GEN_INT_FLG_REG			GIFR
#define PIN_CHG_INT_FLAG		PCIF
#define GATE_CNTR_OVF_vect		TIMER1_OVF_vect
#define PIN_CHG_vect			PCINT_vect
#endif

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)

// 以下はタイマ/カウンタ1を16ビット計数器として使用し、タイマ/カウンタ0を開門計時器として使用して100msで動くように設定します。
#define FREQ_CNTR_CTRLA			TCCR1A
#define FREQ_CNTR_CTRLB			TCCR1B
#define GATE_CNTR_CTRLB			TCCR0B
#define TC_COUNTMODE			TCW0
#define PIN_CHG_MASK			PCMSK2
#define PIN_CHG_INT				PCINT21
#define GEN_INT_MASK			PCICR
#define PIN_CHG_INT_ENAB		PCIE2
#define PIN						PIND
#define PIN_NO					PD5
#define FREQ_CNTR_INT_FLAG_REG	TIFR1
#define	FREQ_CNTR_OVF_FLAG		TOV1
#define GATE_CNTR_INT_FLAG_REG	TIFR0
#define	GATE_CNTR_OVF_FLAG		TOV0
#define	FREQ_CNTR_COUNT_LOW		TCNT1L
#define	FREQ_CNTR_COUNT_HIGH	TCNT1H
#define	GATE_CNTR_COUNT			TCNT0
#define	GATE_CNTR_PRELOAD		255-195
#define GATE_CNTR_INT_MASK_REG	TIMSK0
#define GATE_CNTR_OVF_INT_ENAB	TOIE0
#define CLOCK_SEL_00			CS00
#define CLOCK_SEL_02			CS02
#define CLOCK_SEL_1				CS11
#define CLOCK_SEL_2				CS12
#define CLOCK_SEL_10			CS10
#define CLOCK_SEL_12			CS12
#define CLOCK_SEL_13			CS13
#define GEN_INT_FLG_REG			GIFR
#define PIN_CHG_INT_FLAG		PCIF
#define GATE_CNTR_OVF_vect		TIMER0_OVF_vect
#define PIN_CHG_vect			PCINT2_vect
#endif
