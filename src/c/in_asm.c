#include "in_asm.h"

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