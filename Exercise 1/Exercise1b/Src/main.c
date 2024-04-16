#include "stm32f303xc.h"
#include "digital_io.h"

uint8_t led_index = 0; // Declare led_index as a global variable

int main(void) {
    digital_io_init(&chase_led);          // Initialize digital I/O module
    set_button_handler(&chase_led);  // Set button handler function

    for(;;) {}
}
