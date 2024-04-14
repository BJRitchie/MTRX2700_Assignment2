
#include <stdint.h>
#include "stm32f303xc.h"

#include "timer.h"
#include "led.h"

int main(void) {
	// enable components
	enable_clocks();
	initialise_leds();
	enable_timer_interrupt();

	// initialise the timer
	TimerInitialise(&TIM2_init, &chase_led);

    /* Loop forever */
	for (;;);
}
