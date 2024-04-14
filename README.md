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
#### Part C - Interrupt-Based Receiving:
- Adapts the serial receiving process to an interrupt-based approach
- Interrupt used to handle data reception dynamically
#### Part D - Advanced Functionality:
- Re-transmits the string using interrupts
- Double buffer system:
  - Double buffer used to allow the serial interface to continue receiving new characters into one buffer while the previously received string is being procesed from another buffer
  - Swaps between two buffers upon completing the reception of one string, alloiwing continuous data handling without losing data
  - Buffer switching managing in the interrupt handler to ensure smooth flow of data and efficient processing

### Exercise 3:


### Exercise 4:
