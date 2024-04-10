
#include "digital_io.h"
#include <stdbool.h>

#define NUM_LEDS 8

static bool led_chase_enabled = false;

void button_press_handler() {
    if (!led_chase_enabled) {
        led_chase();
        led_chase_enabled = true;
    } else {
        // Turn off all LEDs
        for (int i = 0; i < NUM_LEDS; i++) {
            toggle_led(i);
        }
        led_chase_enabled = false;
    }
}

int main(void) {
    digital_io_init(&button_press_handler); // Pass function pointer to button press handler
    while (1) {
        // Main application loop
        // This loop may contain other tasks or remain empty
    }
}
