
#include <stdint.h>
#include "stm32f303xc.h"

#include "timer.h"
#include "led.h"

int main(void)
{
	// enable components
	enable_clocks();
	initialise_leds();
	enable_timer_interrupt();

	// initialise timer with set period between interrupts
	uint32_t period = 5000; // ms
	TimerInitialise(&TIM2_init, period, &chase_led);

    /* Loop forever */
	for(;;);
}
