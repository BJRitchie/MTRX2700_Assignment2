#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <led.h>
#include <serial.h>
#include <timer.h>
#include <integration.h>

#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void) {
	enable_clocks();
	initialise_leds();
	enable_timer_interrupt();

    SerialInitialise(BAUD_115200, &USART1_PORT, finished_transmission, received_new_string);

    for (;;) {
    }
}
