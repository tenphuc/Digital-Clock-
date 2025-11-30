#include "uart_driver.h"

void init_uart(uint8_t usart, uint32_t BRR)
{
	/*
	 * USART_2: PA2 (TX), PA3 (RX)
	 * USART_1: PA9 (TX), PA10 (RX)
	 * USART_3: PB10 (TX), PB11 (RX)
	 *
	 */
		// Enable Alternative Function
		RCC_APB2ENR |= 0x01;
		if(usart == 1)
		{
			// Enable USART 1
			RCC_APB2ENR |= 1 << 14;
			// Enable the related PINs
			init_GP(PA, 10, IN, IN_PULL_UP_OR_DOWN);
			init_GP(PA, 9, OUT_50MHZ, OUT_GP_PUSH_PULL);
			// Setup baud rate
			USART1->BRR = generated_baudrate(usart, BRR);
			// Enable UART transmit
			USART1->CR1 |= 8;
			// Enable UART receive
			USART1->CR1 |= 4;
			// Enable USART
			USART1->CR1 |= 0x2000;
		}
		// Enable UART 2
		else if(usart == 2)
		{
			RCC->APB1ENR |= 0x20000;
			// Enable the related PINs
			init_GP(PA, 3, IN, IN_PULL_UP_OR_DOWN);
			init_GP(PA, 2, OUT_50MHZ, OUT_GP_PUSH_PULL);
			// Setup Baud rate
			USART2->BRR = generated_baudrate(usart, BRR);
			// Enable UART transmit
			USART2->CR1 |= 8;
			// Enable UART receive
			USART2->CR1 |= 4;
			// Enable USART
			USART2->CR1 |= 0x2000;
		}
		else if (usart == 3)
		{
			RCC->APB1ENR |= 1 << 18;
			// Enable the related PINs
			init_GP(PB, 11, IN, IN_PULL_UP_OR_DOWN);
			init_GP(PB, 10, OUT_50MHZ, OUT_GP_PUSH_PULL);
			// Setup Baud rate
			USART3->BRR = generated_baudrate(usart, BRR);
			// Enable UART transmit
			USART3->CR1 |= 8;
			// Enable UART receive
			USART3->CR1 |= 4;
			// Enable USART
			USART3->CR1 |= 0x2000;
		}

}
uint32_t generated_baudrate(uint8_t usart, uint32_t baud)
{
	uint32_t div = 36000000;
	uint32_t dec = 0;
	uint32_t final = 0;
	double frac = 36000000.00;
	double frac1 = 1.00;
	if(usart == 1)
	{
		div = 72000000;
		frac = 72000000.00;
	}
	div = div / (16 * baud);
	frac = 16 * (frac / (16 * baud) - div);
	dec = frac;
	frac1 = (frac - dec) * 100;
	if(frac1 > 50)
	{
		dec ++;
		if(dec == 16)
		{
			dec = 0;
			div ++;
		}

	}
	final += (div << 4);
	final += dec;
	return final;

}
uint8_t rx_data(uint8_t usart)
{
	char c = '\0';
	if(usart == 1)
	{
		while((USART1->SR & 0x20) == 0x00){};
		c = USART1->DR;
	}
	else if (usart == 2)
	{
		while((USART2->SR & 0x20) == 0x00){};
		c = USART2->DR;
	}
	else if(usart == 3)
	{
		while((USART3->SR & 0x20) == 0x00){};
		c = USART3->DR;
	}
	return c;
}
void tx_data(uint8_t usart, uint8_t c)
{
	if(usart == 1)
	{
		while((USART1->SR & (1<<7) ) == 0x80){};
		USART1->DR = c;
	}
	else if (usart == 2)
	{
		while((USART2->SR & (1<<7) ) == 0x80){};
		USART2->DR = c;
	}
	else if(usart == 3)
	{
		while((USART3->SR & (1<<7) ) == 0x80){};
		USART3->DR = c;
	}
}
