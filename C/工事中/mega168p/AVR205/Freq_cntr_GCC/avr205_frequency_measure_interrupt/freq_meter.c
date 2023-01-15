#include <avr/io.h>
#include <compat/ina90.h>		// ここでは_NOP, _CLI, _SEIが定義されます。
#include <avr/interrupt.h>
#include "freq_meter.h"

unsigned int 	x48_cnt_to_four_ints;

unsigned int	freq_cntr_freq_divd_by_10;

void freq_cntr_init(void) {
	TCCR1A = 0;			/* Timer1: OverFlow */
}

void freq_cntr_start_measurement(void){
	PCMSK2 = (1<<PCINT21);          /* ﾋﾟﾝ変化割り込み許可ﾚｼﾞｽﾀ2: PD5 */
	PCICR = (1<<PCINT21);           /* ﾋﾟﾝ変化割り込み制御ﾚｼﾞｽﾀ */
}

unsigned int	freq_cntr_get_result(void)
{
	return freq_cntr_freq_divd_by_10;
}

void	freq_cntr_clear_result(void)
{
	freq_cntr_freq_divd_by_10 = 0;
}

ISR(PIN_CHG_vect) {

	if ((PIND & (1<<PD5)) !=0)
	{
		PCMSK2	= (0<<PCINT21);
		PCICR &= ~(1<<PCINT21);
		GATE_CNTR_INT_FLAG_REG	 |= (1<<GATE_CNTR_OVF_FLAG);

		FREQ_CNTR_COUNT_HIGH  = 0x00;
		FREQ_CNTR_COUNT_LOW  = 0x00;
		TCCR1B = (1<<CLOCK_SEL_2)|(1<<CLOCK_SEL_1);

		GATE_CNTR_COUNT = 0;

		GATE_CNTR_INT_MASK_REG = (1<<GATE_CNTR_OVF_INT_ENAB);
		TCCR0B = (1<<CLOCK_SEL_02)|(1<<CLOCK_SEL_00);
		x48_cnt_to_four_ints = 0;
	}
}


ISR(GATE_CNTR_OVF_vect) {

	x48_cnt_to_four_ints++;
	if(x48_cnt_to_four_ints == 3)GATE_CNTR_COUNT = 0xff-13;
	if(x48_cnt_to_four_ints >= 4)
	{
		TCCR1B = 0;
		TCCR0B = 0;

		if ((FREQ_CNTR_INT_FLAG_REG & (1<<FREQ_CNTR_OVF_FLAG)) !=0) {
			freq_cntr_freq_divd_by_10 = 0xFFFF;
		}
		else {
			freq_cntr_freq_divd_by_10 = FREQ_CNTR_COUNT_LOW+(FREQ_CNTR_COUNT_HIGH<<8);
		}
	}
}
