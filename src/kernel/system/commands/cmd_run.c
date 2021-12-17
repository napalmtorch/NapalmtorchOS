#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>
#include <kernel/runtime/elf.h>

void CMD_METHOD_RUN(char* input, char** argv, int argc)
{
    uint32_t* len = 0;
    uint8_t* prog_data = vfs_read_bytes(input + 4, &len);
    elf_start(prog_data + 128, len, NULL);
}