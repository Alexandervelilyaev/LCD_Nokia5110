![lm016l Banner](https://4u9ej3n0081av6bpl3yb4qtl-wpengine.netdna-ssl.com/wp-content/uploads/2014/09/nokia5110_fronta.jpg)
# AVR library for graphic display Nokia 5110(based on PCD8544)
This library was developed and tested for microcontroller Atmega328p. But I'm sure that it will work for others microcontrollers of the AVR family. Library implements most necessary functions(according to datasheet).

## Example
```c
#define F_CPU 8000000UL
#include <avr/io.h>
#include "lcd_nokia5110.h"

int main(void)
{
	InitLCD(1, 2, 3, 4, 5);//In this case, the pinout is the following: (RST - 1, SCE - 2, DC - 3, DIN - 4, CLK - 5)
	//LCD connected to PORT, that defined in header file "lcd_nokia5110.h", identifier "PORTx"

	//Drawing something similar to the "Little Bear"
	SetPixel(15,5,1);
	SetPixel(10,15,1);
	SetPixel(30,25,1);
	SetPixel(40,15,1);
	SetPixel(55,17,1);
	SetPixel(70,15,1);
	SetPixel(83,25,1);

	Flush();

	while (1)
	{
	}
}
```
