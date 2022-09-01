#include "basic.h"

extern "C" void __attribute((section(".entry"))) main(){
    printf("Hello %s!","World");

    //Loop infinito

    while (TRUE)
    {
        continue;
    }

    return;
}