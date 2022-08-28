#pragma once

#include <stdint.h>
#include <stddef.h>

#include "asmport.h"

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

//DIMENSIONI DELLO SCHERMO

const unsigned WIDTH    =   80;
const unsigned HEIGHT   =   25;
int ScreenX             =   80;
int ScreenY             =   25;

// ======================== FUNZIONI ========================

void print(const char* str, int foreg, int backg){
    int color = (backg * 16) + foreg;
    volatile char *VGA = (volatile char*)0xB8000;
    while(*str){
        *VGA++ = *str++;
        *VGA++ = color;
    }
    return;
}

void cls(){
    volatile char *VGA = (volatile char*)0xB8000;
    for (int i=0; i<HEIGHT; i++){
        for (int j=0; i<WIDTH; i++){
            *VGA++ = ' ';
            *VGA++ = 0xC4;
        }
    }
    return;
}