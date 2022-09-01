#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

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

#define DEFAULT      (BLUE * 16) + WHITE

//VERO FALSO (DECIMALE)

#define FALSE        0
#define TRUE         1

//DIMENSIONI DELLO SCHERMO

#define WIDTH        80
#define HEIGHT       25

int ScreenX = 0, ScreenY = 0;
char* VGA = (char*)0xB8000;

//Dichiarazione delle funzioni

void putc(char c);
void puts(const char* str);
void printf(const char* fmt, ...);

//Creazione delle funzioni

void putc(char c){
    VGA[2* (ScreenY * WIDTH + ScreenX)] = c;
    VGA[2* (ScreenY * WIDTH + ScreenX) + 1] = DEFAULT;
    ScreenX++;
    if (ScreenX == WIDTH){
        ScreenX = 0;
        ScreenY++;
    }
    return;
}

void puts(const char* str){
    while (*str){
        switch (*str){
        case '\n':
            ScreenX = 0;
            ScreenY++;
            break;
        
        case '\t':
            for (int i = 0; i < 3; i++){
                putc(' ');
            }
            break;

        case '\r':
            ScreenX = 0;
            ScreenY = 0;
            break;

        default:
            putc(*str);
            break;
        }
        *str++;
    }
    return;
}

void printn(int num)
{
    char number;
    int divisore = 1000000000, fnum = 0, exit = 0, risultato;   //Max 1 miliardo, altrimenti overflow
    if(num == 0){           //Controlla se il numero è 0
        putc('0');          //Allora in questo caso scrivi zero, ed esci
        exit = 1;
    } else if(num < 0){     //Oppure se è negativo
        putc('-');          //Metti un '-' davanti, trasforma il numero in positivo, e stampalo
        num *= -1;
    }
    while(divisore >= 1 && exit == 0){
        risultato = num / divisore;
        if(risultato == 0 && fnum == 0){
            divisore = divisore / 10;
        } else {
            number = risultato + '0';
            putc(number);
            num %= divisore;
            divisore /= 10;
            fnum = 1;
        }
    }
    return;
}

#define START  0
#define INIT   1

void printf(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);
    
    while (*fmt){
        switch (*fmt){
        //Se lo trovo, mi preparo a riconoscere cosa dare in output
        case '%':
            fmt++;
            switch (*fmt){
            //i/d Stampano entrambi un int
            case 'i':
            case 'd':   printn(va_arg(args, int));
                        fmt++;
                        break;
            //s Stampa una string
            case 's':   puts(va_arg(args, const char*));
                        fmt++;
                        break;
            //c Stampa un char
            case 'c':   putc((char)va_arg(args, int));
                        fmt++;
                        break;
            //Ignora in caso di carattere non consentito
            default:    fmt++;
                        break;
            }
        //Altrimenti resetto lo stato, e scrivo il carattere
        default:    putc(*fmt);
                    break;
        }
        fmt++;
    }
    va_end(args);
    return;
}