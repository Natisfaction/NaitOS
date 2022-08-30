nasm -f bin "src/boot.asm" -o "build/boot.bin"
nasm -f bin "src/fine.asm" -o "build/fine.bin"
nasm -f elf "src/start_kern.asm" -o "build/start_kern.o"
i686-elf-gcc -ffreestanding -m32 -g -c "src/kernel.cpp" -o "build/kernel.o"
i686-elf-ld -o "build/kernel.bin" -Tlinker.ld "C:\i686-elf-tools\lib\gcc\i686-elf\7.1.0\libgcc.a" "build/start_kern.o" "build/kernel.o" --oformat=binary
wsl