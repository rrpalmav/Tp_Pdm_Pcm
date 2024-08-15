/*
 * ssd1306.h
 *
 *  Created on: Aug 9, 2024
 *      Author: rpalma
 */

#ifndef SSD1306_H
#define SSD1306_H 100

/**
 * Pantalla Oled
 * This SSD1306 LCD uses I2C for communication
 * Default pinout
 *
 */

#include "stm32f4xx_hal.h"

#include "fonts.h"

#include "stdlib.h"
#include "string.h"


/* SSD1306 settings */
/* SSD1306 width in pixels */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH            128
#endif
/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT           64
#endif

/* I2C address */
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR         0x78
#endif

#ifndef ssd1306_I2C_TIMEOUT
#define ssd1306_I2C_TIMEOUT					20000
#endif


typedef enum {
	SSD1306_COLOR_BLACK = 0x00,
	SSD1306_COLOR_WHITE = 0x01
} SSD1306_COLOR_t;


uint8_t SSD1306_Init(void);
void SSD1306_UpdateScreen(void);
void SSD1306_ToggleInvert(void);
void SSD1306_Fill(SSD1306_COLOR_t Color);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
void SSD1306_GotoXY(uint16_t x, uint16_t y);
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);
void SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row);
void SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row);
void SSD1306_Scrolldiagright(uint8_t start_row, uint8_t end_row);
void SSD1306_Scrolldiagleft(uint8_t start_row, uint8_t end_row);
void SSD1306_Stopscroll(void);
void SSD1306_InvertDisplay (int i);
void SSD1306_Clear (void);

#endif
