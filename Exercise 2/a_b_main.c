#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Including the relevant header files
#include "serial.h"
#include "stm32f303xc.h"

// Callback function to simulate a delay after transmission
void finished_transmission(uint32_t bytes_sent) {
	for (volatile uint32_t i = 0; i < 0x8ffff; i++) {
	}
}

// Callback function that echoes the received string back to the sender
void receive_callback(uint8_t* buffer, uint32_t length) {
	SerialOutputString(buffer, &USART1_PORT);
}

// Calling the serial test 
void run_serial_test(void);


int main(void) {
	
// Initialising the serial module 
    SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission, &receive_callback);
	
// Continue to run the serial test indefinitely 
    for (;;) {
    	run_serial_test();
    }
}
