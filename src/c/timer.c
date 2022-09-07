#include "../header/idt.h"
#include "../header/irq.h"

int timer_ticks = 0;
int seconds = 0;

void timer_phase(int hz){
	int divisor = 1193180 / hz;	   /* Calculate our divisor */
	outb(0x43, 0x36);			 /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);	 /* Set high byte of divisor */
}

void timer_handler(regs *r){
	/* Increment our 'tick count' */
	timer_ticks++;
	

	if (timer_ticks % 18 == 0){
		seconds++;
	}
}

void timer_install(){
	irq_install_handler(0, timer_handler);
}

void sleep(int second){
    int csec = seconds;
    while(csec < second);
    return;
}