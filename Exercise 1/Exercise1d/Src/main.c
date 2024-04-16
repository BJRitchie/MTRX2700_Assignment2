#include "stm32f303xc.h"
#include "digital_io.h"

int main(void) {
    digital_io_init(&chase_led);          // Initialize digital I/O module
    set_button_handler(&chase_led);  // Set button handler function
    uint32_t delay = 5000;

    // Initialize timer for controlling LED speed
    timer_init(delay);

    for(;;) {}
}
