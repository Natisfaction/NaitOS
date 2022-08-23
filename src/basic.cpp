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

//SCHERMO E CURSORE (DECIMALE)

const int WIDTH         =   80;
const int HEIGHT        =   25;

int ScreenX;
int ScreenY;

static char *vidmem     =   (char*)0xB8000;

static uint16_t x       =   0;
static uint16_t y       =   0;

// ---------------------------------
//            FUNZIONI
// ---------------------------------

//FUNZIONE PER SCRIVERE UN CARATTERE (DEPRECATA)

void putc(char *ch){
    *(char*)0xB8000 = *ch;
    return;
}

//DIVERTIMENTO CON L'INLINE ASSEMBLER PER INIZIALIZZARE UN PO DI COSUCCE SUL CURSORE

static inline void outb(unsigned short port, unsigned char value){      //PER MOSTRARE IL PUNTATORE
    __asm volatile("OUTB %0, %1" : : "a"(value), "Nd"(port));           //QUI CI SI DIVERTE
    return;
}

static inline unsigned char inb(unsigned short port){                   //PER NASCONDERE IL PUNTATORE
    unsigned char ret;                                                  //SERVE UN CARATTERE DI RITORNO
    __asm volatile("INB %1, %0" : "=a"(ret) : "Nd"(port) );             //DIVERTIMENTO ASSICURATO
    return ret;                                                         //CARATTERE DI RITORNO
}

//FUNZIONI IN C PER IL CURSORE

void show_cursor(void){                                                 //MOSTRA IL CURSORE
    uint8_t x = 15, y = 15;

	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | x);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | y);

    return;
}

void hide_cursor(void){

	outb(0x3D4, 0x0A);                                                  //NASCONDE IL CURSORE
	outb(0x3D5, 0x20);

    return;
}

void set_cursor(uint16_t x, uint16_t y){                                //SPOSTA IL CURSORE IN UNA POSIZIONE PRECISA
    uint16_t position = y * WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, ((uint8_t)(position & 0xFF)));

    outb(0x3D4, 0x0E);
    outb(0x3D5, ((uint8_t)((position >> 8) & 0xFF)));

    show_cursor();

    return;
}

uint16_t get_cursor_position(void){                                     //OTTIENI LA POSIZIONE DEL CURSORE
    uint16_t pos = 0;

    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);

    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;

    return pos;
}

//FUNZIONE PRINT (SCRIVE UNA STRINGA, CON COLORE)

void print(char *str,int colore){       //SERVE LA FRASE DA SCRIVERE E IL COLORE
    int x = 0, y = 0;
    char *vidmem = (char*)0xB8000;      //PUNTATORE ALLA MEMORIA VIDEO
    while(*str != 0){                   //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = *str++;             //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = colore;             //E ASSIEME A QUELLA ANCHE IL COLORE
        x++;
    }
    set_cursor(x,y);
    return;
}

//FUNZIONE PER IL CLEARSCREEN

void cls(void){
    char *vidmem = (char*)0xB8000;      //PUNTATORE ALLA MEMORIA VIDEO
    for(int i=0; i<2000; i++){          //FINCHE' NON ARRIVO ALLA FINE DELLA FRASE (NULL)
        *vidmem++ = 0;                  //INCREMENTIAMO LA MEMORIA VIDEO
        *vidmem++ = WHITE;              //E ASSIEME A QUELLA ANCHE IL COLORE
    }
    set_cursor(0,0);
    return;
}