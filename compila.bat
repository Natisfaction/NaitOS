nasm -f bin "src/boot.asm" -o "build/boot.bin"
nasm -f bin "src/fine.asm" -o "build/fine.bin"
nasm -f elf "src/start_kern.asm" -o "build/start_kern.o"
C:\Users\cl406\Desktop\Coding\NaitOS\gcc-i686\bin\i686-elf-gcc -ffreestanding -m32 -g -c "src/kernel.cpp" -o "build/kernel.o"
C:\Users\cl406\Desktop\Coding\NaitOS\gcc-i686\bin\i686-elf-ld -o "build/kernel.bin" -Tlinker.ld -nostdlib "C:\i686-elf-tools\lib\gcc\i686-elf\7.1.0\libgcc.a" "build/start_kern.o" "build/kernel.o" --oformat=binary