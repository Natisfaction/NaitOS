nasm -f bin "src/boot.asm" -o "build/boot.bin"
nasm -f bin "src/fine.asm" -o "build/fine.bin"
nasm -f elf "src/start_kern.asm" -o "build/start_kern.o"
i386-elf-gcc -ffreestanding -m32 -g -c "src/kernel.cpp" -o "build/kernel.o"
i386-elf-ld -o "build/kernel.bin" -Ttext 0x1000 "build/start_kern.o" "build/kernel.o" --oformat=binary