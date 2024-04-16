#include <stddef.h>
#include "digital_io.h"
#include "stm32f303xc.h"

static ButtonCallback button_callback = NULL;
static uint8_t led_index = 0;

void EXTI0_IRQHandler(void) {
    if (button_callback != NULL) {
        button_callback();
    }
    EXTI->PR |= EXTI_PR_PR0; // Clear interrupt pending bit
}

void digital_io_init(ButtonCallback callback) {
    button_callback = callback;
    led_index = 0;

    // Enable clocks for GPIOA and GPIOE
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;

    // Initialize LED pins (PE8-PE15) as outputs
    GPIOE->MODER |= 0x55550000; // Set PE8-PE15 to general purpose output mode

    // Initialize button pin (PA0) as input
    GPIOA->MODER &= ~(GPIO_MODER_MODER0); // Set PA0 to input mode

    // Enable SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Connect EXTI0 line to PA0
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;

    // Configure EXTI0 line to trigger on rising edge
    EXTI->RTSR |= EXTI_RTSR_TR0;

    // Unmask EXTI0 line
    EXTI->IMR |= EXTI_IMR_MR0;

    // Set interrupt priority and enable EXTI0 interrupt
    NVIC_SetPriority(EXTI0_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void set_led(uint8_t led_num) {
    GPIOE->BSRR = (1 << (led_num + 8)); // Set corresponding bit to turn on LED
}

void clear_led(uint8_t led_num) {
    GPIOE->BSRR = (1 << (led_num + 8 + 16)); // Set corresponding bit to turn off LED
}

void toggle_led(uint8_t led_num) {
    GPIOE->ODR ^= (1 << (led_num + 8)); // Toggle corresponding LED
}

void set_button_handler(ButtonCallback handler) {
    button_callback = handler;
}
void chase_led() {
    clear_led(led_index);       // Clear the currently lit LED
    led_index = (led_index + 1) % 8;  // Move to the next LED
    set_led(led_index);         // Set the next LED
}
