#pragma once

#include <stdint.h>

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

//Cursore

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y);

void update_cursor_full(uint16_t pos);
uint16_t get_cursor_position();

//Inutilizzate 

void Up();
void Down();
void Dx();

void Sx();

//Carattere & video

int getc(int x, int y);
int getcol(int x, int y);

void scroll();
void cls();
void putc(char c);

void puts(const char* str);

void print_unsigned(int u_num, int base);
void print_signed(int s_num, int base);
void printf(const char* fmt, ...);

//Tastiera

int input();
void getcommand();

//Schermo & errore

void OSScreenInit();
extern void ErrorScreenInit();