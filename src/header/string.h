#pragma once

#include "types.h"

void *memsets(void *s, int c, size_t n);
void *memcpys(void *dst, const void *src, size_t n);
void *memmoves(void *dst, const void *src, size_t n);
void *memchrs(const void *buf, int c, size_t n);
int memcmps(const void *s1, const void *s2, size_t n);

size_t strlens(const char *str);
char *strcpys(char *dst, const char *src);
char *strncpys(char *dst, const char *src, size_t n);
int strcmps(const char *s1, const char *s2);

char *strcpy_safes(char *dst, const char *src, size_t dstSize);