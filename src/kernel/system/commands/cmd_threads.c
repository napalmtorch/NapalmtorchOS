#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_THREADS(char* input, char** argv, int argc)
{
    uint32_t count = 0;
    thread_t** threads = taskmgr_get_threads(&count);

    for (uint32_t i = 0; i < count; i++)
    {
        if (threads[i]->terminated) { continue; }
        vga_printf("THREAD: ID = %d, STACK_ADDR: 0x%8x, STACK_SIZE = %d bytes\n", threads[i]->id, (uint32_t)threads[i]->stack, threads[i]->stack_size);
    }

    free(threads);
}