#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

//Colori

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

//Bool

#define FALSE        0
#define TRUE         1

//Schermo

#define WIDTH        80
#define HEIGHT       25

int ScreenX = 0, ScreenY = 0;
char* VGA = (char*)0xB8000;

//Numeri

#define BIN         2
#define OCT         8
#define DEC         10
#define HEX         16

//Dichiarazione delle funzioni

void putc(char c);
void puts(const char* str);
void printf(const char* fmt, ...);
void print_signed(int s_num, int base);
void print_unsigned(int u_num, int base);

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

void print_signed(int s_num, int base){
    if(s_num < 0){     //Oppure se è negativo
        putc('-');     //Metti un '-' davanti, trasforma il numero in positivo, e stampalo
        s_num *= -1;
    }
    print_unsigned(s_num,base);
    return;
}

const char Numeri[] = "0123456789ABCDE";

void print_unsigned(int u_num, int base){
    char buffer[32];
    int cambio = 0;
    //Calcola ogni cifra del numero, e controlla se bisogna mettere una lettera (esadecimale)
    do{
        int risultato = u_num % base;
        u_num /= base;
        buffer[cambio++] = Numeri[risultato];
    }while (u_num > 0);
    //Ma i numeri messi nel buffer sono al contrario, quindi stampali dritti
    while(--cambio >= 0){
        putc(buffer[cambio]);
    }
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
            //i/d Stampano entrambi un numero con segno (nel caso fosse negativo)
            case 'i':
            case 'd':   print_signed(va_arg(args, int),DEC);
                        break;
            //u Stampa un numero senza segno
            case 'u':   print_unsigned(va_arg(args, int),DEC);
                        break;
            //s Stampa una string
            case 's':   puts(va_arg(args, const char*));
                        break;
            //c Stampa un char
            case 'c':   putc((char)va_arg(args, int));
                        break;
            //o Stampa un numero in base 8
            case 'o':   print_unsigned(va_arg(args, int),OCT);
                        break;
            //x Stampa un numero in base 16
            case 'x':   print_unsigned(va_arg(args, int),HEX);
                        break;
            //% Stampa '%'
            case '%':   putc('%');
                        break;
            //Ignora in caso di carattere non consentito
            default:    break;
            }
            break;
        //Altrimenti resetto lo stato, e scrivo il carattere
        default:    putc(*fmt);
                    break;
        }
        fmt++;
    }
    va_end(args);
    return;
}