/*
 * ds3231_RTC.h
 *
 * Created: 26-12-2021 23:03:51
 *  Author: AK
 */ 


#ifndef DS3231_RTC_H_
#define DS3231_RTC_H_

#define DS3231READ 0xD1
#define DS3231WRITE 0xD0
#define CTRLREG 0x0E
#include <avr/io.h>
#include "I2C.h"

typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekDay;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	
}rtc_t;

void RTC_Init(void);
void RTC_SetDateTime(rtc_t *rtc);
void RTC_GetDateTime(rtc_t *rtc);
uint8_t BcdToByte(uint8_t bcd);
uint8_t ByteToBcd(uint8_t i);



#endif /* DS3231_RTC_H_ */
