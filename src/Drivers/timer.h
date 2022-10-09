#pragma once

#include "../Drivers/idt.h"
#include "../Drivers/irq.h"

void timer_phase(int hz);
void timer_handler(regs *r);
void sleep(int ticks);
void timer_install();