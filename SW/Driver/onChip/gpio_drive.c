#include "gpio_drive.h"
#include <stdint.h>
void init_GP(uint8_t port, uint8_t pin, uint8_t dir, uint8_t opt)
{
	volatile uint32_t* CR;
	uint8_t tPIN = pin;
	uint8_t offset = 0x00;
	if(pin > 7 )
	{
		tPIN -= 8;
		offset = 0x01;
	}
	if( port == 1)
	{
		RCC_APB2ENR |= 4;
		CR = (volatile uint32_t *)(&GPIO_A + offset);
	}
	else if (port == 2)
	{
		RCC_APB2ENR |= 8;
		CR = (volatile uint32_t *)(&GPIO_B + offset);
	}
	else if (port == 3)
	{
		RCC_APB2ENR |= 16;
		CR = (volatile uint32_t *)(&GPIO_C + offset);
	}
	*CR &= ~(0xf << (tPIN) * 4); // reset the CR register
	*CR |= (dir << (tPIN) * 4) | (opt <<(tPIN * 4 + 2));// set up the direction and the option of the pin

}
uint16_t R_GP(uint8_t port, uint8_t pin)
{
	volatile uint32_t* IDR;
	uint32_t offset = 0x02;
	uint32_t state;
	if(port == 1)
	{
		IDR = (volatile uint32_t *)(&GPIO_A + offset);
	}
	else if (port == 2)
	{
		IDR = (volatile uint32_t *)(&GPIO_B + offset);
	}
	else if (port == 3)
	{
		IDR = (volatile uint32_t *)(&GPIO_C + offset);
	}
	state = (*IDR & (1 << pin)) >> pin;
	return state;
}
void W_GP(uint8_t port, uint8_t pin, uint8_t status)
{
		volatile uint32_t *ODR;
		uint32_t offset = 0x03;
		if(port == 1)
		{
			ODR = (volatile uint32_t *)(&GPIO_A + offset);
		}
		else if (port == 2)
		{
			ODR = (volatile uint32_t *)(&GPIO_B + offset);
		}
		else if (port == 3)
		{
			ODR = (volatile uint32_t *)(&GPIO_C + offset);
		}
		*ODR = status ? (*ODR |= (status << pin)) : (*ODR &= ~(1 << pin));
}
void toggle_GP(uint8_t port, uint8_t pin)
{
	if(R_GP(port, pin))
	{
		W_GP(port, pin, 0);
	}
	else
	{
		W_GP(port, pin, 1);
	}
}
