#pragma once

#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t byte);
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y);
void putc(char c);
void puts(const char* str);
void print_unsigned(int u_num, int base);
void print_signed(int s_num, int base);