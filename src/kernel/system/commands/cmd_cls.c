#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_CLS(char* input, char** argv, int argc)
{
    vga_clear(vga_get_bg());
}