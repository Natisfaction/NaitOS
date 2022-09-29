#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../header/stdio.h"
#include "../header/in_asm.h"
#include "../header/string.h"

int DEFAULT_COLOR  =  0x1F;
int x = 0,  y = 0;
bool normalprint = true;          //Variabile di sicurezza (true se sta stampando, false se deve andare a libero schermo)

const char* ready = "Ready! > ";
const char* NaitOS_v = "NaitOS Version 0.5";

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

//Commentate per bugs

void Up(){
    //if (y != 1){
    //    y--;
    //}
    //update_cursor(x,y);

    return;
}

void Down(){
    //if (y != 23){
    //    y++;
    //}
    //update_cursor(x,y);

    return;
}

void Dx(){
    //if (!(x == 79 && y == 23)){
    //    x++;
    //}
    //update_cursor(x,y);

    return;
}

void Sx(){
    //if (!(x == 0 && y == 1)){
    //    x--;
    //}
    //update_cursor(x,y);

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
            for (size_t i = 0; i < 3; i++){
                putc(' ');
            }
            break;

        case '\r':
            x = 0;
            y++;
            break;
        
        case '\b':
            x--;
            putc('\0');
            x--;
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

        for (size_t a = 0; a < WIDTH; a++){
            putc('\0');
        }
        

        for (size_t b = 1; b != HEIGHT; b++){

            //Prendi i caratteri

            y = b, x = 0;

            for (size_t c = 0; c < WIDTH; c++){
                buffer[c] = getc(x,y);
                x++;
            }

            //Pulisci la linea

            y = b, x = 0;

            for (size_t c = 0; c < WIDTH; c++){
                putc('\0');
            }

            //Scrivi alla y precedente il buffer

            y = b-1, x = 0;

            printf("%s",buffer);
        }

        y = 23, x = 0;

        for (size_t a = 0; a < WIDTH; a++){
            putc('\0');
        }

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

    for(size_t h = 0; h < HEIGHT; h++){
        for (size_t j = 0; j < WIDTH; j++){
            putc('\0');
        }
    }

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

//Calcolatrice

void calcolatrice(){

    char num1ch, num2ch, segno;

    printf("\tInserisci il primo numero: ");
    //Serve per fare in modo di avere un numero / segno
    do{
        num1ch = input();
    } while (num1ch == '\0');
    printf("%c",num1ch);

    printf("\r\tInserisci il secondo numero: ");
    do{
        num2ch = input();
    } while (num2ch == '\0');
    printf("%c",num2ch);

    printf("\r\tInserisci il segno: ");
    do{
        segno = input();
    } while (segno == '\0');
    printf("%c",segno);

    int num1 = num1ch - '0';
    int num2 = num2ch - '0';
    int risultato;

    switch (segno){
        case '+':
            risultato = num1 + num2;
            printf("\r\tIl risultato di %c %c %c e' %d\r%s",num1ch,segno,num2ch,risultato,ready);
            break;
        case '-':
            risultato = num1 - num2;
            printf("\r\tIl risultato di %c %c %c e' %d\r%s",num1ch,segno,num2ch,risultato,ready);
            break;
        case '*':
            risultato = num1 * num2;
            printf("\r\tIl risultato di %c %c %c e' %d\r%s",num1ch,segno,num2ch,risultato,ready);
            break;
        case '/':
            risultato = num1 / num2;
            printf("\r\tIl risultato di %c %c %c e' %d\r%s",num1ch,segno,num2ch,risultato,ready);
            break;
        case '%':
            risultato = num1 % num2;
            printf("\r\tIl risultato di %c %c %c e' %d\r%s",num1ch,segno,num2ch,risultato,ready);
            break;
        default:
            printf("\r\tErrore: non hai inserito un operatore valido!\r%s",ready);
            break;
    }

    return;
}

//Funzioni per la tastiera (processing del carattere)

char Usercmd[32];
bool command = false;
int index;

int input(){
    int gotten = getc(x,y);
    if (gotten != '\0'){
        printf("\b");
        return gotten;
    } else {
        return 0;
    }
}

void getcommand(){

    command = true;
    char character;

    while (true){

        int i = 0;
        
        for (size_t aa = 0; aa < 32; aa++){
            Usercmd[aa] = '\0';
        }

        while (true){

            character = input();

            if (character == '\0'){
                continue;
            } else if (character == '.'){
                printf("\r");
                break;
            } else {
                printf("%c",character);
                Usercmd[i] = character;
                i++;
            }

        }
        
        //Non potendo usare lo switch case, utilizzo l'if

        if (strcmps(Usercmd,"help") == 0){
            //help: restituisce i comandi
            printf("\thelp: displays a list of commands\r\tversion: displays the OS version\r\tcalc: opens a calculator\r%s",ready);
        } else if (strcmps(Usercmd,"version") == 0){
            //version: restituisce la versione
            printf("\tVersion: %s\r%s",NaitOS_v,ready);
        } else if (strcmps(Usercmd,"calc") == 0){
            //calc: apre la calcolatrice (ancora molto base)
            calcolatrice();
        } else if (strcmps(Usercmd,"cls") == 0){
            //cls: pulisce lo schermo
            OSScreenInit();
        } else {
            //Se non è stato inserito un comando valido, resituisci un errore
            printf("\tCommand not recognized\r%s",ready);
        }

    }
}

//Schermata del sistema operativo

void OSScreenInit(){
    cls();
    printf("[NaitOS version 0.5]\r%s",ready);
    
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