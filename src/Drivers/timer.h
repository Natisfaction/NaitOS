#pragma once

void timer_phase(int hz);
void timer_handler(regs *r);
void timer_install();
void sleep(int second);