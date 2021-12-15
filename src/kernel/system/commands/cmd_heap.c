#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_HEAP(char* input, char** argv, int argc)
{
    mm_print_heap(DEBUGMODE_TERM);
}