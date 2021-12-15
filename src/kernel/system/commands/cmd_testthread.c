#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

int thread_test(thread_t* t)
{
    tlock();
    vga_printf("Hello world\n");
    vga_printf("This is a test thread\n");
    tunlock();
    while (TRUE);
}

void CMD_METHOD_TESTTHREAD(char* input, char** argv, int argc)
{
    tlock();
    thread_t* thread = thread_create(thread_test, 8192);
    debug_info("STRTED");
    tunlock();
}