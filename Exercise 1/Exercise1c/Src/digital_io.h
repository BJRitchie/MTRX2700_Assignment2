#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*ButtonPressCallback)();

void digital_io_init(ButtonPressCallback callback);
void toggle_led(uint8_t led_num);
void set_button_handler(ButtonPressCallback handler);
void led_chase();

#endif
