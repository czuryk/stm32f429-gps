#include "swo.h"
#include <stdio.h>

// SWO Core clock: 60000000

struct __FILE { int handle; };
FILE __stdout;
FILE __stdin;


int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch; // Using SWO port 0
  }
  return(ch);
}
