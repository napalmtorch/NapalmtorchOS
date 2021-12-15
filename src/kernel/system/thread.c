#include <kernel/system/thread.h>
#include <kernel/core/kernel.h>

extern uint32_t stack_top;
extern uint32_t stack_bottom;

// threading information
uint32_t   thread_cid = 0;

void thread_exit(thread_t* thread);

// generate kernel thread using current registers
thread_t* thread_initial()
{
    thread_t* thread = calloc(sizeof(thread_t));
    thread->id = thread_cid++;
    thread->stack = &stack_top;
    thread->stack_size = &stack_top - &stack_bottom;
    thread->registers.eflags = 0x200;
    thread->state = THREADSTATE_RUNNING;
    return thread;
}

// create new thread with specified entry pointer, stack size, and arguments pointer
thread_t* thread_create(const char* name, thread_entry_t entry, uint32_t stack_size, void* arg)
{
    thread_t* thread = tcalloc(sizeof(thread_t), MEMSTATE_THREAD);

    // copy name
    for (uint32_t i = 0; i < strlen(name); i++) { if (i < 64) { thread->name[i] = name[i]; } }
    
    thread->id = thread_cid++;
    thread->stack = calloc(stack_size);
    thread->stack_size = stack_size;
    thread->state = THREADSTATE_HALTED;
    thread->runtime = NULL;

    uint32_t* s = ((uint32_t)thread->stack + (stack_size - 16));

    *--s = (uint32_t)thread;
    *--s = (uint32_t)arg;
    *--s = (uint32_t)thread_exit;
    *--s = (uint32_t)entry;

    thread->registers.esp    = (uint32_t)s;
    thread->registers.ebp    = 0; //(uint32_t)thread->stack + thread->stack_size;
    thread->registers.eflags = 0x200;
    debug_info("Created thread: ID = %d, ESP: 0x%8x, ENTRY: 0x%8x", thread->id, thread->registers.esp, (uint32_t)entry);
    return thread;
}

// on thread exit
void thread_exit(thread_t* thread)
{
    cli();
    spinlock_lock(&thread->lock);
    register int code asm ("eax");
    thread->exit_code = code;
    thread->state = THREADSTATE_TERMINATED;
    spinlock_unlock(&thread->lock);
    sti();
    while (TRUE);
}

// monitor thread and update time values
void thread_monitor(thread_t* thread)
{
    if (thread == NULL) { panicf(EXCEPTION_NULLPTR, NULL, "thread_monitor"); return; }

    thread->time.ticks++;
    thread->time.ticks_total++;
    thread->time.time = pit_get_seconds_total();
    if (thread->time.time != thread->time.last_time)
    {
        thread->time.tps_old = thread->time.ticks_per_second;
        thread->time.seconds_total++;
        thread->time.ticks_per_second = thread->time.ticks;
        thread->time.ticks = 0;
        thread->time.last_time = thread->time.time;
    }
}