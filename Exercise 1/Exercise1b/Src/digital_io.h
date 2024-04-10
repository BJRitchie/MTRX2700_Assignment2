#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

#include <stdint.h>

// Function pointer type for button press callback
typedef void (*ButtonPressCallback)();

void digital_io_init(ButtonPressCallback callback);
void set_button_handler(ButtonPressCallback handler);
void set_led(uint8_t led_num);
void clear_led(uint8_t led_num);
void toggle_led(uint8_t led_num);

#endif
