#ifndef __GPIO_DRIVE_H
#define __GPIO_DRIVE_H
#include <stdint.h>
#define RCC_APB2ENR 	(*((volatile unsigned long*) 0x40021018))
#define GPIO_A 			(*((volatile unsigned long*) 0x40010800))
#define GPIO_B 			(*((volatile unsigned long*) 0x40010C00))
#define GPIO_C 			(*((volatile unsigned long*) 0x40011000))
#define GPIO_D			(*((volatile unsigned long*) 0x40011400))
#define GPIO_E			(*((volatile unsigned long*) 0x40011800))

// List of ports
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5

// Directions
#define IN 			0
#define OUT_10MHZ 	1
#define OUT_2MHZ 	2
#define OUT_50MHZ 	3

// Direction options for input
#define IN_ANALOG 	0
#define IN_FLOATING 1
#define IN_PULL_UP_OR_DOWN 2

// Direction options for output
#define OUT_GP_PUSH_PULL	0
#define OUT_GP_OPEN_DRAIN	1
#define OUT_AF_PUSH_PULL	2
#define OUT_AF_OPEN_DRAIN	3
// Status
#define HIGH 1
#define LOW 0
// Define Functions
void init_GP(uint8_t port, uint8_t pin, uint8_t dir, uint8_t opt);
uint16_t R_GP(uint8_t port, uint8_t pin);
void W_GP(uint8_t port, uint8_t pin, uint8_t status);
void toggle_GP(uint8_t port, uint8_t pin);

#endif
