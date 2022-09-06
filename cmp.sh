export PREFIX="/mnt/c/Users/cl406/Desktop/Coding/NaitOS/Nait/Toolchain/i686-elf"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

nasm -f bin "src/asm/boot.asm" -o "build/boot.bin"
nasm -f bin "src/asm/fine.asm" -o "build/fine.bin"
nasm -f elf "src/asm/start_kern.asm" -o "build/start_kern.o"

i686-elf-gcc -ffreestanding -m32 -g -c "src/c/kernel.c" -o "build/kernel.o" -Wall
i686-elf-gcc -ffreestanding -m32 -g -c "src/c/stdio.c" -o "build/stdio.o" -Wall
i686-elf-gcc -ffreestanding -m32 -g -c "src/c/in_asm.c" -o "build/in_asm.o" -Wall

i686-elf-ld -o "build/kernel.bin" -Tlinker.ld "build/start_kern.o" "build/in_asm.o" "build/stdio.o" "build/kernel.o" --oformat=binary
cat "build/boot.bin" "build/kernel.bin" > "OS/NaitOS.bin"
qemu-system-i386 -fda "OS/NaitOS.bin"