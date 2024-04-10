#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

#include <stdint.h>

void digital_io_init();
void set_led(uint8_t led_num);
void clear_led(uint8_t led_num);
void toggle_led(uint8_t led_num);
void set_button_handler(void (*handler)());

#endif /* DIGITAL_IO_H */
