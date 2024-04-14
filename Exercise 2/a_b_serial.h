#ifndef SERIAL_PORT_HEADER
#define SERIAL_PORT_HEADER

#include <stdint.h>
#include <stdlib.h>

typedef void (*SerialReceiveCallback)(uint8_t*, uint32_t);
struct _SerialPort;

typedef struct _SerialPort SerialPort;

extern SerialPort USART1_PORT;

enum {
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200
};

void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t), SerialReceiveCallback receive_callback );

void run_serial_test(void);

void SerialOutputChar(uint8_t, SerialPort *serial_port);

void SerialEnableInterrupt(SerialPort *serial_port);

void SerialOutputString(uint8_t *pt, SerialPort *serial_port);

uint32_t SerialInputString(uint8_t *buffer, uint32_t buffer_size, SerialPort *serial_port);

#endif
