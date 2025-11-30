//#include "stm32f103xb.h"
#include <stdint.h>
#include <stdlib.h>
#include "stm32f1xx.h"
#include "gpio_drive.h"
#include "systick_timer.h"
#include "uart_driver.h"
#include "lcd_drive.h"
#include "oled_i2c.h"
#include "i2c_driver.h"

#include "ds3231.h"
#include "aht20.h"
#include "support.h"
// debug
#define main_loop (0)
//#define set_alarm (1)
//#define AHT20 (2)
//#define pomodoro (3)
volatile char mODE = 0, mOVE = 0, sET = 0;
volatile char signal_buzzer = 0;
// time
char yr, month, day, hrs, min, sec;
char time[12], date[12], time_alarm[12];
char hrs_alarm = 0, min_alarm = 0;
char time_table[7];
char check_mode1 = 0, check_mode3 = 0;
char shum[12], stemp[12];
float hum, temp;
//
uint8_t sec_mode3 = 60, min_mode3 = 25;
char time_mode3[12];
void Buttons_GPIO_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRH &= ~(0xF << (0 * 4));
    GPIOA->CRH |=  (0x8 << (0 * 4));
    GPIOA->CRH &= ~(0xF << (1 * 4));
    GPIOA->CRH |=  (0x8 << (1 * 4));
    GPIOA->CRH &= ~(0xF << (2 * 4));
    GPIOA->CRH |=  (0x8 << (2 * 4));
   // pull up
    GPIOA->ODR |= (1 << 8) | (1 << 9) | (1 << 10);
}

void Buttons_EXTI_Init(void)
{

    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;


    AFIO->EXTICR[2] &= ~(AFIO_EXTICR3_EXTI8  |
                         AFIO_EXTICR3_EXTI9  |
                         AFIO_EXTICR3_EXTI10);

    EXTI->FTSR |=  (1 << 8) | (1 << 9) | (1 << 10);
    EXTI->RTSR &= ~((1 << 8) | (1 << 9) | (1 << 10));

    EXTI->IMR |= (1 << 8) | (1 << 9) | (1 << 10);

    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void TIM2_Init_30s(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 7200 - 1;
    TIM2->ARR = 300000 - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM2->CR1 |= TIM_CR1_CEN;
}
void TIM2_Stop(void)
{
    NVIC_DisableIRQ(TIM2_IRQn);
    TIM2->DIER &= ~TIM_DIER_UIE;
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;
        signal_buzzer = 0;
        W_GP(PB, 6, 0);
        TIM2_Stop();
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI->PR & (1 << 8))
    {
        EXTI->PR = (1 << 8);
        delays_ms(20);
        if (!(GPIOA->IDR & (1 << 8)))
        {
            while (!(GPIOA->IDR & (1 << 8))){}
             mODE ++;
             check_mode3 = 0;
        }
        if(mODE > 3) mODE = 0;
    }

    if (EXTI->PR & (1 << 9))
    {
        EXTI->PR = (1 << 9);
        //
        delays_ms(20);
        if (!(GPIOA->IDR & (1 << 9)))
        {
            while (!(GPIOA->IDR & (1 << 9))){}
            mOVE ++;
            sET = 0;
            check_mode3 = 0;
        }
        if(mODE == 0)
        {
        	mOVE = 0;
        	signal_buzzer = 0;
        	W_GP(PB, 6, 0);
        }
        else if (mODE == 1)
        {
        	if(mOVE == 1|| mOVE == 4 || mOVE == 9  ||
        			mOVE == 12 || mOVE == 15) mOVE += 2;
        	if(mOVE == 7)mOVE = 8;
        	if(mOVE >= 16) mOVE = 0;
        	if(mOVE == 2 || mOVE == 5|| mOVE == 10 || mOVE ==  13)mOVE ++;
        }
        else if (mODE == 2)
        {
        	if(mOVE == 1 || mOVE == 4) mOVE += 2;
        	if(mOVE == 2 ) mOVE ++;
        	if(mOVE >= 5) mOVE = 0;
        }
        else if (mODE == 3)
        {
        	mOVE = mOVE > 1 ? 0 : mOVE;
        }
    }
    if (EXTI->PR & (1 << 7))
       {
    		EXTI->PR |= (1 << 7);

//    		delays_ms(20);
//    		if (!(GPIOB->IDR & (1 << 7)))
////    		{
//    			while (!(GPIOB->IDR & (1 << 7)));
//
//    			init_GP(PC, 13, OUT_50MHZ, OUT_GP_PUSH_PULL);
//    			toggle_GP(PC, 13);

    			signal_buzzer = 1;
    			W_GP(PB, 6, 1);

//    			lcd_msg(2, 10, "ok");

    			TIM2_Init_30s();
//    		}
       }
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & (1 << 10))
    {
       EXTI->PR = (1 << 10);
       //
       delays_ms(20);
         if (!(GPIOA->IDR & (1 << 10)))
         {
             while (!(GPIOA->IDR & (1 << 10))){}
              sET ++;
         }
    }
    if(mODE == 0)
    {
    	sET = 0;
    	signal_buzzer = 0;
    	W_GP(PB, 6, 0);
    }
    if(mODE == 3)
    {
    	check_mode3 = 1;
    }
}


