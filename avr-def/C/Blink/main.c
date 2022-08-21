#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include "PBToggle.h"
#include "AvrInit.h"

int main() {
  // Setup
  AvrInit();

  // Main loop
  while(1) {
    PBToggle();
    _delay_ms(250);
  }

  return 0;
}

