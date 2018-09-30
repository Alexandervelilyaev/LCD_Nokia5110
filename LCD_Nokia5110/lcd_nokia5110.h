/*
* lcd_nokia5110.h
*
* Created: 23.09.2018
* Author: Alexander Velilyaev
* Contact: Alexandervelilyaev@gmail.com
*
* This library implements only the most necessary functions for comfortable work with this display
*/


#ifndef LCD_NOKIA5110_H_
#define LCD_NOKIA5110_H_
#define F_CPU 8000000UL //8Mhz - frequency of CPU (Change this value if frequency of your CPU is different)

#define PORTx PORTB
#define DDRx DDRB

#include <avr/io.h>

//Clear buffer
void ClearBuffer();

//Set port and pin numbers, initialize lcd
void InitLCD(uint8_t rst, uint8_t sce, uint8_t dc, uint8_t din, uint8_t clk);

//Set pixel to buffer
void SetPixel(uint8_t x, uint8_t y, uint8_t value);

//Flush buffer data(set data from buffer to DDRAM)
void Flush();

#endif /* LCD_NOKIA5110_H_ */