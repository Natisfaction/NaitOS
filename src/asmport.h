#pragma once

#include <stdint.h>

void outb(uint16_t port, uint8_t data){
	__asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
	return;
}

uint8_t inb(uint16_t port){
	uint8_t res;
	__asm volatile("inb %1, %0" : "=a"(res) : "Nd"(port));
	return res;
}

void outw(uint16_t port, uint16_t value){
	__asm volatile ("outw %w0, %1" : : "a" (value), "id" (port) );
}

uint16_t inw(uint16_t port){
   uint16_t ret;
   __asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
} 

void outl(uint16_t port, uint32_t value){
	__asm volatile ("outl %%eax, %%dx" : : "d" (port), "a" (value));
}

uint32_t inl(uint16_t port){
   uint32_t ret;
   __asm volatile ("inl %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
} 
