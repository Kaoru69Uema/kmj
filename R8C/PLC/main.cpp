/*
   作者：上間つよし
   平松さんのコード使用：ありがとうございます。
PLC:ミニシーケンサ
2023年9月8日（令和5年9月8日）作成開始
*/
/*
   @author 平松邦仁 (hira@rvf-rc45.net)
   @copyright	Copyright (C) 2017, 2021 Kunihito Hiramatsu @n
   Released under the MIT license @n
https://github.com/hirakuni45/R8C/blob/master/LICENSE
*/
#include "common/renesas.hpp"
#include "common/format.hpp"
#include "common/intr_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/trb_io.hpp"

enum class OP {
    END,     // プログラム終了：最初へ戻る
    OUT,     // コイル駆動命令
    LD,     // 接続命令 a接点
    LDI,     // 接続命令 b接点
    AND,     // 直列接続 a接点
    ANI,     // 直列接続 b接点
    OR,     // 並列接続 a接点
    ORI,     // 並列接続 b接点
    ANB,     // ブロック間直列接続
    ORB,     // ブロック間並列接続
    SET,     // 動作保持コイル命令
    RST,     // 動作保持解除コイル命令
    FF,     // フリップフロップ：トグル
    PLS,     // １周期分パルス生成
    SP,     // カウンタ、タイマ（）、タイマ（蓄積）、の値（4095以下）
    NOP,     // 無処理：プログラムの消去・スペース用
};

namespace {

	typedef utils::fifo<uint8_t, 16> TX_BUFFER; // 送信
	typedef utils::fifo<uint8_t, 16> RX_BUFFER; // 受信
	typedef device::uart_io<device::UART0,TX_BUFFER,RX_BUFFER> UART;
	UART	uart_;

	// 入力：X：　ONしたら0[v]、OFFは5[v]なので論理反転
	typedef device::PORT<device::PORT4, device::bitpos::B2, false> X0;
	typedef device::PORT<device::PORT3, device::bitpos::B7, false> X1;
	typedef device::PORT<device::PORT4, device::bitpos::B7, false> X2;
	typedef device::PORT<device::PORT3, device::bitpos::B5, false> X3;
	typedef device::PORT<device::PORT3, device::bitpos::B4, false> X4;
	typedef device::PORT<device::PORT3, device::bitpos::B3, false> X5;
	typedef device::PORT<device::PORT4, device::bitpos::B5, false> X6;
	typedef device::PORT<device::PORT1, device::bitpos::B7, false> X7;

	// 出力：Y：　0[v]=点灯、5[v]=消灯なので論理反転
	typedef device::PORT<device::PORT1, device::bitpos::B0, false> Y0;
	typedef device::PORT<device::PORT1, device::bitpos::B1, false> Y1;
	typedef device::PORT<device::PORT1, device::bitpos::B2, false> Y2;
	typedef device::PORT<device::PORT1, device::bitpos::B3, false> Y3;
	typedef device::PORT<device::PORT1, device::bitpos::B6, false> Y4;

	volatile uint16_t trb_count_;

	/// タイマー割り込みで実行する動作の定義
	class trb_intr_task {
		public:
			void operator() () {
				++trb_count_;
			}
	};

	typedef device::trb_io<trb_intr_task, uint8_t> TIMER_B;
	TIMER_B	timer_b_;

}


extern "C" {

	void sci_putch( char ch ) {
		uart_.putch( ch );
	}

	char sci_getch( void ) {
		return uart_.getch();
	}

	uint16_t sci_length( void ) {
		return uart_.length();
	}

	void sci_puts( const char* str ) {
		uart_.puts( str );
	}


	void TIMER_RB_intr( void ) {
		timer_b_.itask();
	}

	void UART0_TX_intr() {
		uart_.isend();
	}

	void UART0_RX_intr() {
		uart_.irecv();
	}


}

