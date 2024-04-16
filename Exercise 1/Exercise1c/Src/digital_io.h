#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

#include <stdint.h>

typedef void (*ButtonCallback)(void);

void digital_io_init(ButtonCallback callback);
void set_led(uint8_t led_num);
void clear_led(uint8_t led_num);
void toggle_led(uint8_t led_num);
void set_button_handler(ButtonCallback handler);
void chase_led();


// Get and set functions for LED states
void set_led_state(uint8_t led_num, uint8_t state);
uint8_t get_led_state(uint8_t led_num);

#endif
