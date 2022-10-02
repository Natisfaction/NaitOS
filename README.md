# NaitOS

*Developing an OS just for fun purposes!!!*


## What is NaitOS???

NaitOS is an Operating System, currently under development, by a young man.

At first, it was supposed to be a little challenge, but has now became a really good passion.

## What are the current commands???

Right now, the OS has the following command:
 - *help*    --> Displays the command list
 - *version* --> Displays the OS version
 - *calc*    --> Opens a basic calculator
 - *conv*    --> Opens a basic number converter
 - *cls*     --> Clears the screen

    ***More coming up soon!!!***

# Features

Current features include:
 - **Bootloader** fully implemented
 - **IDT**, **GDT**, **ISR**, **IRQ**
 - **Keyboard** fully implemented
 - **Printing to screen** fully implemented
 - **Error management** fully implemented

## How to...

...cross compile the OS???
 - To cross-compile the OS, you'll need to build an i686 cross compiler. You can watch this tutorial to learn how to do it: https://www.youtube.com/watch?v=TgIdFVOV_0U , or you can just download a pre-made cross-compiler from here: https://github.com/lordmilko/i686-elf-tools/releases . If you have done all the steps correctly, you'll only need to edit a bit the "cmp.sh" file, and it should work.

...launch the OS???
 - To launch the OS, you'll need a specific emulator: *qemu-system-i386*. You can download it from https://www.qemu.org/download/

## What next???

A lot of features are coming up in version 2.0 (not soon), the main ones are the following:
 - **Memory management**
 - **File System** (FAT12)

***Stay tuned!!!***
