#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

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

const int DEFAULT       =   (BLUE * 16) + WHITE;

//VERO FALSO (DECIMALE)

const int FALSE         =   0;
const int TRUE          =   1;

//DIMENSIONI DELLO SCHERMO

const unsigned WIDTH    =   80;
const unsigned HEIGHT   =   25;

// ======================== FUNZIONI ========================

//Funzione putchar (inserisce un carattere in una posizione specifica)

void putchar(int x, int y, char c){
    uint8_t* Screen = (uint8_t*)0xB8000;
    Screen[2 * (y * WIDTH + x)] = c;            //Il carattere sta nella posizione pari
    return;
}

//Funzione putcolor (inserisce un colore in una posizione specifica)

void putcolor(int x, int y, uint8_t color){
    uint8_t* Screen = (uint8_t*)0xB8000;
    Screen[2 * (y * WIDTH + x) + 1] = color;    //Il colore sta nella posizione dispari
    return;
}

//Funzione putc (unisce putcolor e putchar)

void putc(char ch, int x, int y){
    putchar(x,y,ch);
    putcolor(x,y,DEFAULT);
    return;
}

//Funzione puts (scrive una stringa)

void puts(const char* string){
    int x = 0, y = 0;
    while (*string){
        if(*string == '\n'){
            x = 0;
            y++;
        } else if(*string == '\t'){
            for (int i = 0; i < 3; i++)
            {
                putc(' ',x,y);
                x++;
            }
        } else {
            putc(*string,x,y);
            x++;
        }
        *string++;
        if (x == WIDTH){
            x -= WIDTH;
            y++;
        }
    }
    return;
}

//Clearscreen

void cls(){
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            putc('\0',x,y);
        }
    }
}

//Funzione printn (scrive i numeri)

void printn(int num){
    int x = 0, y = 0;
    char number;
    int divisore = 1000000000, fnum = 0, exit = 0, risultato;   //Max 1 miliardo, altrimenti overflow
    if(num == 0){                                               //Controlla se il numero è 0
        putc('0',x,y);
        exit = 1;
    } else if(num < 0){                                         //Oppure se è negativo
        putc('-',x,y);
        num *= -1;
        x++;
    }
    while(divisore >= 10 && exit == 0){                         //E poi stampa
        risultato = num / divisore;
        if (risultato == 0 && fnum == 0){
            divisore = divisore / 10;
        } else {
            number = risultato + '0';
            putc(number,x,y);
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
                putc(number,x,y);
            }
        }
    }
    return;
}

#define START  0
#define NORMAL 1
#define NUMBER 2
#define STRING 3
#define CHAR   4

void printf(const char* fmt, ...){
    int x = 0, y = 0;

    int tipo = NORMAL;

    va_list args;
    va_start(args, fmt);
    while(*fmt){
        if(tipo == START){
            if (*fmt == '%'){
                tipo = NORMAL;
            } else if(tipo == NORMAL){
                //FINISICI DI IMPLEMENTARE DOPO
            }
        }
    }
    va_end(args);
    return;
}