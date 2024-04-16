#ifndef INTEGRATION_HEADER
#define INTEGRATION_HEADER

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <led.h>
#include <serial.h>
#include <timer.h>

// Function for receiving string sent consisting of a command and parameters
void received_new_string(uint8_t *buffer, uint32_t length);

// Carry out a function based on the command inputted
void execute_command(const char* command, const char* params);

#endif
