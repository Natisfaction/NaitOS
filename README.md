# NaitOS: my first self-developed OS.

NaitOS is an Operating System based on i386 architecture, developed by me, a really passionate coder who loves low level coding. Right now I'm no longer writing more code for this OS, just because I moved on coding another UEFI-based OS with a friend of mine.

# What are the commands NaitOS supports???

The OS has the following command:
 - **help**    --> Displays the command list
 - **version** --> Displays the OS version
 - **calc**    --> Opens a basic calculator
 - **conv**    --> Opens a basic number converter
 - **cls**     --> Clears the screen

# Features implemented

Current features include:
 - **Bootloader** fully implemented
 - **IDT**, **GDT**, **ISR**, **IRQ**
 - **Keyboard** fully implemented
 - **Printing to screen** fully implemented
 - **Error management** fully implemented

# How to...

...cross compile the OS???
 - To cross-compile the OS, you'll need to build an i686 cross compiler. You can watch [this](https://www.youtube.com/watch?v=TgIdFVOV_0U) tutorial to learn how to do it, or you can just download a pre-made cross-compiler from [here](https://github.com/lordmilko/i686-elf-tools/releases). If you have done all the steps correctly, you'll only need to edit a bit the "cmp.sh" file to adapt the directories, and it should compile perfectly.

...launch the OS???
 - To launch the OS, you'll need an **i386** or **i686** emulator: in my development i decided to use *qemu-system-i386*. You can download it from [here](https://www.qemu.org/download/).

# What next???

Sadly, as I already said, I'm not going to maintain this OS. It was my first-made OS, and I'd really like if it stays as it is right now!
