#include "basic.c"

extern "C" void _start(){
    print("Hello Kernel!",WHITE);
    cls();
    print("Hi",BLUE);
    return;
}