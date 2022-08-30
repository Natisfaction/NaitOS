#include "basic.h"

extern "C" void __attribute((section(".entry"))) main(){
    printn(69);
    //Loop infinito
    while(TRUE){
        continue;
    }
    return;
}