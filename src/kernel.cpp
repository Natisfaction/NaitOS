#include "basic.h"

extern "C" void __attribute((section(".entry"))) main(){
    printf("My name is %c",'C');

    //Loop infinito

    while (TRUE)
    {
        continue;
    }

    return;
}