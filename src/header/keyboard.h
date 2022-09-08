#include "../header/idt.h"
#include "../header/irq.h"
#include "../header/in_asm.h"
#include "../header/stdio.h"

void keyboard_handler(struct regs *r);
void keyboard_install();