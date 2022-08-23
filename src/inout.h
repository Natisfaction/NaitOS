#pragma once

#include <stdint.h>
#include <stddef.h>

#include "cursor.h"

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

//FUNZIONE PRINT (SCRIVE UNA STRINGA, CON COLORE)

void print(char *str,int foreground,int background){        //SERVE LA FRASE DA SCRIVERE E IL COLORE
    int colore = (background * 16) + foreground;            //PER OTTENERE IL COLORE DI ENTRAMBI BASTA SPOSTARE IL BACKGROUND A SINISTRA E IL FOREGROUND A DESTRA
    int xcoord = 0, ycoord = 2;
    volatile char *vidmem = (volatile char*)0xB8000+(2*(ycoord*WIDTH)+xcoord);
    while(*str != 0){                                       //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = *str++;                                  //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = colore;                                  //E ASSIEME A QUELLA ANCHE IL COLORE
        xcoord++;
        switch(x){                                          //SE LE RIGHE SONO FINITE VAI A CAPORIGA
            case WIDTH:
                xcoord = 0;
                ycoord++;
                break;
            default:
                break;
        }
    }
    set_cursor(xcoord,ycoord);
    return;
}

//FUNZIONE PER IL CLEARSCREEN

void cls(void){
    char *vidmem = (char*)0xB8000;      //PUNTATORE ALLA MEMORIA VIDEO
    for(int i=0; i<2000; i++){          //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = 0;                  //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = WHITE;              //E ASSIEME A QUELLA ANCHE IL COLORE
    }
    set_cursor(0,0);
    return;
}