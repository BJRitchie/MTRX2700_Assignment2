#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Include relevant header files
#include "d_serial.h"
#include "stm32f303xc.h"

// Callback function to simulate a delay after transmission
void finished_transmission(uint32_t bytes_sent) {
	
// This loop simulates a short delay
	for (volatile uint32_t i = 0; i < 0x8; i++) {
	}
}

// Callback function that is triggered when a new string is received
void received_new_string(uint8_t *buffer, uint32_t bytes_sent) {
	
// Add a carriage return and newline at the end of the string to format the output
    	buffer[bytes_sent] = '\r';  // Carriage return to return the cursor to the beginning of the line
    	buffer[bytes_sent + 1] = '\n';  // New line to advance to the next line
    	buffer[bytes_sent + 2] = '\0';  // Null terminator to end the string
	
// Output the received string back to the serial port, useful for echo tests or command line interfaces
	SerialOutputString(buffer, &USART1_PORT);
}

int main(void) {

// Initialising the serial module
    SerialInitialise(BAUD_115200, &USART1_PORT, finished_transmission, received_new_string);
	
// Loop indefinitely
    for (;;) {
    }
}
