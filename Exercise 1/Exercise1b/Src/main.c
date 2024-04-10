#include "digital_io.h"

void button_press_handler() {
    toggle_led(0); // Toggle LED connected to pin PE8
}

int main(void) {
    digital_io_init(&button_press_handler); // Pass function pointer to button press handler
    while (1) {

    }
}
