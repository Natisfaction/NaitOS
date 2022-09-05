export PREFIX="/mnt/c/Users/cl406/Desktop/Coding/NaitOS/Nait/Toolchain/i686-elf"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

nasm -f bin "src/boot.asm" -o "build/boot.bin"
nasm -f bin "src/fine.asm" -o "build/fine.bin"
nasm -f elf "src/start_kern.asm" -o "build/start_kern.o"

i686-elf-gcc -ffreestanding -m32 -g -c "src/kernel.c" -o "build/kernel.o" -Wall
i686-elf-gcc -ffreestanding -m32 -g -c "src/basic.c" -o "build/basic.o" -Wall

i686-elf-ld -o "build/kernel.bin" -Ttext 0x1000 "build/start_kern.o" "build/basic.o" "build/kernel.o" --oformat=binary
cat "build/boot.bin" "build/kernel.bin" > "OS/NaitOS.bin"
qemu-system-i386 -fda "OS/NaitOS.bin"