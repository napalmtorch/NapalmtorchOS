#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_CPUINFO(char* input, char** argv, int argc)
{
    vga_write_fg("CPU Usage        :       ", COL4_YELLOW);
    vga_printf("%f", taskmgr_get_cpu_usage());
    vga_writeln("%");
}