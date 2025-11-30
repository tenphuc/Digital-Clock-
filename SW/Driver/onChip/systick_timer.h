#ifndef __SYSTICK_TIMER_H
#define __SYSTICK_TIMER_H
#include <stdint.h>
#define STK_CRL		(*((volatile unsigned long*) 0xE000E010))
#define STK_LOAD	(*((volatile unsigned long*) 0xE000E014))
#define STK_VAL		(*((volatile unsigned long*) 0xE000E018))
#define STK_CALIB	(*((volatile unsigned long*) 0xE000E01C))

void systick_init(void);
void delays_miliseconds(void);
void delays_microseconds(void);
void delays_ms(uint32_t time);
void delays_us(uint32_t time);

void systick_enable_it(void);
#endif
