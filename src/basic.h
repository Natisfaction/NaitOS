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

//Inserisce un carattere ad una posizione specifica

void putchar(int x, int y, char c){
    uint8_t* Screen = (uint8_t*)0xB8000;
    Screen[2 * (y * WIDTH + x)] = c;            //Il carattere sta nella posizione pari
    return;
}

//Inserisce un colore ad una posizione specifica

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

//Funzione di print per le stringhe

void prints(const char* str){
    int x = 0, y = 0;
    while(*str != 0){
        putc(*str,x,y);
        x++;
        if (x == WIDTH){
            x = 0;
            y++;
        }
        str++;
    }
    return;
}

//Funzione prinf (print full)

// #define PRINTF_STATE_NORMAL         0
// #define PRINTF_STATE_LENGTH         1
// #define PRINTF_STATE_LENGTH_SHORT   2
// #define PRINTF_STATE_LENGTH_LONG    3
// #define PRINTF_STATE_SPEC           4

// #define PRINTF_LENGTH_DEFAULT       0
// #define PRINTF_LENGTH_SHORT_SHORT   1
// #define PRINTF_LENGTH_SHORT         2
// #define PRINTF_LENGTH_LONG          3
// #define PRINTF_LENGTH_LONG_LONG     4

// void printf(const char* fmt, ...)
// {
//     va_list args;
//     va_start(args, fmt);

//     int state = PRINTF_STATE_NORMAL;
//     int length = PRINTF_LENGTH_DEFAULT;
//     int radix = 10;
//     bool sign = false;
//     bool number = false;

//     while (*fmt)
//     {
//         switch (state)
//         {
//             case PRINTF_STATE_NORMAL:
//                 switch (*fmt)
//                 {
//                     case '%':   state = PRINTF_STATE_LENGTH;
//                                 break;
//                     default:    putc(*fmt);
//                                 break;
//                 }
//                 break;

//             case PRINTF_STATE_LENGTH:
//                 switch (*fmt)
//                 {
//                     case 'h':   length = PRINTF_LENGTH_SHORT;
//                                 state = PRINTF_STATE_LENGTH_SHORT;
//                                 break;
//                     case 'l':   length = PRINTF_LENGTH_LONG;
//                                 state = PRINTF_STATE_LENGTH_LONG;
//                                 break;
//                     default:    goto PRINTF_STATE_SPEC_;
//                 }
//                 break;

//             case PRINTF_STATE_LENGTH_SHORT:
//                 if (*fmt == 'h')
//                 {
//                     length = PRINTF_LENGTH_SHORT_SHORT;
//                     state = PRINTF_STATE_SPEC;
//                 }
//                 else goto PRINTF_STATE_SPEC_;
//                 break;

//             case PRINTF_STATE_LENGTH_LONG:
//                 if (*fmt == 'l')
//                 {
//                     length = PRINTF_LENGTH_LONG_LONG;
//                     state = PRINTF_STATE_SPEC;
//                 }
//                 else goto PRINTF_STATE_SPEC_;
//                 break;

//             case PRINTF_STATE_SPEC:
//             PRINTF_STATE_SPEC_:
//                 switch (*fmt)
//                 {
//                     case 'c':   putc((char)va_arg(args, int));
//                                 break;

//                     case 's':   
//                                 puts(va_arg(args, const char*));
//                                 break;

//                     case '%':   putc('%');
//                                 break;

//                     case 'd':
//                     case 'i':   radix = 10; sign = true; number = true;
//                                 break;

//                     case 'u':   radix = 10; sign = false; number = true;
//                                 break;

//                     case 'X':
//                     case 'x':
//                     case 'p':   radix = 16; sign = false; number = true;
//                                 break;

//                     case 'o':   radix = 8; sign = false; number = true;
//                                 break;

//                     // ignore invalid spec
//                     default:    break;
//                 }

//                 if (number)
//                 {
//                     if (sign)
//                     {
//                         switch (length)
//                         {
//                         case PRINTF_LENGTH_SHORT_SHORT:
//                         case PRINTF_LENGTH_SHORT:
//                         case PRINTF_LENGTH_DEFAULT:     printf_signed(va_arg(args, int), radix);
//                                                         break;

//                         case PRINTF_LENGTH_LONG:        printf_signed(va_arg(args, long), radix);
//                                                         break;

//                         case PRINTF_LENGTH_LONG_LONG:   printf_signed(va_arg(args, long long), radix);
//                                                         break;
//                         }
//                     }
//                     else
//                     {
//                         switch (length)
//                         {
//                         case PRINTF_LENGTH_SHORT_SHORT:
//                         case PRINTF_LENGTH_SHORT:
//                         case PRINTF_LENGTH_DEFAULT:     printf_unsigned(va_arg(args, unsigned int), radix);
//                                                         break;
                                                        
//                         case PRINTF_LENGTH_LONG:        printf_unsigned(va_arg(args, unsigned  long), radix);
//                                                         break;

//                         case PRINTF_LENGTH_LONG_LONG:   printf_unsigned(va_arg(args, unsigned  long long), radix);
//                                                         break;
//                         }
//                     }
//                 }

//                 // reset state
//                 state = PRINTF_STATE_NORMAL;
//                 length = PRINTF_LENGTH_DEFAULT;
//                 radix = 10;
//                 sign = false;
//                 break;
//         }

//         fmt++;
//     }

//     va_end(args);
// }