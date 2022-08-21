#include "AvrInit.h"

int AvrInit(void) {
  DDRB = DDRB | 0b11111111; // Set PBx to output
  PORTB = 11111111; // Set PBx output to LOW
  return 0;
}

