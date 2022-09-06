#pragma once

#include <stdint.h>

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y);

uint16_t get_cursor_position();
void putc(char c);
void puts(const char* str);

void print_unsigned(int u_num, int base);
void print_signed(int s_num, int base);
void printf(const char* fmt, ...);