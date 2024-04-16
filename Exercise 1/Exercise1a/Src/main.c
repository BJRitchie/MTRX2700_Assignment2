#include "stm32f303xc.h"
#include "digital_io.h"

int main(void) {
    digital_io_init();          // Initialize digital I/O module
    set_button_handler(&chase_led);  // Set button handler function

    for(;;) {}
}
