#include <stdlib.h>

#include "d_serial.h"
#include "stm32f303xc.h"

#define BUFFER_SIZE 64 // Define the size of each buffer

// Define the structure for managing a serial port
struct _SerialPort {
    USART_TypeDef *UART; // Pointer to USART hardware registers
    GPIO_TypeDef *GPIO; // Pointer to GPIO hardware registers
    volatile uint32_t MaskAPB2ENR; // Bit mask to enable USART in APB2 peripheral clock enable register
    volatile uint32_t MaskAHBENR; // Bit mask to enable GPIO in AHB peripheral clock enable register
    volatile uint32_t SerialPinModeValue; // GPIO mode settings for USART pins
    volatile uint32_t SerialPinSpeedValue; // GPIO speed settings for USART pins
    volatile uint32_t SerialPinAlternatePinValueLow; // Lower half of alternate function settings
    volatile uint32_t SerialPinAlternatePinValueHigh; // Upper half of alternate function settings
    void (*completion_function)(uint32_t); // Callback function for transmission complete
    void (*receive_callback)(uint8_t *, uint32_t); // Callback function for data reception
    uint8_t buffer1[BUFFER_SIZE]; // First buffer for data reception
    uint8_t buffer2[BUFFER_SIZE]; // Second buffer for data reception
    uint8_t *activeBuffer; // Pointer to the currently active buffer
    uint8_t *processingBuffer; // Pointer to the buffer available for processing
    uint32_t pos; // Position in the active buffer
};

// Configuration for USART1, defining its connection with hardware and functionality
SerialPort USART1_PORT = {
    .UART = USART1,
    .GPIO = GPIOC,
    .MaskAPB2ENR = RCC_APB2ENR_USART1EN,
    .MaskAHBENR = RCC_AHBENR_GPIOCEN,
    .SerialPinModeValue = 0xA00,
    .SerialPinSpeedValue = 0xF00,
    .SerialPinAlternatePinValueLow = 0x770000,
    .activeBuffer = USART1_PORT.buffer1,
    .processingBuffer = USART1_PORT.buffer2,
    .pos = 0,
    .completion_function = NULL,
    .receive_callback = NULL
};

// Initialise the USART hardware with specific settings
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t), void (*receive_callback)(uint8_t *, uint32_t)) {
    serial_port->completion_function = completion_function; // Set the transmission completion callback
    serial_port->receive_callback = receive_callback; // Set the data reception callback


// Enable clock for GPIO and USART peripherals
    RCC->AHBENR |= serial_port->MaskAHBENR; // Enable AHB bus clock for GPIO
    RCC->APB2ENR |= serial_port->MaskAPB2ENR; // Enable APB2 bus clock for USART

// Set GPIO mode and speed for USART pins
    serial_port->GPIO->MODER |= serial_port->SerialPinModeValue;
    serial_port->GPIO->OSPEEDR |= serial_port->SerialPinSpeedValue;
    serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
    serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;

// Configure USART
// For 115200 baud rate at 8MHz clock, the baud rate divider is 69.4444 (approximately).
// Use the closest integer value for the baud rate register (BRR), which is 69.
    serial_port->UART->BRR = 69;

    serial_port->UART->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE; // Enable receiver, transmitter, and USART

// Enable USART interrupt in NVIC
    NVIC_SetPriority(USART1_IRQn, 1); // Set the interrupt priority
    NVIC_EnableIRQ(USART1_IRQn); // Enable the interrupt

    serial_port->UART->CR1 |= USART_CR1_RXNEIE; // Enable RXNE interrupt for receiving data
}

// Function to send a single character via USART, enabling TXE interrupt after sending
void SerialOutputChar(uint8_t data, SerialPort *serial_port) {
    while (!(serial_port->UART->ISR & USART_ISR_TXE)) {} // Wait for transmit data register to be empty
    serial_port->UART->TDR = data; // Send data
    serial_port->UART->CR1 |= USART_CR1_TXEIE; // Enable transmit empty interrupt
}

// Function to send a string via USART
void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {
    while (*pt) {
        SerialOutputChar(*pt++, serial_port); // Send each character of the string
    }
}

// Interrupt handler for USART1 to handle data reception and buffer swapping
void USART1_IRQHandler(void) {
    if (USART1->ISR & USART_ISR_RXNE) { // Check if data is received
        uint8_t data = USART1->RDR; // Read received data

        USART1_PORT.activeBuffer[USART1_PORT.pos++] = data; // Store data in active buffer
// Check for terminating characters or buffer overflow
        if (data == '\r' || data == '\n' || USART1_PORT.pos >= BUFFER_SIZE - 1) {
            USART1_PORT.activeBuffer[USART1_PORT.pos] = '\0';  // Null-terminate the string
            if (USART1_PORT.receive_callback != NULL) {
                USART1_PORT.receive_callback(USART1_PORT.activeBuffer, USART1_PORT.pos); // Calls receive callback
            }

// Swap buffers to allow continuous reception while processing current data
            uint8_t *temp = USART1_PORT.activeBuffer;
            USART1_PORT.activeBuffer = USART1_PORT.processingBuffer;
            USART1_PORT.processingBuffer = temp;
            USART1_PORT.pos = 0;
        }

        USART1->ISR &= ~USART_ISR_RXNE;  // Clear the RXNE flag to prepare for the next data reception
    }
}
