#include "../Drivers/timer.h"

int timer_ticks = 0;
int seconds = 0;

void timer_handler(regs *r){
	timer_ticks++;
	
	if (timer_ticks % 18 == 0)
		seconds++;
}

void timer_install(){
	irq_install_handler(0, timer_handler);
}

//void timer_phase(int hz){
//	int divisor = 1193180 / hz;
//	outb(0x43, 0x36);
//	outb(0x40, divisor & 0xFF);
//	outb(0x40, divisor >> 8);
//}

void sleep(int ticks){
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}