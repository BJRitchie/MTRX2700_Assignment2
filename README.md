# MTRX2700 Assignment 2 README File Wednesday PM
## Project Details:
The C Lab assignment is comprised of 3 primary C programming exercises for the STM32F303Discovery Microcontroller with 1 modular integration exercise.

## Group Members
- Sebastian O'Connell (520443826)
- Ben Ritchie (SID)
- Ranjeev Perera (SID)

## Roles & Responsibilities
### Sebastian O'Connell:
- Exercise 2 and documentation
- Exercise 4 integration and documentation
- Minutes

### Ben Ritchie:
- Exercise 3 and documentation
- Exercise 4 integration and documentation

### Ranjeev Perera:
- Exercise 1 and documentation

## Exercise Breakdowns
### Exercise 1:


### Exercise 2:
#### Part A - Basic Functionality:
- Develops module that reads incoming characters into a memory buffer over UART
#### Part B - Callback Function:
- Establishes a callback function during the initialisation of the module
- Re-transmits the string back over the UART
##### Testing Plan:
- Enter a string into the PuTTY terminal once the code is running
- The string should be re-transmitted back to the terminal
#### Part C - Interrupt-Based Receiving:
- Adapts the serial receiving process to an interrupt-based approach
- Interrupt used to handle data reception dynamically
#### Part D - Advanced Functionality:
- Re-transmits the string using interrupts
- Double buffer system:
  - Double buffer used to allow the serial interface to continue receiving new characters into one buffer while the previously received string is being procesed from another buffer
  - Swaps between two buffers upon completing the reception of one string, alloiwing continuous data handling without losing data
  - Buffer switching managing in the interrupt handler to ensure smooth flow of data and efficient processing
##### Testing Plan:


## Exercise 3.1: Continuous Timer 
### Summary
This task involved the configuration and operation of a continous timer using hardware clocks. It required the use of interrupts to enable modularity in the code, so other tasks can be done while the timer ticks in the background. 

### Usage
TimerInitialise can be used to initialise the desired timer, setting it to operate in a continous manner by default. Input a pointer to the struct containing the relevant initialisation information, a delay period in milliseconds, and a pointer to the completion function. When run, the timer will start and generate an intrrupt after the entered delay period. 

### Valid input
A struct for timer 2 (TIM2_init) is prepared and can be used, and a delay/period input of up to (2^32-1 = 4.3*10^9)ms can be input. The completion function must return nothing, and have no inputs. 

### Functions and modularity
The project is broken into two modules - one for timers, and another for the LEDs. 

#### Timer Module 
- TimerInitialise: function to initialise and start a hardware timer 
- enable_timer_interrupt: function that enables the global interrupts for timer 2. 

#### LED Module
- initialise_leds: configures the LEDs so they can be turned on and off
- enable_clocks: enables the peripherals to use the LEDs 
- chase_led: a function that makes a single LED go around in circles 

### Testing
Test values for period ranging from 100-10000ms. These delays will be visible, at 0.1s-10s intervals. One can verify the accuracy of the timer by timing how long it takes for the LEDs to change 10 times, then averaging the period. 


## Exercise 3.2: 
### Summary
This task involved the application of get/set functions to change timer settings like the period. 

### Usage
Each of the get/set functions require a pointer to the timer struct containing the relevant information, and the set functions also require the variable to be set. 

### Valid input
There is a predefined struct named TIM2_init that contains valid initialisation paramters for timer 2, and the period can be set from 1 to 2^{32}-1 milliseconds. The timer completion function can also be set using a pointer to a function outputting nothing and without any inputs. 

### Functions and modularity
#### Timer Module 
A module containing the struct and functions for to initialise a hardware timer. Includes: 
- TimerInitialise: initialise and start the timer,
- enable_timer_interrupt: enable global interrupts,
- The associated get/set functions for the timer. 

#### LED Module
This module is unchanged. 

### Testing
Running the task_3_2 project will demonstrate periods of decreasing length. Otherwise, one can input a delay of 1000 and test 10 cycles, then average to confirm the average period. 

### Exercise 4:
