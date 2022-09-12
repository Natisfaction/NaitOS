export PREFIX="/mnt/c/Users/cl406/Desktop/Coding/NaitOS/Nait/Toolchain/i686-elf"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
echo "======================== ASM COMPILING ======================="
nasm -f bin "src/asm/boot.asm" -o "build/boot.bin"
nasm -f bin "src/asm/fine.asm" -o "build/fine.bin"
nasm -f elf "src/asm/start_kern.asm" -o "build/start_kern.o"
echo "======================== C COMPILING ========================="
#Tutti i file di manipolazione I/O
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/c/kernel.c" -o "build/kernel.o" -I header -Wall
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/c/stdio.c" -o "build/stdio.o" -I header -Wall
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/c/in_asm.c" -o "build/in_asm.o" -I header -Wall
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/c/string.c" -o "build/string.o" -I header -Wall
#Tutti i file di driver etc
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/Drivers/gdt.c" -o "build/gdt.o" -I Drivers -Wall
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/Drivers/idt.c" -o "build/idt.o" -I Drivers -Wall
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/Drivers/isr.c" -o "build/isr.o" -I Drivers -Wall
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/Drivers/irq.c" -o "build/irq.o" -I Drivers -Wall
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/Drivers/timer.c" -o "build/timer.o" -I Drivers -Wall
i686-elf-gcc -ffreestanding -m32 -O2 -g -c "src/Drivers/keyboard.c" -o "build/keyboard.o" -I Drivers -Wall
echo "======================== LINKING ========================"
i686-elf-ld -o "build/kernel.bin" -Tlinker.ld "build/start_kern.o" "build/in_asm.o" "build/gdt.o" "build/idt.o" "build/isr.o" "build/irq.o" "build/timer.o" "build/keyboard.o" "build/string.o" "build/stdio.o" "build/kernel.o" --oformat=binary
cat "build/boot.bin" "build/kernel.bin" > "OS/NaitOS.img"
echo "======================== EXECUTING ========================"
qemu-system-i386 -drive format=raw,file="OS/NaitOS.img"