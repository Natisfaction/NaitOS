#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include "inout.h"
#include "cursor.h"

int hi = 4;

//FIX PRINT NOT SHOWING CORRECTLY!

extern "C" void _start(void){
    printf("%d",hi);
    return;
}