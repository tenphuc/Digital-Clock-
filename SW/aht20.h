/*
 * aht20.h
 *
 *  Created on: Nov 24, 2025
 *      Author: Windows
 */

#ifndef __AHT20_H_
#define __AHT20_H_
#include "i2c_driver.h"
void aht20_add(char i2c, char R_W);
void aht20_sendcommand(char i2c, char data);
char aht20_readstatus(char i2c);
void aht20_init(char i2c);
void aht20_readraw(char i2c, uint32_t *temp, uint32_t *hum);
void aht20_soft_reset(char i2c);
void aht20_read(char i2c_bus, float *humidity, float *temperature);

#endif /* SRC_AHT20_H_ */
