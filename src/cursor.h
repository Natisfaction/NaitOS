#pragma once

#include <stddef.h>
#include "asmport.h"

//SCHERMO E CURSORE (DECIMALE)

const unsigned CWIDTH   =   80;
const unsigned CHEIGHT  =   25;

//Definisco le funzioni

//FUNZIONI IN C PER IL CURSORE

void enable_cursor(int first, int least)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | first);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | least);
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void update_cursor(int& ScreenX, int& ScreenY)
{
	uint16_t pos = ScreenX * CWIDTH + ScreenY;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));

	enable_cursor(15,15);
}