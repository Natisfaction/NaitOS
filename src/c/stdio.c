#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#include "stdio.h"
#include "in_asm.h"

#define BLACK         0x0
#define BLUE          0x1
#define GREEN         0x2
#define CYAN          0x3
#define RED           0x4
#define PURPLE        0x5
#define BROWN         0x6
#define GREY          0x7
#define DARK_GREY     0x8
#define LIGHT_BLUE    0x9
#define LIGHT_GREEN   0xA
#define LIGHT_CYAN    0xB
#define LIGHT_RED     0xC
#define LIGHT_PURPLE  0xD
#define YELLOW        0xE
#define WHITE         0xF

#define WIDTH         80
#define HEIGHT        25

#define BIN           2
#define OCT           8
#define DEC           10
#define HEX           16

int DEFAULT_COLOR  =  0x1F;
int x = 0,  y = 0;

//Cursore

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end){
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);

    return;
}

void disable_cursor(){
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);

    return;
}

void update_cursor(int x, int y){
    uint16_t pos = y * WIDTH + x;
    update_cursor_full(pos);

    return;
}

void update_cursor_full(uint16_t pos){
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));

    return;
}

uint16_t get_cursor_position(){
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;

	return pos;
}

//Carattere

void putc(char c){
    char* VGA = (char*)0xB8000;
    switch(c){
        case '\n':
            x = 0;
            y++;
            break;
        
        case '\t':
            for (size_t i = 0; i < 3; i++){
                VGA[2* (y * WIDTH + x)] = ' ';
                VGA[2* (y * WIDTH + x) + 1] = DEFAULT_COLOR;
                x++;
            }
            break;

        case '\r':
            x = 0;
            y = 0;
            break;
        
        default:
            VGA[2* (y * WIDTH + x)] = c;
            VGA[2* (y * WIDTH + x) + 1] = DEFAULT_COLOR;
            x++;
            break;
        }
    if (x == WIDTH){
        x = 0;
        y++;
    }
    update_cursor(x,y);

    return;
}

void cls(int foreg, int backg){
    DEFAULT_COLOR = (backg * 16) + foreg;
    x = 0;
    y = 0;
    for (size_t j = 0; j < WIDTH; j++){
        for (size_t k = 0; k < HEIGHT; k++){
            putc(' ');
        }
    }
    x = 0;
    y = 0;
    update_cursor(x,y);
    
    return;
}

void puts(const char* str){
    while(*str){
        putc(*str);
        str++;
    }

    return;
}

const char Numeri[] = "0123456789ABCDE";

void print_unsigned(int u_num, int base){
    char buffer[32];
    int cambio = 0;

    do{
        int risultato = u_num % base;
        u_num /= base;
        buffer[cambio++] = Numeri[risultato];
    } while(u_num > 0);

    while(--cambio >= 0){
        putc(buffer[cambio]);
    }

    return;
}

void print_signed(int s_num, int base){
    if(s_num < 0){
        putc('-');
        s_num *= -1;
        print_unsigned(s_num,base);
    } else if(s_num > 0){
        putc('+');
        print_unsigned(s_num,base);
    } else {
        putc('0');
    }

    return;
}

void printf(const char* fmt, int foreg, int backg, ...){
    DEFAULT_COLOR = (backg * 16) + foreg;
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

            //x/X/p Stampano un numero in base 16 (anche i puntatori lavorano in esadecimale)
            case 'p':
            case 'X':
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

//Main screen

void DarkScreenInit(){
    cls(BLUE,LIGHT_BLUE);
    DEFAULT_COLOR = 0x1F;
    printf("                                     NaitOS                                     ",LIGHT_BLUE,BLUE);
    x = 0, y = 24;
    printf("                                 Main OS Screen                                 ",LIGHT_BLUE,BLUE);
    x = 0, y = 1;
    update_cursor(x,y);

    return;
}

void LightScreenInit(){
    cls(LIGHT_GREEN,YELLOW);
    printf("                                     NaitOS                                     ",YELLOW,LIGHT_GREEN);
    x = 0, y = 24;
    printf("                                 Main OS Screen                                 ",YELLOW,LIGHT_GREEN);
    x = 0, y = 1;
    update_cursor(x,y);

    return;
}