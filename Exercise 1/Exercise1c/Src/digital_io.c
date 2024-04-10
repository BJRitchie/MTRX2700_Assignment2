#include "digital_io.h"
#include "stm32f303xc.h"
#include <stdbool.h>

#define NUM_LEDS 8
#define LED_DELAY 1000

static ButtonPressCallback button_press_callback = 0x00;
static bool led_chase_running = false;

void EXTI0_IRQHandler(void) {
    if (button_press_callback != 0x00) {
        button_press_callback(); // Call the callback function
    }
    EXTI->PR |= EXTI_PR_PR0; // Reset interrupt
}

void digital_io_init(ButtonPressCallback callback) {
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

    // Save the callback function pointer
    button_press_callback = callback;
}

void toggle_led(uint8_t led_num) {
    GPIOE->ODR ^= (1 << (led_num + 8)); // Toggle corresponding LED
}

void led_chase() {
    static uint8_t current_led = 0;
    if (!led_chase_running) {
        led_chase_running = true;
        while (1) {
            toggle_led(current_led); // Toggle LED
            current_led = (current_led + 1) % NUM_LEDS; // Move to next LED
            for (volatile uint32_t i = 0; i < LED_DELAY * 1000; i++); // Delay
        }
    }
}

void set_button_handler(ButtonPressCallback handler) {
    button_press_callback = handler;
}
