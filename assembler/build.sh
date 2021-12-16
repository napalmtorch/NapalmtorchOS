clear

rm -r "bin"
mkdir "bin"

# main
gcc -ggdb -m32 -Iinclude -c "src/main.c" -o "bin/main.o" -Wall -Wno-unused-variable -fsanitize=undefined,address

# utility
gcc -ggdb -m32 -Iinclude -c "src/utility/debug.c" -o "bin/debug.o" -Wall -Wno-unused-variable -fsanitize=undefined,address
gcc -ggdb -m32 -Iinclude -c "src/utility/string.c" -o "bin/string.o" -Wall -Wno-unused-variable -fsanitize=undefined,address
gcc -ggdb -m32 -Iinclude -c "src/utility/vector.c" -o "bin/vector.o" -Wall -Wno-unused-variable -fsanitize=undefined,address
gcc -ggdb -m32 -Iinclude -c "src/utility/stream.c" -o "bin/stream.o" -Wall -Wno-unused-variable -fsanitize=undefined,address

# core
gcc -ggdb -m32 -Iinclude -c "src/core/tokenizer.c" -o "bin/tokenizer.o" -Wall -Wno-unused-variable -fsanitize=undefined,address
gcc -ggdb -m32 -Iinclude -c "src/core/util.c" -o "bin/util.o" -Wall -Wno-unused-variable -fsanitize=undefined,address

# link
gcc -ggdb -m32 -o "bin/assembler" "bin/main.o" "bin/debug.o" "bin/string.o" "bin/vector.o" "bin/stream.o" "bin/tokenizer.o" "bin/util.o" -Wall -Wno-unused-variable -fsanitize=undefined,address

# run
./bin/assembler tests/test2.asm
