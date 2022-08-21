#include "PBToggle.h"

int PBToggle(void) {
  PORTB = ~PORTB;
  return 0;
}

