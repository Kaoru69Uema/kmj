#include <compat/ina90.h>		// ここでは_NOP, _CLI, _SEIが定義されます。
#include <avr/interrupt.h>
#include "freq_meter.h"

unsigned int	freq_cntr_result;
void users_init(void);

int main() {
  freq_cntr_init();
  users_init();
  _SEI();

  freq_cntr_start_measurement();
  while (1)
  {
	if(freq_cntr_get_result() !=0) {

	freq_cntr_result = freq_cntr_get_result();
	freq_cntr_clear_result();

	_NOP();

	freq_cntr_start_measurement();

	}
	_NOP();
  }
}


void users_init(void)
{
  _NOP();
}
