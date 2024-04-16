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

void received_new_string(uint8_t *buffer, uint32_t length);

void execute_command(const char* command, const char* params);

#endif
