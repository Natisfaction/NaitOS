#include <stdint.h>
#include <stddef.h>

#include "cursor.h"
#include "inout.h"

extern "C" void _start(){
    print("Test print",GREEN,BLUE);
    return;
}