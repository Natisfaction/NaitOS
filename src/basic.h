#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#include "asmport.h"

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

//Funzione printn (scrive i numeri)

int ScreenX = 0, ScreenY = 0;
char* VGA = (char*)0xB8000;

void putc(char c, int col){
    VGA[2* (ScreenY * WIDTH + ScreenX)] = c;
    VGA[2* (ScreenY * WIDTH + ScreenX) + 1] = DEFAULT;
    ScreenX++;
    return;
}
// void printn(int num)
// {
//     static int x = 0, y = 0;
//     static char number;
//     static int divisore = 1000000000, fnum = 0, exit = 0, risultato;   //Max 1 miliardo, altrimenti overflow
//     if(num == 0){                                               //Controlla se il numero è 0
//         putc('0',x,y);
//         exit = 1;
//     } else if( num < 0 ){                                         //Oppure se è negativo
//         putc('-',x,y);
//         num *= -1;
//         x++;
//     }
//     while(divisore >= 10 && exit == 0)
//     {
//         risultato = num / divisore;
//         if (risultato == 0 && fnum == 0)
//         {
//             divisore = divisore / 10;
//         } else 
//         {
//             number = risultato + '0';
//             putc(number,x,y);
//             num %= divisore;
//             divisore /= 10;
//             fnum = 1;
//             x++;
//             if (x == WIDTH)
//             {
//                 x = 0;
//                 y++;
//             }
//             if (divisore == 1)
//             {
//                 number = num + '0';
//                 putc(number,x,y);
//             }
//         }
//     }
//     return;
// }

// void printf(const char* fmt, ...)
// {
//     va_list args;
//     va_start(args, fmt);

//     static int x = 0, y = 0;
    
//     //Spazio futuro per cose
    
//     va_end(args);
//     return;
// }