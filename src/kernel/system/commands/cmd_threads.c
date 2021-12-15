#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_THREADS(char* input, char** argv, int argc)
{
    uint32_t count = 0;
    thread_t** threads = taskmgr_get_threads(&count);

    term_write_fg("------ ", COL32_DARKGRAY);
    term_write_fg("THREADS", COL32_YELLOW);
    term_writeln_fg(" -------------------------------------------", COL32_DARKGRAY);
    term_writeln_fg("INDEX       ID          STATE  STACK     NAME ", COL32_DARKGRAY);

    for (uint32_t i = 0; i < count; i++)
    {
        if (threads[i] == NULL) { return; }
        term_printf("0x%8x  0x%8x  0x%2x   0x%8x %s\n", i, threads[i]->id, threads[i]->state, threads[i]->stack_size, threads[i]->name);
        //term_printf("THREAD: ID = %d, STACK_ADDR: 0x%8x, STACK_SIZE = 0x%8x, NAME: %s\n", threads[i]->id, (uint32_t)threads[i]->stack, threads[i]->stack_size, threads[i]->name);
    }

    free(threads);
}