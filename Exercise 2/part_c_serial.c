#include <stdlib.h>

#include "first_serial.h"
#include "stm32f303xc.h"

struct _SerialPort {
    USART_TypeDef *UART;
    GPIO_TypeDef *GPIO;
    volatile uint32_t MaskAPB2ENR;
    volatile uint32_t MaskAPB1ENR;
    volatile uint32_t MaskAHBENR;
    volatile uint32_t SerialPinModeValue;
    volatile uint32_t SerialPinSpeedValue;
    volatile uint32_t SerialPinAlternatePinValueLow;
    volatile uint32_t SerialPinAlternatePinValueHigh;
    void (*completion_function)(uint32_t);
    void (*receive_callback)(uint8_t *, uint32_t);
};

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

void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t), void (*receive_callback)(uint8_t *)) {
    serial_port->completion_function = completion_function;
    serial_port->receive_callback = receive_callback;

    // Enable clock for GPIO and USART
    RCC->AHBENR |= serial_port->MaskAHBENR;
    RCC->APB2ENR |= serial_port->MaskAPB2ENR;

    // Configure GPIO pins for USART
    serial_port->GPIO->MODER |= serial_port->SerialPinModeValue;
    serial_port->GPIO->OSPEEDR |= serial_port->SerialPinSpeedValue;
    serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
    serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;

    // Configure USART
    // For 115200 baud rate at 8MHz clock, the baud rate divider is 69.4444 (approximately).
    // Use the closest integer value for the baud rate register (BRR), which is 69.
    serial_port->UART->BRR = 69;

    serial_port->UART->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE; // Enable receiver, transmitter, and USART itself

    // Enable USART interrupt in NVIC
    NVIC_SetPriority(USART1_IRQn, 1); // Set priority as per application needs
    NVIC_EnableIRQ(USART1_IRQn);

    // Enable RXNE interrupt
    serial_port->UART->CR1 |= USART_CR1_RXNEIE;
}

void SerialOutputChar(uint8_t data, SerialPort *serial_port) {
    while (!(serial_port->UART->ISR & USART_ISR_TXE)) {}
    serial_port->UART->TDR = data;
}

void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {
    while (*pt) {
        SerialOutputChar(*pt++, serial_port);
    }
}

void USART1_IRQHandler(void) {
    static uint8_t buffer[64];  // Increase size as needed
    static uint32_t pos = 0;

    if (USART1->ISR & USART_ISR_RXNE) {
        uint8_t data = USART1->RDR;

        buffer[pos++] = data;

        if (data == '\r' || data == '\n' || pos >= sizeof(buffer) - 1) {
            buffer[pos] = '\0';  // Null-terminate the string
            if (USART1_PORT.receive_callback != NULL) {
                USART1_PORT.receive_callback(buffer, pos);
            }
            pos = 0;  // Reset buffer position
        }

        USART1->ISR &= ~USART_ISR_RXNE;  // Clear the RXNE flag
    }
}
