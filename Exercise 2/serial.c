#include "serial.h"
#include "stm32f303xc.h"

static uint8_t received_string[64]; // Buffer for received string
static uint32_t received_length = 0; // Length of the received string

// Serial port structure definition, including GPIO and USART settings
struct _SerialPort {
	USART_TypeDef *UART;
	GPIO_TypeDef *GPIO;
	volatile uint32_t MaskAPB2ENR;	// mask to enable RCC APB2 bus registers
	volatile uint32_t MaskAPB1ENR;	// mask to enable RCC APB1 bus registers
	volatile uint32_t MaskAHBENR;	// mask to enable RCC AHB bus registers
	volatile uint32_t SerialPinModeValue;
	volatile uint32_t SerialPinSpeedValue;
	volatile uint32_t SerialPinAlternatePinValueLow;
	volatile uint32_t SerialPinAlternatePinValueHigh;
	void (*completion_function)(uint32_t);
	SerialReceiveCallback receive_callback;
};

// Configuration for USART1
SerialPort USART1_PORT = {USART1,
		GPIOC,
		RCC_APB2ENR_USART1EN, // bit to enable for APB2 bus
		0x00,	// bit to enable for APB1 bus
		RCC_AHBENR_GPIOCEN, // bit to enable for AHB bus
		0xA00,
		0xF00,
		0x770000,  // for USART1 PC10 and 11, this is in the AFR low register
		0x00, // no change to the high alternate function register
		0x00 // default function pointer is NULL
		};

// Function to read input string from serial until a terminating character is read
uint32_t SerialInputString(uint8_t *buffer, uint32_t buffer_size, SerialPort *serial_port) {
    static uint8_t last_character = 0;  // Static variable to store the last character received
    uint32_t count = 0;

    while (count < buffer_size - 1) {
        // Wait for the data to be received
        if ((serial_port->UART->ISR & USART_ISR_RXNE) != 0) {
            uint8_t character = serial_port->UART->RDR;

            // Check if the character is different from the last character received
            if (character != last_character) {
                last_character = character;  // Update the last character received

                // Check for the carriage return character
                if (character == '\r') {
                    break;
                }
                buffer[count++] = character;
            }
        }
    }
    buffer[count] = '\0'; // Null-terminate the string

    return count;
}

// Test function to repeatedly receive and echo strings
void run_serial_test(void) {
    //uint8_t *string_to_send = "STM32 ready to receive:\r\n";
    uint8_t received_string[64]; // Buffer for received string
    //SerialOutputString(string_to_send, &USART1_PORT);
    uint32_t received_length = SerialInputString(received_string, sizeof(received_string), &USART1_PORT);
    if (received_length >= 0) {
        // Echo the received string back
        SerialOutputString(received_string, &USART1_PORT);
    }
}

// Initialise the serial port with specified baud rate and callback functions
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t), SerialReceiveCallback receive_callback) {

	serial_port->completion_function = completion_function;
	// enable clock power, system configuration clock and GPIOC
	// common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// enable the GPIO which is on the AHB bus
	RCC->AHBENR |= serial_port->MaskAHBENR;

	// set pin mode to alternate function for the specific GPIO pins
	serial_port->GPIO->MODER = serial_port->SerialPinModeValue;

	// enable high speed clock for specific GPIO pins
	serial_port->GPIO->OSPEEDR = serial_port->SerialPinSpeedValue;

	// set alternate function to enable USART to external pins
	serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
	serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;

	// enable the device based on the bits defined in the serial port definition
	RCC->APB1ENR |= serial_port->MaskAPB1ENR;
	RCC->APB2ENR |= serial_port->MaskAPB2ENR;

	// Get a pointer to the 16 bits of the BRR register that we want to change
	uint16_t *baud_rate_config = (uint16_t*)&serial_port->UART->BRR; // only 16 bits used

	// Baud rate calculation from datasheet
	switch(baudRate){
	case BAUD_9600:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_19200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_38400:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_57600:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
	//SerialEnableInterrupt(serial_port);
	serial_port->receive_callback = receive_callback;
}

// Enables the receive interrupt for the USART
void SerialEnableInterrupt(SerialPort *serial_port) {
    // Enable the USART receive interrupt
    serial_port->UART->CR1 |= USART_CR1_RXNEIE;

    // Enable the USART interrupt in the NVIC
    NVIC_SetPriority(USART1_IRQn, 1); // Set priority level to 1
    NVIC_EnableIRQ(USART1_IRQn); // Enable IRQ for USART1
}

// Function to send a single character over serial
void SerialOutputChar(uint8_t data, SerialPort *serial_port) {
	// Wait for the transmit data register to be empty
	while((serial_port->UART->ISR & USART_ISR_TXE) == 0){
	}

	// Load the data into the transmit data register
	serial_port->UART->TDR = data;
}

// Function to send a string of characters over serial
void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {
	uint32_t counter = 0; // Counter for the number of bytes sent
	while(*pt) { // Loop until end of string
		SerialOutputChar(*pt, serial_port); // Send current character and increase counter
		counter++;
		pt++;
	}
	
	// Call the completion function after the whole string has been sent
	serial_port->completion_function(counter);
}

// Interrupt handler for USART1
void USART1_IRQHandler(void) {
    static uint8_t received_buffer[64]; // Buffer to store received data
    static uint32_t received_length = 0; // Length of received data

    // Check if the receive data register is not empty
    if (USART1->ISR & USART_ISR_RXNE) {
        uint8_t received_byte = USART1->RDR; // Read the received byte

	// Check if the received byte is the carriage return character
        if (received_byte == '\r') { 
	    // Call the receive callback function if it is not NULL
            if (USART1_PORT.receive_callback != NULL) {
                USART1_PORT.receive_callback(received_buffer, received_length); // Trigger the callback
            }
            received_length = 0; // Reset the buffer for the next message
        } else {
	    // Check if there is space in the buffer
            if (received_length < sizeof(received_buffer) - 1) {
                received_buffer[received_length++] = received_byte; // Store the byte in the buffer
                received_buffer[received_length] = '\0'; // Null-terminate the string
            }
        }
    }
}
