#include <stdint.h>
#include <stddef.h>

#include "cursor.h"
#include "inout.h"

extern "C" void _start(){
    print("Hi from kernel!",BLACK,WHITE);
    cls();
    print("Hello there!",RED,YELLOW);
    return;
}