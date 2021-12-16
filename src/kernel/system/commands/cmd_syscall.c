#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_SYSCALL(char* input, char** argv, int argc)
{
    asm volatile("int $0x80");
}
