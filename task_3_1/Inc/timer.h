
#ifndef TIMER_HEADER
#define TIMER_HEADER

#include <stdint.h>

// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks();

/* function to initialise the timer with interrupt at overflow
 * Inputs:
 * - int period: desired period between actions in milliseconds
 *
 * Outputs:
 * - None */
void TIM2_interrupt_init(uint32_t period);

#endif
