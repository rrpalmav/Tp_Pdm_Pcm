/*
 * ds3231.h
 *
 *  Created on: Aug 9, 2024
 *      Author: rpalma
 */


#ifndef DS3221_H
#define DS3221_H

#include "stdint.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"




#ifndef DS3231_I2C_ADDRESS
#define DS3231_I2C_ADDRESS         0xD0
#endif

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
} TIME;

typedef struct {
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} DATE;

typedef enum{
	RTC_inactivo,
	RTC_running,
	RTC_set_hora,
	RTC_set_fecha,
} ESTADOS;


uint8_t decToBcd(int val);
int bcdToDec(uint8_t val);



void init();
void set_Time (uint8_t sec, uint8_t min, uint8_t hour);
TIME get_Time (void);
void set_Date (uint8_t dom, uint8_t month, uint8_t year);
DATE get_date (void);

#endif /* API_INC_DS3231_H_ */
