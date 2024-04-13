#include <stddef.h>
#include "digital_io.h"
#include "stm32f303xc.h"

void (*button_handler)() = 0x00;

void EXTI0_IRQHandler(void) {
    if (button_handler != 0x00) {
        button_handler();
    }
    EXTI->PR |= EXTI_PR_PR0; // Reset interrupt
}

void digital_io_init() {
    // Enable clocks for GPIOA and GPIOE
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;

    // Initialize LED pins (PE8-PE15) as outputs
    GPIOE->MODER &= ~0xFFFF0000; // Clear mode bits for pins PE8-PE15 first
    GPIOE->MODER |= 0x55550000;  // Set PE8-PE15 to general purpose output mode

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
    if (led_num < 8) { // Ensure the LED number is within 0-7 range
        GPIOE->BSRR = (1 << (led_num + 8)); // Set bit to turn on LED
    }
}

void clear_led(uint8_t led_num) {
    if (led_num < 8) { // Ensure the LED number is within 0-7 range
        GPIOE->BRR = (1 << (led_num + 8)); // Clear bit to turn off LED
    }
}

void toggle_led(uint8_t led_num) {
    if (led_num < 8) { // Ensure the LED number is within 0-7 range
        GPIOE->ODR ^= (1 << (led_num + 8)); // Toggle corresponding LED PE8 to PE15
    }
}

void set_button_handler(void (*handler)()) {
    button_handler = handler;
}
