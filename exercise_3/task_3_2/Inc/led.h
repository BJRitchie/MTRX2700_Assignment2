
#ifndef LED_HEADER
#define LED_HEADER

#include <stdint.h>

// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_leds();

// code from lecture example W05-C-interrupt by Dr Stewart Worrall
// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks();

// chase the led around a circle
void chase_led();

#endif
