#include "digital_io.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_LEDS 8

bool led_chase_enabled = false;

void button_press_handler() {
    led_chase_enabled = !led_chase_enabled;  // Toggle the chase sequence on/off
    if (!led_chase_enabled) {
        // Turn off all LEDs if chase is disabled
        for (int i = 0; i < NUM_LEDS; i++) {
            turn_off_led(i);
        }
    }
}

int main(void) {
    digital_io_init(&button_press_handler); // Pass function pointer to button press handler
    while (1) {
        if (led_chase_enabled) {
            led_chase();  // Run the LED chase sequence
        }
    }
}
