#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "serial.h"
#include "stm32f303xc.h"
#include "timer.h"
#include "initialise.h"
#include "digital_io.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void execute_command(const char* command, const char* params);

void received_new_string(uint8_t *buffer, uint32_t length) {
    char command[64] = {0};
    char params[64] = {0};

    sscanf((const char *)buffer, "%63s %63[^\n]", command, params);

    execute_command(command, params);
}

void oneshot_callback(void) {
    SerialOutputString((uint8_t *)"One-shot timer expired.\r\n", &USART1_PORT);
}


void timer_callback(void) {
    SerialOutputString((uint8_t *)"Timer tick.\r\n", &USART1_PORT);
}


void execute_command(const char* command, const char* params) {

    if (strcmp(command, "led") == 0) {
        if (params[0] == '1' || '0') {
            for (int i = 0; i < 8; i++) {
                if (params[i] == '1') {
                    set_led(i); 
                } else {
                	clear_led(i);
                }
            }
            SerialOutputString((uint8_t *)"LED pattern processed.\r\n", &USART1_PORT);
        } else {
            SerialOutputString((uint8_t *)"Invalid LED pattern. Please enter exactly 8 digits.\r\n", &USART1_PORT);
        }
    } else if (strcmp(command, "serial") == 0) {
        SerialOutputString((uint8_t *)params, &USART1_PORT);
        SerialOutputString((uint8_t *)"\r\n", &USART1_PORT);
    } else if (strcmp(command, "oneshot") == 0) {
        uint32_t delay = strtoul(params, NULL, 10);
        set_one_shot_mode(&TIM4_init, delay, oneshot_callback);
    } else if (strcmp(command, "timer") == 0) {
        uint32_t period = strtoul(params, NULL, 10);
        set_continuous_mode(&TIM2_init, period, timer_callback); 
    } else {
        SerialOutputString((uint8_t *)"Unknown command\r\n", &USART1_PORT);
    }
}

void finished_transmission(uint32_t bytes_sent) {
	for (volatile uint32_t i = 0; i < 0x8; i++) {
	}
}

int main(void) {
	digital_io_init();
	enable_timer_interrupt();
    SerialInitialise(BAUD_115200, &USART1_PORT, finished_transmission, received_new_string);

    for (;;) {
    }
}
