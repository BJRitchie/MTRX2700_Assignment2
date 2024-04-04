/*
 * Notes:
 *
 * (pg 294) Hardware interrupt selection:
 * 	To configure a line as interrupt source, use the following procedure:
 *		• Configure the corresponding mask bit in the EXTI_IMR register.
 *		• Configure the Trigger Selection bits of the Interrupt line (EXTI_RTSR and EXTI_FTSR)
 *		• Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
 *		  EXTI so that an interrupt coming from one of the EXTI line can be correctly
 *		  acknowledged.
 *
 *
 *
 * One pulse mode:
 * - see pg 647, TIMx_CR1
 *
 * STM32 tutorials:
 * https://github.com/dekuNukem/STM32_tutorials/blob/master/lesson4_timers_and_pwm/README.md
 *
 */


/*
 Plan:

 Prep:
 - define overflow param

 - define interrupt program
 	 - TIM2_IRQHandler
 	 - check cause
 	 	 - if overflow (UIF flag), do x

 - enable interrupts
 	 - TIM2 overflow interrupt
 	 1. Config mask bit in EXTI_IMR register
 	 2. Config trigger selection bits of Interrupt Line (EXTI_RTSR and EXTI_FTSR)
 	 3. Config the enable and mask bits for NVIC IRQ channel mapped to EXTI


 Program:
 - wait until interrupt is flagged

 */

#include <stdint.h>
#include "stm32f303xc.h"

#include "timer.h"
#include "initialise.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// make a function pointer
void (*on_timer_overflow)() = 0x00;

// function called when interrupt is flagged for TIM2
void TIM2_IRQHandler() {
    // Check if the interrupt is due to an overflow
    if (TIM2->SR & TIM_SR_UIF) {
        // Clear the update interrupt flag
        TIM2->SR &= ~TIM_SR_UIF;

        // Run the overflow handler
        if (on_timer_overflow != 0x00) { // make sure the handler is not null
            on_timer_overflow();
        }
    }
}


void chase_led(){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	*led_register <<= 1;
	if (*led_register == 0) {
		*led_register = 1;
	}
}

int main(void)
{
	// enable components
	enable_clocks();
	initialise_board();

	// set the interrupt handling function:
	on_timer_overflow = &chase_led;

	// initialise timer with set period between interrupts
	uint32_t period = 1000; // ms
	TIM2_interrupt_init(period);

    /* Loop forever */
	for(;;);
}
