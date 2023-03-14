/*
 * RTC.c
 *
 * Created: 26-12-2022 23:04:14
 *  Author: AK
 */ 
#include "DS3231_RTC.h"


void RTC_Init(void)
{
	I2C_Init();                             // Initialize the I2c module.
	I2C_Start(DS3231WRITE);					// Start I2C communication
	
	I2C_Write(CTRLREG);						// Select the Ds1307 ControlRegister to configure Ds1307
	
	I2C_Write(0x00);                        // Write 0x00 to Control register to disable SQW-Out
	
	I2C_Stop();                             // Stop I2C communication after initializing DS3231
}
void RTC_SetDateTime(rtc_t *rtc)
{
	I2C_Start(DS3231WRITE);                            // Start I2C communication

	//I2C_Write(DS3231WRITE);      // connect to DS1307 by sending its ID on I2c Bus
	I2C_Write(0x00); // Request sec RAM address at 00H
	
	I2C_Write(rtc->sec);                    // Write sec from RAM address 00H
	I2C_Write(rtc->min);                    // Write min from RAM address 01H
	I2C_Write(rtc->hour);                    // Write hour from RAM address 02H
	I2C_Write(rtc->weekDay);                // Write weekDay on RAM address 03H
	I2C_Write(rtc->date);                    // Write date on RAM address 04H
	I2C_Write(rtc->month);                    // Write month on RAM address 05H
	I2C_Write(rtc->year);                    // Write year on RAM address 06h
	
	I2C_Stop();                              // Stop I2C communication after Setting the Date
}
void RTC_GetDateTime(rtc_t *rtc)
{
	I2C_Start(DS3231WRITE);                            // Start I2C communication and connect to DS3132 in Write by SLA+W
	
	I2C_Write(0x00);							// Request Seconds register RAM address 00H
	I2C_Stop();                                // Stop I2C communication after selecting Seconds Register
	I2C_Start(DS3231READ);                     // Start I2C communication in read by SLA+R
	rtc->sec = I2C_Read_Ack();                // read second and return Positive ACK
	rtc->min = I2C_Read_Ack();                 // read minute and return Positive ACK
	rtc->hour= I2C_Read_Ack();               // read hour and return Negative/No ACK
	rtc->weekDay = I2C_Read_Ack();           // read weekDay and return Positive ACK
	rtc->date= I2C_Read_Ack();              // read Date and return Positive ACK
	rtc->month=I2C_Read_Ack();            // read Month and return Positive ACK
	rtc->year =I2C_Read_Nack();             // read Year and return Negative/No ACK
	I2C_Stop();                              // Stop I2C communication after reading the Date
}

uint8_t BcdToByte(uint8_t bcd){
	return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

uint8_t ByteToBcd(uint8_t i){
	return ((i / 10) << 4) + (i % 10);
}
