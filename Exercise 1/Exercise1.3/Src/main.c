#include "stm32f303xc.h"
#include "digital_io.h"

void chase_led() {
    static uint8_t led_index = 0;
    clear_led(led_index);       // Clear the currently lit LED
    led_index = (led_index + 1) % 8;  // Move to the next LED
    set_led(led_index);         // Set the next LED
}

int main(void) {
    digital_io_init();          // Initialize digital I/O module
    set_button_handler(&chase_led);  // Set button handler function

    while (1) {
    }
}
