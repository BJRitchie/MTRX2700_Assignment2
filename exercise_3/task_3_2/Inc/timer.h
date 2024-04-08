
#ifndef TIMER_HEADER
#define TIMER_HEADER

#include <stdint.h>
#include "stm32f303xc.h"

// Defining the hardware timer struct
struct _HardwareTimer;
typedef struct _HardwareTimer HardwareTimer;

// make instance for TIM2
extern HardwareTimer TIM2_init;

// initialise timer
// inputs:
// - hardware_timer: pointer to a struct containing
//	the initialisation variables for the desired timer
// - completion_function: a pointer to the function to be
// 	completed when an interrupt is encountered.
void TimerInitialise(HardwareTimer *hardware_timer, void (*completion_function)());

// enable the timer interrupts
void enable_timer_interrupt();

// get and set functions for timer

// gets the ARRValue
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// ouput:
// - prescaler value of timer
uint16_t getPrescaleValue(struct _HardwareTimer *hardware_timer);

// set the auto-reload register value
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - value: the desired prescaler to be set
void setPrescaleValue(struct _HardwareTimer *hardware_timer, uint16_t value);

// gets the ARRValue
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// ouput:
// - ARRValue
uint32_t getARRValue(struct _HardwareTimer *hardware_timer);

// set the auto-reload register value
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - value: the desired ARRValue to be set
void setARRValue(struct _HardwareTimer *hardware_timer, uint32_t value);

// Set a period between clock overflows and therefore interrupts .
// Will also reset clock.
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - period: the delay period in microseconds
void setPeriod(struct _HardwareTimer *hardware_timer, uint32_t period);

// function to be run when timer overflows
// inputs;
// - hardware_timer: pointer to the hardware timer in question
// - on_timer_overflow: a pointer to the function to be run when timer overflows
void timer_completion_func(struct _HardwareTimer *hardware_timer, void (*on_timer_overflow)(void));

#endif
