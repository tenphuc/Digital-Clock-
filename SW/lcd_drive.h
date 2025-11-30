#ifndef __LCD_DRIVE_H
#define __LCD_DRIVE_H
#include "systick_timer.h"
#include "gpio_drive.h"
/*
 * PINs setup
 * PA8 -> RS
 * PA9 -> RW
 * PA10 -> E
 * PA0 -> DB0
 * PA1
 * PA2
 * PA3
 * PA4
 * PA5
 * PA6
 * PA7
 */
void pin_ouput(void);
void lcd_init(void);
void lcd_rs(uint8_t HIGH_LOW);
void lcd_rw(uint8_t HIGH_LOW);
void lcd_e(uint8_t HIGH_LOW);
void lcd_output_data(uint8_t data);
void lcd_data(uint8_t data);
void lcd_cmd(uint8_t data);
void lcd_output_string(char *s1);
void lcd_msg(uint8_t pos_line, uint8_t pos_0_16, char *s);
void lcd_clear();
#endif
