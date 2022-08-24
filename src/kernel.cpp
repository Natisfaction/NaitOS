#include <stdint.h>
#include <stddef.h>

#include "inout.h"
#include "cursor.h"

//FIX LIGHT MODE NOT SHOWING CORRECTLY!

extern "C" void _start(void){
    cls(BLUE,CYAN);
    print("                                     NaitOS                                     ",CYAN,BLUE,0,0);
    print("                                                                                ",CYAN,BLUE,0,24);
    print("NaitOS\ >",BLUE,CYAN,0,1);
    return;
}