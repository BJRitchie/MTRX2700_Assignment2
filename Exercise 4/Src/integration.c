#include <integration.h>
#include "stm32f303xc.h"


void received_new_string(uint8_t *buffer, uint32_t length) {
    char command[64] = {0};
    char params[64] = {0};

    // Assuming buffer is null-terminated; otherwise, ensure it before calling sscanf
    sscanf((const char *)buffer, "%63s %63[^\n]", command, params);

    execute_command(command, params);
}

void execute_command(const char* command, const char* params) {
    if (strcmp(command, "led") == 0) {
        // Ensure we have exactly 8 characters for LED control
        if (params[0] == '1' || '0') {
            // Iterate over each character in the params
            for (int i = 0; i < 8; i++) {
                // Check each character and if '1', toggle corresponding LED
                if (params[i] == '1') {
                    set_led(i);  // Toggle the LED at this position
                } else {
                	clear_led(i);
                }
                // No action is taken if the character is '0'
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
        set_continuous_mode(&TIM2_init, period, timer_callback);  // Ensure this function is implemented
    } else {
        SerialOutputString((uint8_t *)"Unknown command\r\n", &USART1_PORT);
    }
}
