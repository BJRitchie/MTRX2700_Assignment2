#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"
#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void execute_command(const char* command, const char* params);

void received_new_string(uint8_t *buffer, uint32_t length) {
    char command[64] = {0};
    char params[64] = {0};

    // Assuming buffer is null-terminated; otherwise, ensure it before calling sscanf
    sscanf((const char *)buffer, "%63s %63[^\n]", command, params);

    execute_command(command, params);
}

void execute_command(const char* command, const char* params) {
    if (strcmp(command, "led") == 0) {
        // Execute LED command
    } else if (strcmp(command, "serial") == 0) {
        // Echo back the string for the serial command
        SerialOutputString((uint8_t *)params, &USART1_PORT);
        SerialOutputString((uint8_t *)"\r\n", &USART1_PORT); // For new line
    } else if (strcmp(command, "oneshot") == 0) {
        // Execute one-shot timer command
    } else if (strcmp(command, "timer") == 0) {
        // Execute continuous timer command
    } else {
        // Unknown command
        SerialOutputString((uint8_t *)"Unknown command\r\n", &USART1_PORT);
    }
}


void finished_transmission(uint32_t bytes_sent) {
	for (volatile uint32_t i = 0; i < 0x8; i++) {
	}
}


int main(void) {
    SerialInitialise(BAUD_115200, &USART1_PORT, finished_transmission, received_new_string);
    for (;;) {
    }
}
