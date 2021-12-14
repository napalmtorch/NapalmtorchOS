#include <kernel/system/thread.h>
#include <kernel/core/kernel.h>

// threading information
thread_t*  thread_current;
uint32_t   thread_cid = 0;

void thread_exit();

// generate kernel thread using current registers
thread_t* thread_initial()
{
    thread_t* thread = calloc(sizeof(thread_t));
    thread->id = thread_cid++;
    thread_current = thread;
    return thread;
}

// create new thread with specified entry pointer, stack size, and arguments pointer
thread_t* thread_create(thread_entry_t entry, uint32_t stack_size, void* arg)
{
    thread_t* thread = calloc(sizeof(thread_t));
    memset(thread, 0, sizeof(thread_t));
    thread->id = thread_cid++;

    thread->stack = calloc(stack_size);
    thread->stack_size = stack_size;

    uint32_t* s = ((uint32_t)thread->stack + (stack_size - 16));

    *--s = (uint32_t)arg;
    *--s = (uint32_t)&thread_exit;
    *--s = (uint32_t)entry;

    thread->registers.esp    = (uint32_t)s;
    thread->registers.ebp    = 0;
    thread->registers.eflags = 0x200;
    taskmgr_ready_thread(thread);
    debug_info("Created thread: ID = %d, ESP: 0x%8x, ENTRY: 0x%8x", thread->id, thread->registers.esp, (uint32_t)entry);
    return thread;
}

// on thread exit
void thread_exit()
{
    debug_info("Thread exited");
    while (TRUE);
}