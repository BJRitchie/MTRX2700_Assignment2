
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

// Set the led to on
void set_led(uint8_t led_num);

// Clear the led
void clear_led(uint8_t led_num);

// Toggle the led on or off depending on current state
void toggle_led(uint8_t led_num);

// chases an led in a circle with increasing speed
void chase_led_changing_period();

#endif
