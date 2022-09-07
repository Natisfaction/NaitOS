#include <stdint.h>
#include <stddef.h>

#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"

int num = 9;
int res;

extern void main(){
    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    __asm__ volatile("sti");
    //res = num / 0;
    DarkScreenInit();
    printf("Hello %s!","Kernel");
    return;
}