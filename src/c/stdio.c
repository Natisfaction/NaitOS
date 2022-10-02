#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../header/stdio.h"
#include "../header/in_asm.h"
#include "../header/string.h"

int DEFAULT_COLOR  =  0x1F;
int x = 0,  y = 0;
bool normalprint = true;

const char* ready = "Ready! > ";
const char* NaitOS_v = "NaitOS Version 1.55";

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

uint16_t get_cursor_position(){
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;

	return pos;
}

//Inutilizzate

void Up(){
    //Farà qualcosa...

    return;
}

void Down(){
    //Farà qualcosa...

    return;
}

void Dx(){
    //Farà qualcosa...

    return;
}

void Sx(){
    //Farà qualcosa...

    return;
}

//Carattere

int getc(int x, int y){
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
            y++;
            break;
        
        case '\t':
            for (size_t i = 0; i < 3; i++)
                putc(' ');
            break;

        case '\r':
            x = 0;
            y++;
            break;
        
        case '\b':
            if (x == 0 && y != 0){
                x = WIDTH;
                y--;

                x--;
                putc('\0');
                x--;
            } else if (x == 0 && y == 0) {
                //Non fare nulla
            } else {
                x--;
                putc('\0');
                x--;
            }
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

    scroll();

    return;
}

void scroll(){

    //Salvataggio variabili

    int checky = get_cursor_position() / WIDTH;
    int checkx = get_cursor_position() % WIDTH;

    //Esegue lo scroll solo se la y non è minore di 25

    if((checky >= HEIGHT-1) && normalprint == true){

        normalprint = false;

        char buffer[WIDTH];

        y = 0, x = 0;

        for (size_t a = 0; a < WIDTH; a++)
            putc('\0');
        

        for (size_t b = 1; b != HEIGHT; b++){

            //Prendi i caratteri

            y = b, x = 0;

            for (size_t c = 0; c < WIDTH; c++){
                buffer[c] = getc(x,y);
                x++;
            }

            //Pulisci la linea

            y = b, x = 0;

            for (size_t c = 0; c < WIDTH; c++)
                putc('\0');

            //Scrivi alla y precedente il buffer

            y = b-1, x = 0;

            printf("%s",buffer);
        }

        y = 23, x = 0;

        for (size_t a = 0; a < WIDTH; a++)
            putc('\0');

        x = checkx, y = checky-1;

        update_cursor(x,y);

        normalprint = true;
    }

    return;
}

