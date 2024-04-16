#include "led.h"
#include "timer.h"
#include "stm32f303xc.h"

// code from lecture example W05-C-interrupt by Dr Stewart Worrall
// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_leds() {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

// code from lecture example W05-C-interrupt by Dr Stewart Worrall
// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}

// chase the led around the circle (also by Dr Stewart Worrall)
void chase_led(){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	*led_register <<= 1;
	if (*led_register == 0) {
		*led_register = 1;
	}
}

void led_on(void) {
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 0xFF;
}

void led_off(void) {
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 0x00;
}

// chase the led around the circle while decreasing the period
void chase_led_changing_period(){
	// get the period
	uint32_t period = getPeriod(&TIM2_init);

	period -= 25;
	if (period <= 25) {
		period = 1000; // reset
	}
	setPeriod(&TIM2_init, period );

	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	*led_register <<= 1;
	if (*led_register == 0) {
		*led_register = 1;
	}
}
