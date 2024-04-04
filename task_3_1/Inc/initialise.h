
#ifndef INITIALISE_HEADER
#define INITIALISE_HEADER

#include <stdint.h>

// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks();

// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board();

#endif
