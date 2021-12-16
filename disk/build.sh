nasm -f bin "disk/testprog.asm" -o "disk/testprog.bin"

i686-elf-gcc -w -Iinclude -c "disk/test.c" -oformat=binary -o "disk/test.o" -nostdlib -ffreestanding -Wall -Wextra -fPIC