#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Include relevant header files
#include "serial.h"
#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void finished_transmission(uint32_t bytes_sent) {
	for (volatile uint32_t i = 0; i < 0x8; i++) {
	}
}

void received_new_string(uint8_t *buffer, uint32_t bytes_sent) {
    buffer[bytes_sent] = '\r';  // Carriage return
    buffer[bytes_sent + 1] = '\n';  // Newline
    buffer[bytes_sent + 2] = '\0';  // Null-terminator
	SerialOutputString(buffer, &USART1_PORT);
}

int main(void) {
    SerialInitialise(BAUD_115200, &USART1_PORT, finished_transmission, received_new_string);
    for (;;) {
    }
}
