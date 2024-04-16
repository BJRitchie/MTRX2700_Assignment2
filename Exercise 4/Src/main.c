#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Include relevant header files
#include <led.h>
#include <serial.h>
#include <timer.h>
#include <integration.h>

#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void) {
// Enabling clocks for timers
	enable_clocks();
// Initialising the LED pins
	initialise_leds();
// Enabling the timer interrupt
	enable_timer_interrupt();

// Initialising the serial module
    SerialInitialise(BAUD_115200, &USART1_PORT, finished_transmission, received_new_string);

// Loop indefinitely 
    for (;;) {
    }
}
