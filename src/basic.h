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

//Inserisce un carattere ad una posizione specifica

void putchar(int x, int y, char c){
    uint8_t* g_ScreenBuffer = (uint8_t*)0xB8000;
    g_ScreenBuffer[2 * (y * WIDTH + x)] = c;            //Il carattere sta nella posizione pari
}

//Inserisce un colore ad una posizione specifica

void putcolor(int x, int y, uint8_t color){
    uint8_t* g_ScreenBuffer = (uint8_t*)0xB8000;
    g_ScreenBuffer[2 * (y * WIDTH + x) + 1] = color;    //Il colore sta nella posizione dispari
}

//Clearscreen

void cls(){
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            putchar(x, y, '\0');
            putcolor(x, y, 0xFA);
        }
}

//Funzione printn (scrive i numeri)

void printn(int num, int foreg, int backg){
    int x = 0, y = 0;
    int col = (backg * 16) + foreg;
    char number;
    int divisore = 1000000000, fnum = 0, exit = 0, risultato;   //Max 1 miliardo, altrimenti overflow
    if(num == 0){
        number = '0';
        putchar(x,y,number);
        putcolor(x,y,col);
        exit = 1;
    } else if(num < 0){
        putchar(x,y,'-');
        putcolor(x,y,col);
        num *= -1;
        x++;
    }
    while(divisore >= 10 && exit == 0){
        risultato = num / divisore;
        if (risultato == 0 && fnum == 0){
            divisore = divisore / 10;
        } else {
            number = risultato + '0';
            putchar(x,y,number);
            putcolor(x,y,col);
            num %= divisore;
            divisore /= 10;
            fnum = 1;
            x++;
            if (x == WIDTH){
                x = 0;
                y++;
            }
            if (divisore == 1){
                number = num + '0';
                putchar(x,y,number);
                putcolor(x,y,col);
            }
        }
    }
    return;
}

//Funzione prinf (print full)

void printf(){
    return;
}