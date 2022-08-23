#pragma once

#include <stdint.h>
#include <stddef.h>
#include "header/basic.h"
#include "header/cursor.h"

//FUNZIONE PRINT (SCRIVE UNA STRINGA, CON COLORE)

void print(char *str,int foreground,int background){        //SERVE LA FRASE DA SCRIVERE E IL COLORE
    int colore = background * 16 + foreground;              //PER OTTENERE IL COLORE DI ENTRAMBI BASTA SPOSTARE IL BACKGROUND A SINISTRA E IL FOREGROUND A DESTRA
    int x = 0, y = 0;
    char *vidmem = (char*)0xB8000;                          //PUNTATORE ALLA MEMORIA VIDEO
    while(*str != 0){                                       //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = *str++;                                 //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = colore;                                 //E ASSIEME A QUELLA ANCHE IL COLORE
        x++;
        switch(x){                                          //SE LE RIGHE SONO FINITE VAI A CAPORIGA
            case 80:
                x = 0;
                y++;
                break;
            default:
                break;
        }
    }
    set_cursor(x,y);
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