#include "systick_timer.h"
void systick_init(void)
{
	STK_CRL = 0;
	STK_LOAD = 0x00ffffff;
	STK_VAL = 0;
	STK_CRL = 5;
}
void systick_enable_it(void)
{
	__disable_irq();
	STK_CRL = 0;
	STK_LOAD = 7200000;
	STK_VAL = 0;
	STK_CRL |= 7;
	__enable_irq();
}
void delays_miliseconds(void)
{
	STK_LOAD = 72000 - 1;
	STK_VAL = 0;
	while((STK_CRL & 0x00010000) == 0);
}
void delays_microseconds(void)
{
	STK_LOAD = 720 - 1;
	STK_VAL = 0;
	while((STK_CRL & 0x00010000) == 0x00);
}
void delays_us(uint32_t time)
{
	while(time --)
	{
		delays_microseconds();
	}
}
void delays_ms(uint32_t time)
{
	// limited by 50 days
	while(time --)
	{
		delays_miliseconds();
	}
}
void delays_sec(uint32_t time)
{
	while(time--)
	{
		delays_ms(1000);
	}
}
