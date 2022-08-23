#pragma once

#include <stdint.h>
#include <stddef.h>

//COLORI (ESADECIMALE)

const int BLACK         =   0x0;
const int BLUE          =   0x1;
const int GREEN         =   0x2;
const int CYAN          =   0x3;
const int RED           =   0x4;
const int PURPLE        =   0x5;
const int BROWN         =   0x6;
const int GREY          =   0x7;
const int DARK_GREY     =   0x8;
const int LIGHT_BLUE    =   0x9;
const int LIGHT_GREEN   =   0xA;
const int LIGHT_CYAN    =   0xB;
const int LIGHT_RED     =   0xC;
const int LIGHT_PURPLE  =   0xD;
const int YELLOW        =   0xE;
const int WHITE         =   0xF;

//VERO FALSO (DECIMALE)

const int FALSE         =   0;
const int TRUE          =   1;

//SCHERMO E CURSORE (DECIMALE)

const int WIDTH         =   80;
const int HEIGHT        =   25;

int ScreenX;
int ScreenY;

static char *vidmem     =   (char*)0xB8000;

static uint16_t x       =   0;
static uint16_t y       =   0;