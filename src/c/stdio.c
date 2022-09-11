#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../header/stdio.h"
#include "../header/in_asm.h"
#include "../header/string.h"

int DEFAULT_COLOR  =  0x1F;
int x = 0,  y = 0;
int status;

const char* ready = "Ready! > ";
const char* NaitOS_v = "NaitOS Version 0.1";

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

    enable_cursor(15,15);

    return;
}

void Up(){
    if (y != 1){
        y--;
    }
    update_cursor(x,y);

    return;
}

void Down(){
    if (y != 23){
        y++;
    }
    update_cursor(x,y);

    return;
}

void Dx(){
    if (!(x == 79 && y == 23)){
        x++;
    }
    update_cursor(x,y);

    return;
}

void Sx(){
    if (!(x == 0 && y == 1)){
        x--;
    }
    update_cursor(x,y);

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

char getc(int x, int y){
    volatile char* VGA = (volatile char*)0xB8000;
    
    return VGA[2* (y * WIDTH + x)];
}

int getcol(int x, int y){
    volatile char* VGA = (volatile char*)0xB8000;
    
    return VGA[2* (y * WIDTH + x) + 1];
}

void putc(char c){
    volatile char* VGA = (volatile char*)0xB8000;
    switch(c){
        case '\n':
            x = 0;
            Down();
            break;
        
        case '\t':
            for (size_t i = 0; i < 3; i++){
                VGA[2* (y * WIDTH + x)] = ' ';
                VGA[2* (y * WIDTH + x) + 1] = DEFAULT_COLOR;
                Dx();
            }
            break;

        case '\r':
            x = 0;
            y = 1;
            break;
        
        case '\b':
            if (!(status == 0 && x == 9)){
                Sx();
                VGA[2* (y * WIDTH + x)] = ' ';
                VGA[2* (y * WIDTH + x) + 1] = DEFAULT_COLOR;
            }
            break;

        default:
            VGA[2* (y * WIDTH + x)] = c;
            VGA[2* (y * WIDTH + x) + 1] = DEFAULT_COLOR;
            Dx();
            break;
        }
    if (x == WIDTH){
        x = 0;
        Down();
    }
    update_cursor(x,y);

    return;
}

void cls(){
    x = 0;
    y = 0;
    for (size_t j = 0; j < WIDTH; j++){
        for (size_t k = 0; k < HEIGHT; k++){
            putc('\0');
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

int character;
const char *Command[] = {"help","version","calc"};
char *Usercmd;

int input(){
    int gotten = getc(x,y);
    if (gotten != '\0'){
        printf("\b");
        return gotten;
    } else {
        return 0;
    }
}

void DarkScreenInit(){
    status = CMDMODE;
    DEFAULT_COLOR = 0x91;
    cls();
    DEFAULT_COLOR = 0x19;
    printf(" \t\t\t\t\t\t\t\t\t\t\t\tNaitOS\t\t\t\t\t\t\t\t\t\t\t\t ");
    x = 0, y = 24;
    printf("  \t\t\t\t\t\t\t\t\t\t\tCMD - Mode\t\t\t\t\t\t\t\t\t\t\t  ");
    x = 0, y = 1;
    DEFAULT_COLOR = 0x91;
    printf("%s",ready);
    while (true){
        int i = 0;
        *Usercmd = "";
        while (true){
            character = input();
            if (character == '\0'){
                continue;
            } else if (character == '.'){
                printf(".\n");
                break;
            } else {
                printf("%c",(char)character);
                Usercmd[i] = character;
                i++;
            }
        }
        if (strcmps(*Usercmd,*Command[0]) == 0){
            printf("\thelp: displays a list of commands\n\tversion: displays the OS version\n\tcalc: opens a calculator\n%s",ready);
        } else if (strcmps(*Usercmd,*Command[1]) == 0){
            printf("\tVersion: %s\n%s",NaitOS_v,ready);
        } else if (strcmps(*Usercmd,*Command[2]) == 0){
            printf("\tCalculator\n%s",ready);
        } else {
            printf("\tCommand not recognized\n%s",ready);
        }
    }
    return;
}

void LightScreenInit(){
    status = CMDMODE;
    DEFAULT_COLOR = 0xEA;
    cls();
    DEFAULT_COLOR = 0xAE;
    printf(" \t\t\t\t\t\t\t\t\t\t\t\tNaitOS\t\t\t\t\t\t\t\t\t\t\t\t ");
    x = 0, y = 24;
    printf("  \t\t\t\t\t\t\t\t\t\t\tCMD - Mode\t\t\t\t\t\t\t\t\t\t\t  ");
    x = 0, y = 1;
    DEFAULT_COLOR = 0xEA;
    printf("%s",ready);

    return;
}

//Schermata di errore, NON DA UTILIZZARE COME FUNZIONE DIRETTA

extern void ErrorScreenInit(){
    DEFAULT_COLOR = 0xC4;
    cls();
    DEFAULT_COLOR = 0x4C;
    printf(" \t\t\t\t\t\t\t\t\t\t\t\tNaitOS\t\t\t\t\t\t\t\t\t\t\t\t ");
    x = 0, y = 10;
    DEFAULT_COLOR = 0xC4;
    printf("\t\t  / \\\t\t  NaitOS: There was an unexpected error and the OS crashed!!!\n\t\t / | \\\t\t Try to restart the computer to fix this issue\n\t\t/  .  \\");
    DEFAULT_COLOR = 0x4C;
    x = 0, y = 24;
    printf("\t\t\t\t\t\t\tOS Error Screen (maybe this isn't good)  \t\t\t\t\t\t");
    DEFAULT_COLOR = 0xC4;
    x = 25, y = 13;
    DEFAULT_COLOR = 0x4C;
    printf("ERROR CODE: ");

    return;
}