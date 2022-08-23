#pragma once

#include <stdint.h>
#include <stddef.h>

#include "cursor.h"

//COLORI (ESADECIMALE)

#define BLACK        0x0
#define BLUE         0x1
#define GREEN        0x2
#define CYAN         0x3
#define RED          0x4
#define PURPLE       0x5
#define BROWN        0x6
#define GREY         0x7
#define DARK_GREY    0x8
#define LIGHT_BLUE   0x9
#define LIGHT_GREEN  0xA
#define LIGHT_CYAN   0xB
#define LIGHT_RED    0xC
#define LIGHT_PURPLE 0xD
#define YELLOW       0xE
#define WHITE        0xF

//FUNZIONE PER OTTENERE LA POSIZIONE DEL CURSORE DALLA X E Y

int genposition(int xc,int yc){
    int coord = (2*(yc*WIDTH)+xc);
    return coord;
}

//FUNZIONE PRINT (SCRIVE UNA STRINGA, CON COLORE)

int print(char *str,int foreground,int background,int coloumn,int row, ...){
    int colore = (background * 16) + foreground;                //PER OTTENERE IL COLORE DI ENTRAMBI BASTA SPOSTARE IL BACKGROUND A SINISTRA E IL FOREGROUND A DESTRA
    int position = (2*(row*WIDTH)+coloumn);
    volatile char *vidmem = (volatile char*)0xB8000+position;
    while(*str != 0){                                           //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = *str++;                                     //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = colore;                                     //E ASSIEME A QUELLA ANCHE IL COLORE
        coloumn++;
        switch(x){                                              //SE LE RIGHE SONO FINITE VAI A CAPORIGA
            case WIDTH:
                coloumn = 0;
                row++;
                break;
            default:
                break;
        }
    }
    set_cursor(coloumn,row);                                    //E PORTA IL CURSORE DOVE IL TESTO E' TERMINATO
    return position;                                            //STILL WORKING...
}

//FUNZIONE PER IL CLEARSCREEN

void cls(int fore,int back){
    char *vidmem = (char*)0xB8000;      //PUNTATORE ALLA MEMORIA VIDEO
    int color = (back * 16) + fore;
    for(int i=0; i<2000; i++){
        *vidmem++ = 0;
        *vidmem++ = color;
    }
    set_cursor(0,0);
    return;
}

//FUNZIONE PER LA DARK MODE

void Dark(void){
    cls(BLUE,LIGHT_BLUE);
    print("                               NaitOS - Dark Mode                               ",CYAN,BLUE,0,0);
    print("                                 Main OS screen                                 ",CYAN,BLUE,0,24);
    set_cursor(0,1);
    return;
}