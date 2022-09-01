#include "basic.h"

extern "C" void __attribute((section(".entry"))) main(){
    printf("Hello Kernel! %s","Test string");

    //Loop infinito

    while (TRUE)
    {
        continue;
    }

    return;
}