void EXTI7_Init(void)
{

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;


    GPIOB->CRL &= ~(0xF << (7 * 4));
    GPIOB->CRL |=  (0x8 << (7 * 4));   // input
    GPIOB->ODR |=  (1 << 7);           // pull-up


    AFIO->EXTICR[1] &= ~(0xF << 12);   // clear EXTI7 bits
    AFIO->EXTICR[1] |=  (0x1 << 12);   // 0001 = PB7


    EXTI->IMR  |=  (1 << 7);   // enable
    EXTI->EMR  &= ~(1 << 7);
    EXTI->RTSR &= ~(1 << 7);   // disable rising edge
    EXTI->FTSR |=  (1 << 7);   // enable falling edge


    NVIC_EnableIRQ(EXTI9_5_IRQn);
}


int main ()
{
	init_GP(PB, 6, OUT_50MHZ, OUT_GP_PUSH_PULL);
//	W_GP(PB, 6, HIGH);
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR &= ~(7 << 24);
	AFIO->MAPR |=  (2 << 24);
	systick_init();

		lcd_init();
		i2c_init(2, i2c_FM);
		Buttons_GPIO_Init();
		Buttons_EXTI_Init();
		EXTI7_Init();
#ifdef main_loop
		lcd_msg(1, 5, "P_CLOCK");
		lcd_msg(2, 2, "DESIGN BY CNP");
		aht20_init(2);
		delays_ms(500);
		while(1)
		{
			lcd_clear();
			while(!mODE )
			{
				DS3231_get_time(2,time_table);
				aht20_read(2, &hum, &temp);
				int2time(DS3231_reverter(time_table[2]),time);
				int2time(DS3231_reverter(time_table[1]),time);
				int2time(DS3231_reverter(time_table[0]),time);
								// SEND time using UART
				lcd_msg(1, 0, time);
				int2date(DS3231_reverter(time_table[4]),date);
				int2date(DS3231_reverter(time_table[5]),date);
				int2date(DS3231_reverter(time_table[6]),date);
				lcd_msg(2, 0, date);
				ftoa_1(hum, shum);
				ftoa_1(temp, stemp);
				lcd_msg(1,11,shum);
				lcd_msg(1,15,"%");
				lcd_msg(2,11, stemp);
				lcd_msg(2,15,"C");
				str_empty(time);
				str_empty(date);
				W_GP(PB, 6, signal_buzzer);
			}

//			lcd_cmd(0x0F);
//			delays_us(50);
			lcd_clear();
			lcd_msg(1, 10, "S_TIME");
			while (mODE == 1)
			{
				switch(mOVE)
				{
				case 0:
					lcd_msg(2, 14, "HR");
					break;

				case 3:
					lcd_msg(2, 14, "MI");
					break;
				case 6:
					lcd_msg(2, 14, "SE");
					break;
				case 8:
					lcd_msg(2, 14, "DD");
					break;
				case 11:
					lcd_msg(2, 14, "MO");
					break;
				default:
					lcd_msg(2, 14, "YY");
					break;
				}


				DS3231_get_time(2,time_table);


				hrs = DS3231_reverter(time_table[2]);
				min = DS3231_reverter(time_table[1]);
				sec = DS3231_reverter(time_table[0]);
				day = DS3231_reverter(time_table[4]);
				month = DS3231_reverter(time_table[5]);
				yr = DS3231_reverter(time_table[6]);
				if(sET > 0) check_mode1 = 1;
				if(mOVE == 0)	hrs += sET, sET = 0;
				if(mOVE == 3)	min += sET, sET = 0;
				if(mOVE == 6)	sec += sET, sET = 0;
				if(mOVE == 8)	day += sET, sET = 0;
				if(mOVE == 11)	month += sET, sET = 0;
				if(mOVE == 14)	yr += sET, sET = 0;
				//
				if(hrs >= 24)	hrs = 0, sET = 0;
				if(min >= 60)	min = 0, sET = 0;
				if(sec >= 60)	sec = 0, sET = 0;
				if(day >= 32)	day = 0, sET = 0;
				if(month >= 13)	month = 0, sET = 0;
				if(yr  >= 99)	yr = 0, sET = 0;
				sET = 0;
				int2time(hrs,time);
				int2time(min,time);
				int2time(sec,time);
									// SEND time using UART
				lcd_msg(1, 0, time);
				int2date(day,date);
				int2date(month,date);
				int2date(yr,date);
				lcd_msg(2, 0, date);

				str_empty(time);
				str_empty(date);

				if(check_mode1)
				{
					DS3231_update_date_time(2,yr,month,day,hrs,min,sec);

					check_mode1 = 0;
				}
			}
			mOVE = 0;
			sET = 0;
			lcd_clear();
			lcd_msg(1, 11, "ALARM");


			while (mODE == 2)
			{

				if (mOVE == 0)lcd_msg(2, 14, "HR");
				else if (mOVE == 3)lcd_msg(2, 14, "MI");
				// get from register
				char alarm_reg[5]={};
				DS3231_rx(2, 0x0B, alarm_reg, 5);
				hrs_alarm = DS3231_reverter(alarm_reg[1]);
				min_alarm = DS3231_reverter(alarm_reg[0]);
				//
				DS3231_tx(2, 0x0D, alarm_reg[2] | 1 << 7);
				alarm_reg[3] &= ~(1 << 0);
				DS3231_tx(2, 0x0E, alarm_reg[3] | (1 << 2 | 1 << 1));
				//
				if(mOVE == 0)hrs_alarm += sET, sET = 0;
				else if(mOVE == 3)min_alarm += sET, sET = 0;
				if(hrs_alarm >=25)hrs_alarm = 0, sET = 0;
				if(min_alarm >=60)min_alarm = 0, sET = 0;
				sET = 0;
				if(hrs_alarm == 24 && min_alarm == 0)
				{
					alarm_reg[3] &= ~((1 << 2) | (1 << 1));
					DS3231_tx(2, 0x0E, alarm_reg[3]);
				}
				DS3231_tx(2, 0x0B, DS3231_converter(min_alarm));
				DS3231_tx(2, 0x0C, DS3231_converter(hrs_alarm));
				int2time(hrs_alarm,time_alarm);
				int2time(min_alarm,time_alarm);
//				int2time(alarm_reg[3],time_alarm);
//				int2time(alarm_reg[4] & 0x02,time_alarm);
//				int2time(signal_buzzer,time_alarm);
				lcd_msg(1, 0, time_alarm);
				str_empty(time_alarm);
				W_GP(PB, 6, signal_buzzer);
			}
//			DS3231_set_alarm(2, hrs_alarm, min_alarm);
			lcd_clear();
			mOVE = 0;
			sET = 0;
			lcd_cmd(0x0C);
			delays_us(50);
			lcd_msg(1, 8, "POMODORO");
			char reg[2]={};
			DS3231_rx(2, 0x0E, reg,2);
			DS3231_tx(2, 0x0F , reg[1] & ~(1 << 1));
			while (mODE == 3)
			{
				if(mOVE == 0)
				{
					lcd_msg(1, 0, "25:00");
					min_mode3 = 24;

				}
				else if(mOVE == 1)
				{
					lcd_msg(1, 0, "50:00");
					min_mode3 = 49;
				}
				lcd_msg(2, 11, "MINS");
				uint8_t tmp_min =0, rest = 0;
				tmp_min = min_mode3;
				while(check_mode3)
				{
					sec_mode3 --;
					if(sec_mode3 == 0xff)
					{
						sec_mode3 = 59;
						min_mode3 --;
					}
					int2time(min_mode3, time_mode3);
					int2time(sec_mode3, time_mode3);
					lcd_msg(1,0, time_mode3);
					delays_ms(100);
					str_empty(time_mode3);
					if(min_mode3 == 0 && sec_mode3 == 0)
					{
						W_GP(PB,6, 1);
						delays_ms(50);
						W_GP(PB,6, 0);

						if(rest == 1)
						{
							min_mode3 = tmp_min;
							sec_mode3 = 60;
							rest = 0;
							lcd_msg(2,11,"MINS");
						}
						else
						{
							min_mode3 = 9;
							sec_mode3 = 60;
							rest = 1;
							lcd_msg(2,11,"REST");
						}
					}
					W_GP(PB, 6, signal_buzzer);
				}
				check_mode3 = 0;
				W_GP(PB, 6, signal_buzzer);
			}
		}

#elif set_alarm
		mODE = 2;
		while (2)
					{
						if (mOVE == 0)lcd_msg(2, 14, "HR");
						else if (mOVE == 3)lcd_msg(2, 14, "MI");

						if(mOVE == 0)hrs_alarm += sET, sET = 0;
						if(mOVE == 3)min_alarm += sET, sET = 0;
						sET = 0;
		//				DS3231_set_alarm(2, hrs, min);
						int2time(hrs_alarm,time_alarm);
						int2time(min_alarm,time_alarm);
						lcd_msg(2, 0, time_alarm);
						str_empty(time_alarm);
					}
#elif AHT20

			while(1)
			{
				aht20_read(2, &hum, &temp);
				ftoa_1(hum, shum);
				ftoa_1(temp, stemp);
				lcd_msg(1,0,shum);
				lcd_msg(2,0, stemp);
			}
#elif pomodoro
		mODE = 3;
//		check_mode3 = 1;
		while(1)
		{

			if(mOVE == 0)
			{
				lcd_msg(1, 0, "25:00");
				min_mode3 = 24;

			}
			else if(mOVE == 1)
			{
				lcd_msg(1, 0, "50:00");
				min_mode3 = 49;
			}
			lcd_msg(2, 11, "MINS");
			while(check_mode3)
			{
				sec_mode3 --;
				if(sec_mode3 < 0)
				{
					sec_mode3 = 59;
					min_mode3 --;
				}
				int2time(min_mode3, time_mode3);
				int2time(sec_mode3, time_mode3);
				lcd_msg(1,0, time_mode3);
				delays_ms(100);
				str_empty(time_mode3);
				if(min_mode3 == 0 && sec_mode3 == 0)
				{
					while(1);
				}
			}
			check_mode3 = 0;
		}
#endif
		sET = 0;

}
