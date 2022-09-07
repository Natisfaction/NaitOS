#include "../header/in_asm.h"

uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile( "inb {%[port], %[retreg] | %[retreg], %[port]}"
                   : [retreg]"=a"(ret)
                   : [port]"Nd"(port) );

    return ret;
}

void outb(uint16_t port, uint8_t byte){
    __asm__ volatile( "outb {%[byte], %[port] | %[port], %[byte]}"
                   :
                   : [byte]"a"(byte),
                     [port]"Nd"(port) );

    return;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(const char *str)
{
    size_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}