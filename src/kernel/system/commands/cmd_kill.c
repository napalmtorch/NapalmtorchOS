#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_KILL(char* input, char** argv, int argc)
{
    if (argc == 2)
    {
        uint32_t id = atol(argv[1]);
        thread_t* thread = taskmgr_get_thread_byid(id);
        if (thread == NULL) { vga_printf("No thread running with id %d\n", id); return; }
        if (!taskmgr_terminate(thread)) { vga_printf("Unable to terminate thread %d\n", id); return; }
        vga_write_fg("Terminated thread ", COL4_GREEN);
        vga_printf("%d\n", id);
        return;
    }

   vga_writeln_fg("Invalid arguments", COL4_RED);
}