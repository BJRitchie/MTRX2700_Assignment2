
#include <stdint.h>
#include "stm32f303xc.h"

#include "timer.h"
#include "initialise.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void chase_led(void){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	*led_register <<= 1;
	if (*led_register == 0) {
		*led_register = 1;
	}
}

int main(void) {
	// enable components
	enable_clocks();
	initialise_board();
	enable_timer_interrupt();

	setPeriod(&TIM2_init, 1000000);
	TimerInitialise(&TIM2_init, &chase_led);

    /* Loop forever */
	for(;;);
}