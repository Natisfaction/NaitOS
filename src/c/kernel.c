#include <stdint.h>
#include <stddef.h>

#include "../header/stdio.h"
#include "../Drivers/gdt.h"
#include "../Drivers/idt.h"
#include "../Drivers/isr.h"
#include "../Drivers/irq.h"
#include "../Drivers/timer.h"
#include "../Drivers/keyboard.h"

extern void main(){
    //Installa i componenti base, abilita gli interrupts, e tutto il resto
    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    __asm__ volatile("sti");
    timer_install();
    keyboard_install();
    OSScreenInit();
    //Ottiene i comandi
    getcommand();

    return;
}