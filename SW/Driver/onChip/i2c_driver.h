#ifndef __I2C_DRIVER_H
#define __I2C_DRIVER_H
#include "stm32f1xx.h"
#include "gpio_drive.h"

#define i2c_FM 0x2d
#define i2c_SM 0xB4
#define ACK 0
#define NACK 1



void i2c_init(char i2c,unsigned short speed_mode);
void i2c_write(char i2c, char address,char data[]);
void i2c_start(char i2c);
void i2c_add(char i2c, char address,char RW);
void i2c_data(char i2c,char data);
void i2c_stop(char i2c);
char i2c_rx(char i2c, char ACK_NACK);
void DS3231_set_alarm(char i2c, char hrs, char min);
#endif
