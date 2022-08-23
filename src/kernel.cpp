#include "basic.cpp"

//Add background!

extern "C" void _start(){
    set_cursor(0,0);
    hide_cursor();
    set_cursor(9,9);
    show_cursor(1,1);
    return;
}