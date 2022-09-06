#pragma once

#include <stdint.h>

void putc(char c);
void puts(const char* str);
void print_unsigned(int u_num, int base);

void print_signed(int s_num, int base);
void printf(const char* fmt, ...);