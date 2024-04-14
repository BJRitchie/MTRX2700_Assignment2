#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Include the relevant header files
#include "serial.h"
#include "stm32f303xc.h"

// Callback function to simulate a delay after transmission
void finished_transmission(uint32_t bytes_sent) {
	for (volatile uint32_t i = 0; i < 0x8; i++) {
	}
}

// Callback function that echoes the received string back to the sender
void received_new_string(uint8_t *buffer, uint32_t bytes_sent) {
	SerialOutputString(buffer, &USART1_PORT);
}

int main(void) {
	
// Initialising the serial module
    SerialInitialise(BAUD_115200, &USART1_PORT, finished_transmission, received_new_string);
	
// Loop indefinitely 
    for (;;) {
    }
}
