#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_CPUINFO(char* input, char** argv, int argc)
{
    term_write_fg("CPU Usage        :       ", COL32_YELLOW);
    term_printf("%f", taskmgr_get_cpu_usage());
    term_writeln("%");
}