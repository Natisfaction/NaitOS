#include "basic.cpp"

//Add background!

extern "C" void _start(){
    print("Hi from kernel!",YELLOW);
    cls();
    print("Hi again!",BLUE);
    return;
}