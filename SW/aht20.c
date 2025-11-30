/*
 * aht20.c
 *
 *  Created on: Nov 24, 2025
 *      Author: Windows
 */
#include "aht20.h"
#include "systick_timer.h"
void aht20_add(char i2c, char R_W)
{
	i2c_add(i2c,0x38 << 1,R_W);

}
void aht20_sendcommand(char i2c, char data)
{
	i2c_start(i2c);
	aht20_add(i2c, 0);
	i2c_data(i2c, data);
	i2c_stop(i2c);
}
char aht20_readstatus(char i2c)
{
	char status;
	i2c_start(i2c);
	aht20_add(i2c, 0);
	i2c_data(i2c, 0x71);
	//
	i2c_start(i2c);
	aht20_add(i2c, 1);
	//
	status = i2c_rx(i2c, NACK);
	i2c_stop(i2c);
	return status;

}
void aht20_init(char i2c)
{
	char status;
	delays_ms(40);
	status = aht20_readstatus(i2c);
	if(!(status & (1<<3)))
	{
		i2c_start(i2c);
		aht20_add(2, 0);
		i2c_data(i2c, 0xbe);
		i2c_data(i2c, 0x08);
		i2c_data(i2c, 0x00);
		i2c_stop(i2c);
		delays_ms(10);
	}

}

void aht20_readraw(char i2c, uint32_t *hum, uint32_t *temp)
{
	uint8_t s[7]={};
	i2c_start(i2c);
	aht20_add(i2c, 0);
	i2c_data(i2c, 0xAC);
	i2c_data(i2c, 0x33);
	i2c_data(i2c, 0x00);
	i2c_stop(i2c);


	delays_ms(80);

	i2c_start(i2c);
	aht20_add(i2c, 1);
	s[0] = i2c_rx(i2c, ACK);
	s[1] = i2c_rx(i2c, ACK);
	s[2] = i2c_rx(i2c, ACK);
	s[3] = i2c_rx(i2c, ACK);
	s[4] = i2c_rx(i2c, ACK);
	s[5] = i2c_rx(i2c, ACK);
	s[6] = i2c_rx(i2c, NACK);

	i2c_stop(i2c);
    *hum  = ((uint32_t)s[1] << 12);
    *hum |= ((uint32_t)s[2] << 4);
    *hum |= ((uint32_t)(s[3] >> 4) & 0x0F);


    *temp  = ((uint32_t)(s[3] & 0x0F) << 16);
    *temp |= ((uint32_t)s[4] << 8);
    *temp |= ((uint32_t)s[5]);

}

void aht20_soft_reset(char i2c)
{
	aht20_sendcommand(i2c, 0xBA);
	delays_ms(20);
}
void aht20_read(char i2c_bus, float *humidity, float *temperature)
{
    uint32_t raw_h, raw_t;

    aht20_readraw(i2c_bus, &raw_h, &raw_t);

    *humidity    = (raw_h * 100.0f) / 1048576.0f;       // 2^20 = 1048576
    *temperature = (raw_t * 200.0f) / 1048576.0f - 50.0f;
}
