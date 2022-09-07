#pragma once

#include <stdint.h>
#include <stddef.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t byte);
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, char val, size_t count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
size_t strlen(const char *str);