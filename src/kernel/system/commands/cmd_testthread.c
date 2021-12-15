#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

spinlock_t test_lock;

int thread_test(void* arg)
{
    spinlock_lock(&test_lock);
    term_printf("Hello world\n");
    term_printf("This is a test thread\n"); 
    spinlock_unlock(&test_lock);
    return 32;
}

void CMD_METHOD_TESTTHREAD(char* input, char** argv, int argc)
{
    thread_t* thread = thread_create("test", thread_test, 8192, 0);
    taskmgr_ready_thread(thread);
}