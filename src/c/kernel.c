#include <stdint.h>
#include <stddef.h>

#include "../header/stdio.h"
#include "../header/gdt.h"
#include "../header/idt.h"
#include "../header/isr.h"
#include "../header/irq.h"
#include "../header/timer.h"

extern void main(){
    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    __asm__ volatile("sti");
    DarkScreenInit();
    printf("Hello: ");
    timer_handler(19);
    printf("World");
    return;
}