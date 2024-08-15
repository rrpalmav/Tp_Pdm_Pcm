/*
 * ds3231.c
 *
 *  Created on: Aug 10, 2024
 *      Author: rpalma
 */

#include <API_ds3231.h>


extern I2C_HandleTypeDef hi2c2;
extern TIME time;
extern DATE date;

ESTADOS state;

void init(){
	state = RTC_inactivo;
	ds3231FSM_update('\0');
}

// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}


void set_time (uint8_t sec, uint8_t min, uint8_t hour)
{
	uint8_t set_time[7];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);

	HAL_I2C_Mem_Write(&hi2c2, DS3231_I2C_ADDRESS, 0x00, 1, set_time, 7, 1000);
}




void set_date(uint8_t dom, uint8_t month, uint8_t year){
	uint8_t set_time[7] = {{0}};
	set_time[4] = decToBcd(dom);
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);
	HAL_I2C_Mem_Write(&hi2c2, DS3231_I2C_ADDRESS, 0x00, 1, get_time, 7, 1000);
}

void get_time (void)
{
	uint8_t get_time[7];
	HAL_I2C_Mem_Read(&hi2c2, DS3231_I2C_ADDRESS, 0x00, 1, get_time, 7, 1000);
	time.seconds = bcdToDec(get_time[0]);
	time.minutes = bcdToDec(get_time[1]);
	time.hour = bcdToDec(get_time[2]);

}

void get_date(void){
	uint8_t get_date[7];
	HAL_I2C_Mem_Read(&hi2c2, DS3231_I2C_ADDRESS, 0x00, 1, get_date, 7, 1000);
	date.dayofmonth = bcdToDec(get_time[4]);
	date.month = bcdToDec(get_time[5]);
	date.year = bcdToDec(get_time[6]);
}


void ds3231FSM_update(char valor){
	switch(state){
		case RTC_inactivo:
			if(valor == 'D')
			state = RTC_running;
			break;
		case RTC_running:
			if(valor == 'I') state = RTC_inactivo;
			if(valor == 'A') state = RTC_set_hora;
			if(valor == 'B') state = RTC_set_fecha;
			break;
		case RTC_set_hora:
			if(valor == 'C') state = RTC_running;
			break;
		case RTC_set_fecha:
			if(valor == 'C') state = RTC_running;
			break;
		default:
			init();
			break;
		}

}
