#include "basic.h"

extern "C" void __attribute((section(".entry"))) main(){
    putc('H',0xF);
    return;
}