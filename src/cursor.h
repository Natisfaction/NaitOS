#pragma once

#include <stddef.h>
#include "asmport.h"

//SCHERMO E CURSORE (DECIMALE)

#define WIDTH  80
#define HEIGHT 25

int ScreenX;
int ScreenY;

static uint16_t x       =   0;
static uint16_t y       =   0;

//FUNZIONI IN C PER IL CURSORE

void show_cursor(){                                                 //MOSTRA IL CURSORE
    uint8_t x = 15, y = 15;

	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | x);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | y);

    return;
}

void hide_cursor(){

	outb(0x3D4, 0x0A);                                                  //NASCONDE IL CURSORE
	outb(0x3D5, 0x20);

    return;
}

void set_cursor(uint16_t x, uint16_t y){                                //SPOSTA IL CURSORE IN UNA POSIZIONE PRECISA
    uint16_t position = (y*WIDTH+x);

    outb(0x3D4, 0x0F);
    outb(0x3D5, ((uint8_t)(position & 0xFF)));

    outb(0x3D4, 0x0E);
    outb(0x3D5, ((uint8_t)((position >> 8) & 0xFF)));

    show_cursor();

    return;
}

uint16_t get_cursor_position(){                                     //OTTIENI LA POSIZIONE DEL CURSORE
    uint16_t pos = 0;

    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);

    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;

    return pos;
}