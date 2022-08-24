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

//PER RIGHE E COLONNE (DOPO L'IMPLEMENTAZIONE DELLA SCHERMATA)

#define DEFAULT      0
#define FIRST_ROW    1
#define MID_UP_ROW   11
#define MID_DOWN_ROW 12
#define LAST_ROW     23

//FUNZIONE PRINT (SCRIVE UNA STRINGA, CON COLORE, CON RIGA E COLONNA DI PARTENZA)

#define VGA (unsigned char*)0xB8000

void print(char *str,int foreground,int background,int coloumn,int row){
    int colore = (background * 16) + foreground;                //PER OTTENERE IL COLORE DI ENTRAMBI BASTA SPOSTARE IL BACKGROUND A SINISTRA E IL FOREGROUND A DESTRA
    unsigned char *vidmem = VGA+(2*(row*WIDTH)+coloumn);
    while(*str != 0){                                           //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = *str++;                                     //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = colore;                                     //E ASSIEME A QUELLA ANCHE IL COLORE
        coloumn++;
        if(coloumn == WIDTH){
            coloumn = 0;
            row++;
        }
    }
    set_cursor(coloumn,row);                                    //E PORTA IL CURSORE DOVE IL TESTO E' TERMINATO
    return;                                                     //STILL WORKING...
}

//FUNZIONE PRINTF (PER LA GUI RISPETTANDO I COLORI)

//FUNZIONE PER IL CLEARSCREEN

void cls(int fore,int back){
    unsigned char *vidmem = VGA;                       //PUNTATORE ALLA MEMORIA VIDEO
    int color = (back * 16) + fore;
    for(int i=0; i<1920; i++){
        *vidmem++ = 0;
        *vidmem++ = color;
    }
    return;
}