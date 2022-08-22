#include <stdint.h>
#include <stddef.h>

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

//VERO FALSO (DECIMALE)

const int FALSE         =   0;
const int TRUE          =   1;

//SCHERMO (DECIMALE)

const int WIDTH    =   80;
const int HEIGHT   =   25;

int ScreenX;
int ScreenY;

static char *vidmem = (char*)0xB8000;

// ---------------------------------
//            FUNZIONI
// ---------------------------------

//FUNZIONE PER SCRIVERE UN CARATTERE (DEPRECATA)

void putc(char *ch){
    *(char*)0xB8000 = *ch;
    return;
}

//FUNZIONE PER SPOSTARE IL CURSORE

//DIVERTIMENTO CON L'INLINE ASSEMBLER PER INIZIALIZZARE UN PO DI COSUCCE

static inline void outb(unsigned short port, unsigned char value){      //PER MOSTRARE IL PUNTATORE
    __asm volatile("OUTB %0, %1" : : "a"(value), "Nd"(port));           //QUI CI SI DIVERTE
    return;
    }

static inline unsigned char inb(unsigned short port){                   //PER NASCONDERE IL PUNTATORE
    unsigned char ret;                                                  //SERVE UN CARATTERE DI RITORNO
    __asm volatile("INB %1, %0" : "=a"(ret) : "Nd"(port) );             //DIVERTIMENTO ASSICURATO
    return ret;                                                         //CARATTERE DI RITORNO
    }

//FUNZIONE IN C PER IMPOSTARE IL CURSORE IN UNA POSIZIONE PRECISA (X,Y)

void setcursor(int x, int y){
    uint16_t position = y * WIDTH + x;

    outb(0x3D4,0x0F);
    outb(0x3D5,((uint8_t)(position & 0xFF)));
    outb(0x3D4,0x0E);
    outb(0x3D5,((uint8_t)((position >> 8) & 0xFF)));
    }

//FUNZIONE PRINT (SCRIVE UNA STRINGA, CON COLORE)

void print(char *str,int colore){       //SERVE LA FRASE DA SCRIVERE E IL COLORE
    int x = 0, y = 0;
    char *vidmem = (char*)0xB8000;      //PUNTATORE ALLA MEMORIA VIDEO
    while(*str != 0){                   //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = *str++;             //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = colore;             //E ASSIEME A QUELLA ANCHE IL COLORE
        x++;
        setcursor(x,y);
    }
    return;
}

//FUNZIONE PER IL CLEARSCREEN

void cls(){
    char *vidmem = (char*)0xB8000;      //PUNTATORE ALLA MEMORIA VIDEO
    for(int i=0; i<2000; i++){          //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = 0;                  //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = BLACK;              //E ASSIEME A QUELLA ANCHE IL COLORE
    }
    setcursor(0,0);                     //RIPRISTINIAMO IL CURSORE ALLA SUA POZIONE INIZIALE
    return;
}