namespace {
	uint8_t inp_lvl_ = 0;
	uint8_t inp_pos_ = 0;
	uint8_t inp_neg_ = 0;

//	void input_service_()
//	{
//		uint8_t lvl = X0::P() << 0|X1::P() << 1|X2::P() << 2|X3::P() << 3|X4::P() << 4|X5::P() << 5|X6::P() << 6|X7::P() << 7;
//		inp_pos_ = ~inp_lvl_ &  lvl; /// 立ち上がりエッジ検出
//		inp_neg_ =  inp_lvl_ & ~lvl; /// 立ち下がりエッジ検出
//		inp_lvl_ =  lvl;             /// 状態の保存
//	}
}
int main(int argc, char *argv[])
{
	using namespace device;

	/**********************/
	/*   XIN=20MHｚ設定   */
	/**********************/

	asm("FCLR I");          // 割り込み禁止
	PRCR.PRC0 = 1;          // プロテクト解除
	OCOCR.LOCODIS = 0;      // 低速オンチップオシレータ発信
	EXCKCR.CKPT   = 1;      // 外部クロック(XIN)入力, I/Oポート選択(P4_7)
	for( volatile uint8_t i= 0; i<2040; ++i ) ; // 発振安定待ち
	SCKCR.HSCKSEL = 0;      // 高速クロック(XIN)選択
	PHISEL = 0;             // システム基準クロックの分周なし
	CKSTPR.SCKSEL = 1;      // 高速クロック選択
	SCKCR &= 0x60;          // CPU ：クロック システムクロックの分周なし
	PRCR.PRC0 = 0;          // プロテクト
	asm("FSET I");          // 割り込み許可
	// タイマーBの設定
	{
		uint8_t intr_level = 1;
		uint16_t freq = 1000-1; // 1000Hz
		timer_b_.start(freq, intr_level);
	}

	{
		utils::PORT_MAP( utils::port_map::P14::TXD0 );
		utils::PORT_MAP( utils::port_map::P15::RXD0 );
		uint8_t ir_level = 1;
		uart_.start( 9600, ir_level );
	}
	// 入力・出力の設定
	{
		X0::DIR = 0; // 入力
		X1::DIR = 0;
		X2::DIR = 0;
		X3::DIR = 0;
		X4::DIR = 0;
		X5::DIR = 0;
		X6::DIR = 0;
		X7::DIR = 0;
		X0::PU  = 1; // プルアップ
		X1::PU  = 1;
		X2::PU  = 1;
		X3::PU  = 1;
		X4::PU  = 1;
		X5::PU  = 1;
		X6::PU  = 1;
		X7::PU  = 1;
		Y0::DIR = 1; // 出力
		Y1::DIR = 1;
		Y2::DIR = 1;
		Y3::DIR = 1;
		Y4::DIR = 1;
		Y0::P   = 0; // 出力OFF
		Y1::P   = 0; // 出力OFF
		Y2::P   = 0; // 出力OFF
		Y3::P   = 0; // 出力OFF
		Y4::P   = 0; // 出力OFF
	}

	// PLC スタート
	sci_puts( "Start LCD. Enjoy! By Uema Tsuyoshi\n" );	

	uint8_t cnt = 0;
	while(1) {
		timer_b_.sync();
		//input_service_();

//		if( ( inp_pos_ & 0b00001 ) != 0 ) {     // 押した
//			sci_puts( "X0 - positive\n" );
//			Y0::P = 1;
//		}
//
//		if( ( inp_pos_ & 0b00010 ) != 0 ) {     // 押した
//			sci_puts( "X1 - positive\n" );
//			Y1::P = 1;
//		}
//
//		if( ( inp_pos_ & 0b00100 ) != 0 ) {     // 押した
//			sci_puts( "X2 - positive\n" );
//			Y2::P = 1;
//		}
//
//		if( ( inp_pos_ & 0b01000 ) != 0 ) {     // 押した
//			sci_puts( "X3 - positive\n" );
//			Y3::P = 1;
//		}
//
//		if( ( inp_pos_ & 0b10000 ) != 0 ) {     // 押した
//			sci_puts( "X4 - positive\n" );
//			Y4::P = 1;
//		}
//
//		if( ( inp_pos_ & 0b00100000 ) != 0 ) {     // 押した
//			sci_puts( "X5 - positive\n" );
//			Y4::P = 1;
//		}
//
//		if( ( inp_pos_ & 0b01000000 ) != 0 ) {     // 押した
//			sci_puts( "X6 - positive\n" );
//			Y4::P = 1;
//		}
//
//		if( ( inp_pos_ & 0b10000000 ) != 0 ) {     // 押した
//			sci_puts( "X7 - positive\n" );
//			Y4::P = 1;
//		}
//
//		if( ( inp_neg_ & 0b00001 ) != 0 ) {     // 離した
//			sci_puts( "X0 - negative\n" );
//			Y0::P = 0;
//		}
//
//		if( ( inp_neg_ & 0b00010 ) != 0 ) {     // 離した
//			sci_puts( "X1 - negative\n" );
//			Y1::P = 0;
//		}
//
//		if( ( inp_neg_ & 0b00100 ) != 0 ) {     // 離した
//			sci_puts( "X2 - negative\n" );
//			Y2::P = 0;
//		}
//
//		if( ( inp_neg_ & 0b01000 ) != 0 ) {     // 離した
//			sci_puts( "X3 - negative\n" );
//			Y3::P = 0;
//		}
//
//		if( ( inp_neg_ & 0b10000 ) != 0 ) {     // 離した
//			sci_puts( "X4 - negative\n" );
//			Y4::P = 0;
//		}
//
//		if( ( inp_neg_ & 0b100000 ) != 0 ) {     // 離した
//			sci_puts( "X5 - negative\n" );
//			Y4::P = 0;
//		}
//
//		if( ( inp_neg_ & 0b1000000 ) != 0 ) {     // 離した
//			sci_puts( "X6 - negative\n" );
//			Y4::P = 0;
//		}
//
//		if( ( inp_neg_ & 0b10000000 ) != 0 ) {     // 離した
//			sci_puts( "X7 - negative\n" );
//			Y4::P = 0;
//		}

		if( cnt % 2 )
			Y0::P=1; 
		else
			Y0::P=0;
		++cnt;
		if(cnt >= 1000-1) cnt = 0;
	}
}
