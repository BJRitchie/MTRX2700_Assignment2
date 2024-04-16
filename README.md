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

## Exercise 2.1 & 2.2: UART Receiving & Transmitting
### Summary
This task involved creating a module that interfaces with the UART to read incoming characters into a memory buffer until a terminating character was received (2.1), and then transmitting the stored string back over the UART (2.2). 

### Usage
This task can be used for communication over UART using polling. It supports both receiving and transmitting with the UART.

### Valid input
Type a string into the terminal. 

### Functions and modularity
These tasks consist of one core module for the serial handling.

#### Serial Module
- finished_transmission: simulates a delay after transmission
- receive_callback: Receives and then transmits string back over UART
- run_serial_test: Calls the serial test using polling
- SerialInitialise: Initialises the serial port and specified baud rate and callback functions

### Testing
Run the code. Type a string into the terminal. The string will be sent back to the terminal. 

## Exercise 2.3 & 2.4: Interrupt Based Approach & Double Buffer 
### Summary
This task involved replacing the current serial interface communication with UART to an interrupt based approach for receiving (2.3) and transmitting over UART using a double buffer (2.4) to allow the serial to continue receiving characters while a different function can use the string. 

### Usage
An interrupt based approach including a double buffer enables for modularity in the code so other modules can function simultaneously. 

### Valid input
The same as for 2.1 & 2.2.

### Functions and modularity
These tasks consist of the same module for serial handling, changed for interrupt capabilites.

#### Serial Module
- received_new_string: Receives and then transmits string over UART, adds carriage return, new line and NULL terminator to the end of the string before transmitting
- SerialInitalise: Additionally configures and enables USART interrupt
- USART1_IRQHandler: Handles data reception and buffer swapping

### Testing
Run the code. Type a string into the terminal. The string will be sent back to the terminal. Implementation with other modules during exercise 4 demonstrates the interrupt working whilst other modules are functioning.



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


## Exercise 3.3: 
### Summary
This task uses a timer in a one-shot configuration. That is, the timer is initialised with a specified delay time, then completes some program when the delay time is up, then stops. This program uses two timers in a one-shot configuration to first turn on the board's LEDs, then turn them off. 

### Usage
The setOneShot function can be used to start the timer in a one-shot mode, and takes a completion function as a paramter so the user can complete some function after a specific delay. 

### Valid input
The setOneShot function requires a pointer to a HardwareTimer struct containing initialisation parameters. It also requires an input defining the delay, which can be set from 1 to 2^{32}-1. Finally, it requires a pointer to a function that returns nothing and takes no inputs, for the completion function.

### Functions and modularity
#### Timer Module 
This module is unchanged besides the setOneShot function discussed above. 

#### LED Module
This module is unchanged. 

### Testing
One can adjust the delay parameters for TIM2 and TIM4, seen in the main file to show the operation of the module. Both are initialised at (essentially) the same time and run parallel. As such, if the first delay was 1000ms, and the second delay was 5000ms, the LEDs would turn on after 1 second, remain on for 4 seconds, then turn off and remain off.


## Exercise 4: 
### Summary
This task combines all of the previous modules. It required each module to use interrupts to carry out their functions to allow each task to be carried out simultaneously, enabling modularity. 

### Usage
Any one of four modules (LED, serial, timer, or oneshot) can be called to carry out a specific function. 

### Valid input
Type one of four commands into the terminal (‘led’, ‘serial’, ‘timer’, or ‘oneshot’) followed by the appropriate parameter for the command.

### Functions and modularity
#### Timer Module 
- enable_timer_interrupt: function that enables the global interrupts for timer 2 and 4

#### LED Module
- enable_clocks: enables the peripherals to use the LEDs
- initialise_leds: configures the LEDs

#### Serial Module
- SerialInitialise: Initialises the serial port
- 
### Testing
Run the code. Type one of the four commands into the terminal and then the desired parameter. The function of the command will be transmitted to the PuTTY terminal (as well as the board for the ‘led’ function). Type all commands into the terminal with their respective desired parameters. The interrupt based approach is proven to be working if all commands work simultaneously, proving modularity. 


