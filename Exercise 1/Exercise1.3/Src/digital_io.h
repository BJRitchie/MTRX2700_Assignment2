#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

#include <stdint.h>

void digital_io_init();
void set_led(uint8_t led_num);
void clear_led(uint8_t led_num);
void toggle_led(uint8_t led_num);
void set_button_handler(void (*handler)());

// New functions to access LED state
uint8_t get_led_state(uint8_t led_num);
void set_led_state(uint8_t led_num, uint8_t state);

#endif /* DIGITAL_IO_H */
