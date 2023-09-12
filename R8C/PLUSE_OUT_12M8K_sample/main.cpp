//=====================================================================//
/*!	@file
  @brief	R8C タイマーJ、パルス出力サンプル @n
  P17(13) からパルス出力。@n
  コンソールから、任意の周波数を設定（有限な設定範囲）
  @author 平松邦仁 (hira@rvf-rc45.net)
  @copyright	Copyright (C) 2017, 2021 Kunihito Hiramatsu @n
  Released under the MIT license @n
https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"

#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/trb_io.hpp"
#include "common/trj_io.hpp"
#include "common/command.hpp"

namespace {

	typedef utils::fifo<uint8_t, 16> BUFFER;
	typedef device::uart_io<device::UART0, BUFFER, BUFFER> UART;
	UART	uart_;

	typedef device::trb_io<utils::null_task, uint8_t> TIMER_B;
	TIMER_B timer_b_;

	typedef device::trj_io<utils::null_task> TIMER_J;
	TIMER_J timer_j_;

	utils::command<64> command_;

}

extern "C" {

	void sci_putch(char ch) {
		uart_.putch(ch);
	}


	char sci_getch(void) {
		return uart_.getch();
	}


	uint16_t sci_length() {
		return uart_.length();
	}


	void sci_puts(const char* str) {
		uart_.puts(str);
	}


	void TIMER_RB_intr(void) {
		timer_b_.itask();
	}


	void UART0_TX_intr(void) {
		uart_.isend();
	}


	void UART0_RX_intr(void) {
		uart_.irecv();
	}


	void TIMER_RJ_intr(void) {
		timer_j_.iout();
	}
}


namespace {

	bool get_dec_(const char* text, uint32_t& val) {
		val = 0;
		char ch;
		while((ch = *text++) != 0) {
			if(ch >= '0' && ch <= '9') {
				ch -= '0';
			} else {
				return false;
			}
			val *= 10;
			val += ch;
		}
		return true;
	}


	bool get_decimal_(uint8_t no, uint32_t& val) {
		char buff[9];
		if(command_.get_word(no, sizeof(buff), buff)) {
			if(get_dec_(buff, val)) {
				return true;
			}
		}
		return false;
	}


	bool help_(uint8_t cmdn) {
		if(cmdn >= 1 && command_.cmp_word(0, "help")) {
			sci_puts("freq FREQUENCY[Hz]\n");
			return true;
		}
		return false;
	}


	bool freq_(uint8_t cmdn) {
		if(cmdn >= 2) {
			uint32_t val;
			if(get_decimal_(1, val)) {
				if(!timer_j_.set_cycle(val)) {
					sci_puts("TRJ out of range.\n");
				}
				return true;
			}
		}
		return false;
	}
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
	// すでに外部クロックは発信中なので発振安定待ちはしない
	SCKCR.HSCKSEL = 0;      // 高速クロック(XIN)選択
	PHISEL = 0;             // システム基準クロックの分周なし
	CKSTPR.SCKSEL = 1;      // 高速クロック選択
	SCKCR &= 0x60;          // CPU ：クロック システムクロックの分周なし
	PRCR.PRC0 = 0;          // プロテクト
	asm("FSET I");          // 割り込み許可


	// タイマーＢ初期化
	{
		uint8_t ir_level = 2;
		timer_b_.start(60, ir_level);
	}

	// UART の設定 (P1_4: TXD0[in], P1_5: RXD0[in])
	// ※シリアルライターでは、RXD 端子は、P1_6 となっているので注意！
	{
		utils::PORT_MAP(utils::port_map::P14::TXD0);
		utils::PORT_MAP(utils::port_map::P15::RXD0);
		uint8_t ir_level = 1;
		uart_.start(9600, ir_level);
	}

	// TRJ のパルス出力設定
	{
		utils::PORT_MAP(utils::port_map::P17::TRJIO);
		if(!timer_j_.pluse_out(1000)) {
			sci_puts("TRJ out of range.\n");
		}
	}

	sci_puts("Start R8C PLUSE output sample\n");

	command_.set_prompt("# ");

	while(1) {
		timer_b_.sync();

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			uint8_t cmdn = command_.get_words();
			if(cmdn == 0) ;
			else if(help_(cmdn)) ;
			else if(freq_(cmdn)) ;
			else {
				sci_puts("Command error: ");
				sci_puts(command_.get_command());
				sci_putch('\n');
			}
		}
	}
}