void cls(){

    normalprint = false;

    x = 0;
    y = 0;

    for(size_t h = 0; h < HEIGHT; h++)
        for (size_t j = 0; j < WIDTH; j++)
            putc('\0');

    x = 0;
    y = 0;

    update_cursor(x,y);

    normalprint = true;
    
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

void print_signed(int si_num, int base){
    if(si_num < 0){
        putc('-');
        si_num *= -1;
        print_unsigned(si_num,base);
    } else if(si_num > 0){
        putc('+');
        print_unsigned(si_num,base);
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

//Input

int input(){

    int bb = 0;

    bool end = false;

    int howmany = 40;

    char buffer[howmany];

    for (size_t cc = 0; cc < howmany; cc++)
        buffer[cc] = '\0';

    do{

        int gotten = getc(x,y);

        switch (gotten){

            case '\0':
                //Se è un null non fare nulla
                break;

            //Invio

            case '\20':
                printf("\b");
                end = true;
                break;
            
            //Backspace

            case '\21':
                printf("\b");
                if (bb > 0){
                    printf("\b");
                    bb--;
                    buffer[bb] = '\0';
                }
                break;

            //F1 - F12

            case '\1':
                printf("\b");
                //end = true;
                break;
            
            case '\2':
                printf("\b");
                //end = true;
                break;
            
            case '\3':
                printf("\b");
                //end = true;
                break;
            
            case '\4':
                printf("\b");
                //end = true;
                break;
            
            case '\5':
                printf("\b");
                //end = true;
                break;
            
            case '\6':
                printf("\b");
                //end = true;
                break;
            
            case '\7':
                printf("\b");
                //end = true;
                break;
            
            case '\18':
                printf("\b");
                //end = true;
                break;
            
            case '\19':
                printf("\b");
                //end = true;
                break;
            
            case '\10':
                printf("\b");
                //end = true;
                break;
            
            case '\11':
                printf("\b");
                //end = true;
                break;
            
            case '\12':
                printf("\b");
                //end = true;
                break;

            default:
                printf("\b%c",gotten);
                buffer[bb] = gotten;
                bb++;
                break;
        }

        if (bb >= howmany){
            end = true;
        }

    } while (end == false);
    
    return buffer;
}

//Da stringa ad int

int atoi(const char *s_num){

    int result = 0;
    int num_sign = 1;

    while (('-' == (*s_num)) || ((*s_num) == '+')){

        if (*s_num == '-')
            num_sign *= -1;
        s_num++;
    }

    while ((*s_num >= '0') && (*s_num <= '9')){
        
        result = (result * 10) + ((*s_num) - '0');
        s_num++;
    }

    return (result * num_sign);
}

//Calcolatrice

void calcolatrice(){

    printf("\r\tFirst number: ");

    char *num1ch = (char*)input();
    int num1 = atoi(num1ch);
    
    printf("\r\tSecond number: ");
    
    char *num2ch = (char*)input();
    int num2 = atoi(num2ch);

    printf("\r\tOperator: ");
    
    char *segnoch = (char*)input();
    
    if (strcmps(segnoch,"+") == 0){
        printf("\r\r\t%d + %d = %d",num1,num2,num1+num2);
    } else if (strcmps(segnoch,"-") == 0){
        printf("\r\r\t%d - %d = %d",num1,num2,num1-num2);
    } else if (strcmps(segnoch,"*") == 0){
        printf("\r\r\t%d * %d = %d",num1,num2,num1*num2);
    } else if (strcmps(segnoch,"/") == 0){
        printf("\r\r\t%d / %d = %d",num1,num2,num1/num2);
    } else if (strcmps(segnoch,"%") == 0){
        printf("\r\r\t%d %% %d = %d",num1,num2,num1%num2);
    } else {
        printf("\r\r\tOperator not recognized...");
    }

    return;
}

//Command mode

void CMDode(){

    OSScreenInit();
    
    while (true){
        
        char *Usercmd = (char*)input();

        if (strcmps(Usercmd,"help") == 0){
            //help: restituisce i comandi
            printf("\r\thelp: displays a list of commands\r\tversion: displays the OS version\r\tcalc: opens a calculator\r\tcls: clears the screen");
            printf("\r%s",ready);
        } else if (strcmps(Usercmd,"version") == 0){
            //version: restituisce la versione
            printf("\r\tVersion: %s",NaitOS_v);
            printf("\r%s",ready);
        } else if (strcmps(Usercmd,"calc") == 0){
            //calc: apre la calcolatrice (ancora molto base)
            calcolatrice();
            printf("\r%s",ready);
        } else if (strcmps(Usercmd,"cls") == 0){
            //cls: pulisce lo schermo
            OSScreenInit();
        } else if (strcmps(Usercmd,"get") == 0){
            //cls: pulisce lo schermo
            printf("\r\tX: %u Y: %u",get_cursor_position()%80,get_cursor_position()/80);
            printf("\r%s",ready);
        } else {
            //Se non è stato inserito un comando valido, resituisci un errore
            printf("\r\tCommand not recognized, type ""help"" for a list of commands");
            printf("\r%s",ready);
        }
    }
    
    return;
}

//Schermata del sistema operativo

void OSScreenInit(){
    cls();
    printf("[%s]\r%s",NaitOS_v,ready);
    
    return;
}

//Schermata di errore (viene chiamata dalle routines)

extern void ErrorScreenInit(){
    DEFAULT_COLOR = 0x4C;
    cls();
    x = 0, y = 10;
    printf("\t\t  / \\\t\t  NaitOS: There was an unexpected error and the OS crashed!!!\r\t\t / | \\\t\t\t\t Try to restart the computer to fix this issue\r\t\t/  .  \\");
    x = 25, y = 13;
    printf("\t ERROR: ");

    return;
}