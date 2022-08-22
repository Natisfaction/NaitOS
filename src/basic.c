#include <stdint.h>
#include <stddef.h>

//QUI DEFINISCO LE COSE BASILARI (PER ORA I COLORI, VERO FALSO, E LE VARIABILI I/O)

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

//VERO FALSO (QUI NON SERVE IN ESADECIMALE)

const int FALSE         =   0;
const int TRUE          =   1;

//DIMENSIONI DELLO SCHERMO

const unsigned WIDTH    =   80;
const unsigned HEIGHT   =   25;
char* Screen            =   (char*)0xB8000;

int ScreenX;
int ScreenY;

static char *vidmem = (char*)0xB8000;
static char *rawmem = (char*)0xB8000;

// ---------------------------------
//    ADESSO CREO LE MIE FUNZIONI
// ---------------------------------

//FUNZIONE PER SCRIVERE UN CARATTERE (ATTUALMENTE INUTILE)

void putc(char *ch){
    *(char*)0xB8000 = *ch;
    return;
}

//FUNZIONE "PRINT" (SCRIVE UNA STRINGA, CON COLORE)

void print(char *str,int colore){       //SERVE LA FRASE DA SCRIVERE E IL COLORE
    char *vidmem = (char*)0xB8000;
    while(*str != 0){                   //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = *str++;             //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = colore;             //E ASSIEME A QUELLA ANCHE IL COLORE
    }
    return;
}

//FUNZIONE DI CLEARSCREEN

static inline void outb(unsigned short port, unsigned char value){
    __asm volatile("OUTB %0, %1" : : "a"(value), "Nd"(port));
    return;
    }

static inline unsigned char inb(unsigned short port){
    unsigned char ret;
    __asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
    }

void setcursor(int x, int y)
{
    uint16_t position = y * WIDTH + x;
    outb(0x3D4,0x0F);
    outb(0x3D5,((uint8_t)(position & 0xFF)));
    outb(0x3D4,0x0E);
    outb(0x3D5,((uint8_t)((position >> 8) & 0xFF)));
}

void cls(){       //SERVE LA FRASE DA SCRIVERE E IL COLORE
    char *vidmem = (char*)0xB8000;
    for(int i=0; i<2000; i++){          //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = 0;                  //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = BLACK;              //E ASSIEME A QUELLA ANCHE IL COLORE
    }
    return;
}