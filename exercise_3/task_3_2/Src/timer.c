
#include "timer.h"
#include "stm32f303xc.h"


struct _HardwareTimer {
	TIM_TypeDef *TIMx;
	volatile uint16_t PrescaleValue;	// prescaler value
	volatile uint32_t ARRValue; 		// prescaler value
	volatile uint32_t MaskAPB2ENR;		// mask to enable RCC APB2 bus registers
	volatile uint32_t MaskAPB1ENR;		// mask to enable RCC APB1 bus registers
	volatile uint32_t MaskAHBENR;		// mask to enable RCC AHB bus registers
	volatile uint32_t MaskCR1; 			// mask to apply to CR1 (control reg 1)
	volatile uint32_t MaskDIER; 		// mask to apply to DIER (DMA/interrupt enable register)
	void (*timer_completion_function)(); 	// the the code block to be run when the interrupt is flagged
};

HardwareTimer TIM2_init = {
		TIM2,
		0, 					// default prescaler value
//		4294967296-1, 		// default overflow value
		100,
		0x00, 				// bit to enable APB2 bus
		RCC_APB1ENR_TIM2EN, // bit to enable APB1 bus
		0x00, 				// bit to enable for AHB bus
		TIM_CR1_URS, 		// bit for CR1 reg; interrupt only flagged at over/underflow
		TIM_DIER_UIE, 		// enable update interrupt
		0x00,
};

// initialise a hardware timer
void TimerInitialise(HardwareTimer *hardware_timer, void (*completion_function)()) {

    // Enable clock
    RCC->APB1ENR |= hardware_timer->MaskAPB1ENR;

    // set prescaler
    hardware_timer->TIMx->PSC = hardware_timer->PrescaleValue;

    // set auto reload register
    hardware_timer->TIMx->ARR = hardware_timer->ARRValue;

    // set CR1 mask
    hardware_timer->TIMx->CR1 |= hardware_timer->MaskCR1;

    // set DIER mask
    hardware_timer->TIMx->DIER |= hardware_timer->MaskDIER;

    // start TIMx
    hardware_timer->TIMx->CR1 |= TIM_CR1_CEN;

    // set the timer completion function
    hardware_timer->timer_completion_function = completion_function;

}

void TIM2_IRQHandler(void){
	// Check if the interrupt is due to an overflow
	if (TIM2->SR & TIM_SR_UIF) {
		// Clear the update interrupt flag
		TIM2->SR &= ~TIM_SR_UIF;

		// Run the overflow handler
		if (TIM2_init.timer_completion_function != 0x00) {
			TIM2_init.timer_completion_function();
		}
	}
}

// enable the timer overflow interrupt
void enable_timer_interrupt() {
    // disable interrupts
	__disable_irq();

	NVIC_SetPriority(TIM2_IRQn, 1); // Set priority
    NVIC_EnableIRQ(TIM2_IRQn); 		// Enable TIM2 IRQ

    // re-enable interrupts
    __enable_irq();
}


// getter and setter functions for timer

// gets the ARRValue
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// ouput:
// - prescaler value of timer
uint16_t getPrescaleValue(struct _HardwareTimer *hardware_timer){
	return hardware_timer->PrescaleValue;
}

// set the auto-reload register value
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - value: the desired prescaler to be set
void setPrescaleValue(struct _HardwareTimer *hardware_timer, uint16_t value) {
	// set value
	hardware_timer->PrescaleValue = value;
    hardware_timer->TIMx->PSC = value;

	// reset timer to engage clock
	hardware_timer->TIMx->EGR |= 0x01;
}

// gets the ARRValue
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// ouput:
// - ARRValue
uint32_t getARRValue(struct _HardwareTimer *hardware_timer){
	return hardware_timer->ARRValue;
}

// set the auto-reload register value
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - value: the desired ARRValue to be set
void setARRValue(struct _HardwareTimer *hardware_timer, uint32_t value) {
	// set value
	hardware_timer->ARRValue = value;
    hardware_timer->TIMx->ARR = value;
}


// Set a period between clock overflows and therefore interrupts .
// Will also reset clock.
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - period: the delay period in microseconds
void setPeriod(struct _HardwareTimer *hardware_timer, uint32_t period) {
	setPrescaleValue(hardware_timer, 8-1); 	// make clock freq 1MHz
	setARRValue(hardware_timer, period);  	// set overflow to period value
}




