#include <stdint.h>
#include <stddef.h>
#include "header/basic.h"
#include "header/inout.h"
#include "header/cursor.h"

//Add background!

extern "C" void _start(){
    print("Hi from kernel!",BLACK,WHITE);
    return;
}