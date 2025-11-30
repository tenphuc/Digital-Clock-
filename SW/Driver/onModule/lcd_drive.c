#include "lcd_drive.h"
void pin_ouput(void)
{
	for(int i = 1;i <= 7 ;i ++)
	{
		init_GP(PA, i, OUT_50MHZ, OUT_GP_PUSH_PULL);
	}
	init_GP(PB, 0, OUT_50MHZ, OUT_GP_PUSH_PULL);
	init_GP(PB, 13, OUT_50MHZ, OUT_GP_PUSH_PULL);
	init_GP(PB, 14, OUT_50MHZ, OUT_GP_PUSH_PULL);
	init_GP(PB, 15, OUT_50MHZ, OUT_GP_PUSH_PULL);
}
void lcd_init(void)
{
	pin_ouput();
	delays_ms(50);
	lcd_cmd(0X38);
	delays_ms(5);
	lcd_cmd(0X0C);
	delays_ms(5);
	lcd_cmd(0X01);
	delays_ms(5);
	lcd_cmd(0X06);
	delays_ms(5);
}
void lcd_rs(uint8_t HIGH_LOW)
{
	W_GP(PB, 15, HIGH_LOW);
}
void lcd_rw(uint8_t HIGH_LOW)
{
	W_GP(PB, 14, HIGH_LOW);
}
void lcd_e(uint8_t HIGH_LOW)
{
	W_GP(PB, 13, HIGH_LOW);
}
void lcd_output_data(uint8_t data)
{
	for(int i = 1 ;i < 8;i ++)
	{
		W_GP(PA, i, (data >> (8 - i)) & 0x01 );
	}
	W_GP(PB, 0, (data >> 0)  & 0x01 );
}
void lcd_data(uint8_t data)
{
	pin_ouput();
	lcd_rs(HIGH);
	lcd_rw(LOW);
	lcd_output_data(data);
	delays_us(2);
    lcd_e(HIGH);
    delays_us(2);
    lcd_e(LOW);
    delays_us(5);
	delays_us(40);
}
void lcd_cmd(uint8_t data)
{
	pin_ouput();
	lcd_rs(LOW);
	lcd_rw(LOW);
	lcd_output_data(data);
	delays_us(2);
    lcd_e(HIGH);
    delays_us(2);
    lcd_e(LOW);
    delays_us(5);
	delays_us(40);
}
void lcd_output_string(char *s1)
{
	while(*s1 != '\0')
	{
		lcd_data(*s1);
		s1 ++ ;
	}
}
void lcd_msg(uint8_t pos_line, uint8_t pos_0_16, char *s)
{
	uint8_t pos = 0;
	if(pos_line == 1)
	{
		pos = 0;
	}
	else
	{
		pos = 0x40;
	}
	lcd_cmd(0x80 + pos + pos_0_16);
	delays_us(100);
	lcd_output_string(s);
}
void lcd_clear()
{
	lcd_cmd(0x01);
	  delays_ms(2);
}
