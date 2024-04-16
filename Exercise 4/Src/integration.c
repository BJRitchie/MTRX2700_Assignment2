#include <integration.h>
#include "stm32f303xc.h"

// Function for receiving string sent
void received_new_string(uint8_t *buffer, uint32_t length) {
    char command[64] = {0}; // Holds the command 
    char params[64] = {0}; // Holds the parameter for the command

    // Assuming buffer is null-terminated; otherwise, ensure it before calling sscanf
    sscanf((const char *)buffer, "%63s %63[^\n]", command, params);
    execute_command(command, params);
}

// Carry out a function based on the command inputted 
void execute_command(const char* command, const char* params) {
    if (strcmp(command, "led") == 0) { // Command for led
        // Ensure it is a valid command by testing for 1 or 0
        if (params[0] == '1' || '0') {
            // Iterate over each character in the params
            for (int i = 0; i < 8; i++) {
                // Check each character and if '1', toggle corresponding LED
                if (params[i] == '1') {
                    set_led(i);  // Toggle the LED at this position
                } else {
                	clear_led(i); // Clear LED if '0'
                }
            }
            SerialOutputString((uint8_t *)"LED pattern processed.\r\n", &USART1_PORT);
        } else {
            SerialOutputString((uint8_t *)"Invalid LED pattern. Please enter exactly 8 digits.\r\n", &USART1_PORT);
        }
    } else if (strcmp(command, "serial") == 0) { // Command for serial
        SerialOutputString((uint8_t *)params, &USART1_PORT); // Output serial command
        SerialOutputString((uint8_t *)"\r\n", &USART1_PORT);
    } else if (strcmp(command, "oneshot") == 0) { // Command for oneshot
        uint32_t delay = strtoul(params, NULL, 10); // Reads the time requested for the oneshot
        set_one_shot_mode(&TIM4_init, delay, oneshot_callback); // Output oneshot command
    } else if (strcmp(command, "timer") == 0) { // Command for timr
        uint32_t period = strtoul(params, NULL, 10); // Reads the time requested for the continuous timer
        set_continuous_mode(&TIM2_init, period, timer_callback);  // Outputs the timer interval
    } else {
        SerialOutputString((uint8_t *)"Unknown command\r\n", &USART1_PORT); // If invalid command
    }
}
