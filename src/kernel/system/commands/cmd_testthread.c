#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

spinlock_t test_lock;
char* test_malloc;

int thread_test(thread_t* t)
{
    spinlock_lock(&test_lock);
    if (test_malloc != NULL) { memset(test_malloc, 0, 2 * 1024 * 1024); }
    term_printf("Hello world\n");
    term_printf("This is a test thread\n");
    
    spinlock_unlock(&test_lock);
    return 0xFF;
}

void CMD_METHOD_TESTTHREAD(char* input, char** argv, int argc)
{
    thread_t* thread = thread_create("test", thread_test, 8192, 0);
    if (test_malloc == NULL) { test_malloc = malloc(2 * 1024 * 1024); }
    taskmgr_ready_thread(thread);
}