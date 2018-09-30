#include "lcd_nokia5110.h"
#include <util/delay.h>

#define WIDTH 84
#define HEIGHT 48
#define SET_BIT(DST, POS, VALUE) DST = (DST & ~(1 << POS)) | (VALUE << POS)
#define GET_BIT(SRC, POS) (SRC & (1 << POS)) >> POS

uint8_t RST, SCE, DC, DIN, CLK;
uint8_t buffer[WIDTH * 6];

//Transmit byte by SPI
void SendByte(uint8_t byte, uint8_t dc)
{
	SET_BIT(PORTx, SCE, 0);
	SET_BIT(PORTx, DC, dc);

	for (uint8_t p = 8; p > 0; p--)
	{
		SET_BIT(PORTx, DIN, GET_BIT(byte, (p-1)));
		SET_BIT(PORTx, CLK, 0);
		SET_BIT(PORTx, CLK, 1);
	}

	SET_BIT(PORTx, SCE, 1);
}

//Make command
void Command(uint8_t cmd)
{
	SendByte(cmd, 0);
}

//Send data to DDRAM
void Data(uint8_t data)
{
	SendByte(data, 1);
}

//PD - (0: chip is active, 1: chip is Power-down mode)
//V - (0: horizontal addressing, 1: vertical addressing)
//H - (0: basic instruction set, 1: extended instruction set)
void FunctionSet(uint8_t PD, uint8_t V, uint8_t H)
{
	Command(0b00100000 | PD << 2 | V << 1 | H << 0);
}

//BASIC INSTRUCTION SET:--------------------------------------------------------------------------------
//D and E - (00: display blank, 10: normal mode, 01: all display segments on, 11: inverse video mode)
void DisplayControl(uint8_t D, uint8_t E)
{
	Command(0b00001000 | D << 2 | E << 0);
}

//Sets Y-address of RAM, 0 <= Y <= 5
void SetRAMAddressY(uint8_t Y2, uint8_t Y1, uint8_t Y0)
{
	Command(0b01000000 | Y2 << 2 | Y1 << 1 | Y0 << 0);
}

//Sets X-address part of RAM, 0 <= X <= 83
void SetRAMAddressX(uint8_t X6, uint8_t X5, uint8_t X4, uint8_t X3, uint8_t X2, uint8_t X1, uint8_t X0)
{
	Command(0b10000000 | X6 << 6 | X5 << 5 | X4 << 4 | X3 << 3 | X2 << 2 | X1 << 1 | X0 << 0);
}
//------------------------------------------------------------------------------------------------------

//EXTENDED INSTRUCTION SET:-----------------------------------------------------------------------------
//TC1 and TC0 - (00: Vlcd temp. coef. 0, 01: Vlcd temp. coef. 1, 10: Vlcd temp. coef. 2, 11: Vlcd temp. coef. 3)
void TemperatureControl(uint8_t TC1, uint8_t TC0)
{
	Command(0b00000100 | TC1 << 1 | TC0 << 0);
}

//Set Bias Sestem(BSx)
void BiasSystem(uint8_t BS2, uint8_t BS1, uint8_t BS0)
{
	Command(0b00010000 | BS2 << 2 | BS1 << 1 | BS0 << 0);
}

//Write Vop to register
void SetVop(uint8_t OP6, uint8_t OP5, uint8_t OP4, uint8_t OP3, uint8_t OP2, uint8_t OP1, uint8_t OP0)
{
	Command(0b10000000 | OP6 << 6 | OP5 << 5 | OP4 << 4 | OP3 << 3 | OP2 << 2 | OP1 << 1 | OP0 << 0);
}
//------------------------------------------------------------------------------------------------------

void Reset()
{
	SET_BIT(PORTx, RST, 1);
	_delay_ms(10);
	SET_BIT(PORTx, RST, 0);
	_delay_ms(10);
	SET_BIT(PORTx, RST, 1);
}

//Clear buffer
void ClearBuffer()
{
	for (int i = 0; i < WIDTH * 6; i++)
	{
		buffer[i] = 0;
	}
}

//Set port and pin numbers, initialize lcd
void InitLCD(uint8_t rst, uint8_t sce, uint8_t dc, uint8_t din, uint8_t clk)
{
	RST = rst;
	SCE = sce;
	DC = dc;
	DIN = din;
	CLK = clk;
	
	SET_BIT(DDRx, rst, 1);
	SET_BIT(DDRx, sce, 1);
	SET_BIT(DDRx, dc, 1);
	SET_BIT(DDRx, din, 1);
	SET_BIT(DDRx, clk, 1);

	Reset();
	FunctionSet(0, 0, 1);//switch to extended instruction set
	SetVop(1, 0, 0, 0, 0, 1, 0);//Vop = 7V
	TemperatureControl(0, 1);//set typical temperature coefficient
	BiasSystem(0, 1, 1);//set optimal bias(1:48)
	ClearBuffer();//clear buffer
	FunctionSet(0, 0, 0);//switch to normal instruction set
	DisplayControl(1, 0);//set normal mode
	Flush();
}

//Set pixel to buffer
//x, y: coordinates(point {0,0} is on the top left), 
//value: 0 - clear point, 1 - set point
void SetPixel(uint8_t x, uint8_t y, uint8_t value)
{
	SET_BIT(buffer[WIDTH * (y >> 3) + x], (y % 8), value);
}

//Flush buffer data(set data from buffer to DDRAM)
void Flush()
{
	SetRAMAddressX(0,0,0,0,0,0,0);
	SetRAMAddressY(0,0,0);
	for (int i = 0; i < WIDTH * 6; i++)
	{
		Data(buffer[i]);
	}
}