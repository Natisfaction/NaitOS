#include "stdio.h"

extern "C" void __attribute((section(".entry"))) main(){
    printf("My name is %s","Christian");

    //Loop infinito

    while (TRUE)
    {
        continue;
    }
    return;
}