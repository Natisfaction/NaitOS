#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../header/stdio.h"
#include "../header/in_asm.h"
#include "../header/string.h"

int DEFAULT_COLOR  =  0x1F;
int x = 0,  y = 0;
bool normalprint = true, command = false;

const char* ready = "Ready! > ";
const char* NaitOS_v = "NaitOS Version 2.0";

const char* helpdoc = "help: displays a list of commands\r\tversion: displays the OS version\r\tcalc: opens a calculator\r\tconv: opens a number converter\r\tcls: clears the screen";
char oldbuffer[32] = "";

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

const char Numeri[] = "0123456789ABCDEF";

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

                //b Stampa un numero in base 2
                case 'b':   print_unsigned(va_arg(args, int),BIN);
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

int atoi(const char *s_num, int base){

    int i_num;
    int result = 0;
    int num_sign = 1;

    while (('-' == (*s_num)) || ((*s_num) == '+')){

        if (*s_num == '-')
            num_sign *= -1;
        s_num++;
    }

    while (((*s_num >= '0') && (*s_num <= '9')) || ((*s_num >= 'A') && (*s_num <= 'F'))){
        if ((*s_num >= '0') && (*s_num <= '9')){
            i_num = *s_num - '0';
        } else {
            switch (*s_num){
                case 'A':
                    i_num = 10;
                    break;
                
                case 'B':
                    i_num = 11;
                    break;
                
                case 'C':
                    i_num = 12;
                    break;
                
                case 'D':
                    i_num = 13;
                    break;
                
                case 'E':
                    i_num = 14;
                    break;
                
                case 'F':
                    i_num = 15;
                    break;
                
                case 'a':
                    i_num = 10;
                    break;
                
                case 'b':
                    i_num = 11;
                    break;
                
                case 'c':
                    i_num = 12;
                    break;
                
                case 'd':
                    i_num = 13;
                    break;
                
                case 'e':
                    i_num = 14;
                    break;
                
                case 'f':
                    i_num = 15;
                    break;
                
                default:
                    break;
            }
        }
        
        result = (result * base) + i_num;
        s_num++;
    }

    return (result * num_sign);
}

void scan_string(char* str){

    int bb = 0;

    int howmany = 32;

    bool end = false, restored = false;

    for (size_t cc = 0; cc < howmany; cc++)
        str[cc] = '\0';
    
    do{

        int gotten = getc(x,y);

        switch (gotten){

            case '\0':
                //Se è un null non fare nulla
                break;

            //Freccia in sù = carica il comando precedente

            case '\14':
                putc('\b');
                if (!restored && command && bb == 0){
                    for (bb = 0; oldbuffer[bb] != 0; bb++){
                        str[bb] = oldbuffer[bb];
                        putc(str[bb]);
                    }

                    restored = true;
                }
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
                    str[bb] = '\0';
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
                str[bb] = gotten;
                bb++;
                break;
        }

        if (bb >= howmany){
            end = true;
        }

    } while (end == false);

    for (size_t ee = 0; ee < howmany && command; ee++)
        oldbuffer[ee] = '\0';
    
    for (bb = 0; str[bb] != '\0' && command; bb++)
        oldbuffer[bb] = str[bb];
}

void scan_int(int* i, int base){
    char* strnum;
    scan_string(strnum);
    *i = atoi(strnum,base);
}

void scan_char(char* c){
    char* str;
    scan_string(str);
    *c = str[0];
}

void scanf(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);

    while (*fmt){
        switch (*fmt){
        //Se lo trovo, mi preparo a riconoscere cosa dare in output
        case '%':
            fmt++;
            switch (*fmt){
                case 'u':
                case 'i':
                case 'd':   scan_int(va_arg(args, int),DEC);
                            break;

                case 's':   scan_string(va_arg(args, char*));
                            break;

                case 'c':   scan_char(va_arg(args, int));
                            break;

                case 'o':   scan_int(va_arg(args, int),OCT);
                            break;

                case 'p':
                case 'X':
                case 'x':   scan_int(va_arg(args, int),HEX);
                            break;

                case 'b':   scan_int(va_arg(args, int),BIN);
                            break;

                default:    break;
            }
            break;

        default:    break;
        }
        
        fmt++;
    }
    va_end(args);

    return;
}

void calcolatrice(){

    int num1,num2;
    char segno;

    printf("\r\tFirst number: ");

    scanf("%d",&num1);
    
    printf("\r\tSecond number: ");
    
    scanf("%d",&num2);

    printf("\r\tOperator: ");
    
    scanf("%c",&segno);
    
    switch (segno){
        case '+':
            printf("\r\r\t%d + %d = %d",num1,num2,num1+num2);
            break;

        case '-':
            printf("\r\r\t%d - %d = %d",num1,num2,num1-num2);
            break;
        
        case '*':
            printf("\r\r\t%d * %d = %d",num1,num2,num1*num2);
            break;
        
        case '/':
            printf("\r\r\t%d / %d = %d",num1,num2,num1/num2);
            break;
        
        case '%':
            printf("\r\r\t%d %% %d = %d",num1,num2,num1%num2);
            break;
        
        default:
            printf("\r\r\tOperator not recognized...");
            break;
    }

    return;
}

void convertitore(){
    
    int num;

    printf("\r\tType the base 10 number: ");
    
    scanf("%d",num);

    printf("\r\tNumber in base H: %x",num);
    printf("\r\tNumber in base 8: %o",num);
    printf("\r\tNumber in base 2: %b",num);

    return;
}

void CMDode(){

    OSScreenInit();

    char *Usercmd;
    
    while (true){

        command = true;

        scan_string(Usercmd);

        command = false;

        if (strcmps(Usercmd,"help") == 0){
            //help: restituisce i comandi
            printf("\r\t%s",helpdoc);
            printf("\r%s",ready);
        } else if (strcmps(Usercmd,"version") == 0){
            //version: restituisce la versione
            printf("\r\tVersion: %s",NaitOS_v);
            printf("\r%s",ready);
        } else if (strcmps(Usercmd,"calc") == 0){
            //calc: apre una semplice calcolatrice
            calcolatrice();
            printf("\r%s",ready);
        } else if (strcmps(Usercmd,"cls") == 0){
            //cls: pulisce lo schermo
            OSScreenInit();
        } else if (strcmps(Usercmd,"test") == 0){
            //Test for scan function
            char* str;
            scanf("%s",str);
            printf("\r%s",str);
            printf("\r%s",ready);
        } else if (strcmps(Usercmd,"conv") == 0){
            //conv: un semplice convertitore
            convertitore();
            printf("\r%s",ready);
        } else {
            //Se non è stato inserito un comando valido, resituisci un errore
            printf("\r\tCommand not recognized, type ""help"" for a list of commands");
            printf("\r%s",ready);
        }

    }
    
    return;
}

void OSScreenInit(){
    cls();
    printf("[%s]\r%s",NaitOS_v,ready);
    
    return;
}

extern void ErrorScreenInit(){
    DEFAULT_COLOR = 0x4C;
    cls();
    x = 0, y = 10;
    printf("\t\t  / \\\t\t  NaitOS: There was an unexpected error and the OS crashed!!!\r\t\t / | \\\t\t\t\t Try to restart the computer to fix this issue\r\t\t/  .  \\");
    x = 25, y = 13;
    printf("\t ERROR: ");

    return;
}