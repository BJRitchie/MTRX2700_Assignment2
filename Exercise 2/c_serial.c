#include <stdlib.h>

#include "c_serial.h"
#include "stm32f303xc.h"

// Define the structure for managing a serial port
struct _SerialPort {
    USART_TypeDef *UART;                     // Pointer to USART hardware registers
    GPIO_TypeDef *GPIO;                      // Pointer to GPIO hardware registers
    volatile uint32_t MaskAPB2ENR;           // Bit mask to enable USART in APB2 peripheral clock enable register
    volatile uint32_t MaskAPB1ENR;           // Bit mask for APB1 peripheral clock enable (not used here)
    volatile uint32_t MaskAHBENR;            // Bit mask to enable GPIO in AHB peripheral clock enable register
    volatile uint32_t SerialPinModeValue;    // GPIO mode settings for USART pins
    volatile uint32_t SerialPinSpeedValue;   // GPIO speed settings for USART pins
    volatile uint32_t SerialPinAlternatePinValueLow; // Lower half of alternate function settings
    volatile uint32_t SerialPinAlternatePinValueHigh;// Upper half of alternate function settings
    void (*completion_function)(uint32_t);   // Callback function for transmission complete
    void (*receive_callback)(uint8_t *, uint32_t); // Callback function for data reception
};

// Configuration for USART1, defining its connection with hardware and functionality
SerialPort USART1_PORT = {
    USART1,
    GPIOC,
    RCC_APB2ENR_USART1EN,
    0,
    RCC_AHBENR_GPIOCEN,
    0xA00,
    0xF00,
    0x770000,
    0,
    NULL,
    NULL
};

// Initialise the USART hardware with specific settings
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t), void (*receive_callback)(uint8_t *)) {
    serial_port->completion_function = completion_function; // Set the completion callback
    serial_port->receive_callback = receive_callback; // Set the receive callback

    // Enable the clock for GPIO and USART peripherals
    RCC->AHBENR |= serial_port->MaskAHBENR;  // Enable AHB bus clock for GPIO
    RCC->APB2ENR |= serial_port->MaskAPB2ENR;// Enable APB2 bus clock for USART

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

    // Configure and enable USART interrupt
    NVIC_SetPriority(USART1_IRQn, 1); // Set the interrupt priority
    NVIC_EnableIRQ(USART1_IRQn); // Enable the interrupt
    serial_port->UART->CR1 |= USART_CR1_RXNEIE; // Enable RXNE interrupt
}

// Function to send a single character via USART
void SerialOutputChar(uint8_t data, SerialPort *serial_port) {
    while (!(serial_port->UART->ISR & USART_ISR_TXE)) {} // Wait for transmit data register to be empty
    serial_port->UART->TDR = data; // Send data
}

// Function to send a string via USART
void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {
    while (*pt) {
        SerialOutputChar(*pt++, serial_port); // Send each character of the string
    }
}

// Interrupt handler for USART1
void USART1_IRQHandler(void) {
    static uint8_t buffer[64];  // Buffer to store received data
    static uint32_t pos = 0; // Position in buffer

    if (USART1->ISR & USART_ISR_RXNE) { // Check if data is received
        uint8_t data = USART1->RDR; // Read received data

        buffer[pos++] = data; // Store data in buffer

        // Check for terminating characters or buffer overflow
        if (data == '\r' || data == '\n' || pos >= sizeof(buffer) - 1) {
            buffer[pos] = '\0';  // Null-terminate the string
            if (USART1_PORT.receive_callback != NULL) {
                USART1_PORT.receive_callback(buffer, pos); // Calls receive callback
            }
            pos = 0;  // Reset buffer position // Reset buffer position
        }

        USART1->ISR &= ~USART_ISR_RXNE;  // Clear the RXNE flag
    }
}
