#pragma once

#include <stdint.h>
#include <stddef.h>
#include "in_asm.h"

extern void gdt_flush();
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();