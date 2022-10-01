#include "../Drivers/idt.h"
#include "../Drivers/irq.h"
#include "../header/in_asm.h"
#include "../header/stdio.h"

void keyboard_handler(struct regs *r);
int computate();
void keyboard_install();