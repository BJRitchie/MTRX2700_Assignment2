
#include <stdint.h>
#include "stm32f303xc.h"

#include "timer.h"
#include "initialise.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void one_shot(void) {
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 0xFF;
}

int main(void) {
	// enable components
	enable_clocks();
	initialise_board();
	enable_timer_interrupt();

//	setPeriod(&TIM2_init, 1000000);
	TimerInitialise(&TIM2_init, 0x00);
	set_one_shot_mode(&TIM2_init, 1000000, &one_shot);

    /* Loop forever */
	for(;;);
}
