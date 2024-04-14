
#include <stdint.h>
#include "stm32f303xc.h"

#include "timer.h"
#include "led.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void) {
	// enable components
	enable_clocks();
	initialise_leds();
	enable_timer_interrupt();

	uint32_t delay = 1000; // delay in milliseconds
	setOneShotMode(&TIM2_init, delay, &led_on); // set timer to one shot mode

	delay = 5000;
	setOneShotMode(&TIM4_init, delay, &led_off); // turn off timers after 1 second

    /* Loop forever */
	for(;;);
}
