#include <kernel/system/taskmgr.h>
#include <kernel/core/kernel.h>

thread_list_t* ready_queue;
thread_list_t* current_thread;
thread_list_t* taskmgr_list;
bool_t         taskmgr_ready;
uint32_t       taskmgr_count;
double         taskmgr_cpu_usage;

// initialize task management interface with initial thread
void taskmgr_init(thread_t* init_thread)
{
    current_thread = (thread_list_t*)calloc(sizeof(thread_list_t));
    current_thread->thread = init_thread;
    taskmgr_list           = current_thread;
    current_thread->next   = NULL;
    ready_queue            = NULL;
    taskmgr_ready          = FALSE;
    taskmgr_count          = 1;
}

// allow task switching to happen
void taskmgr_start() { taskmgr_ready = TRUE; }

// mark thread as ready and add to linked list
void taskmgr_ready_thread(thread_t* thread)
{
    thread_list_t* item = (thread_list_t*)calloc(sizeof(thread_list_t));
    item->thread = thread;
    item->next   = NULL;

    if (!ready_queue) { ready_queue = item; }
    else
    {
        thread_list_t* iterator = ready_queue;
        while (iterator->next) { iterator = iterator->next; }
        iterator->next = item;
    }

    taskmgr_count++;
}

// mark thread as not ready and remove from linked list
void taskmgr_unready_thread(thread_t* thread)
{
    thread_list_t* iterator = ready_queue;

    if (iterator->thread == thread)
    {
        ready_queue = iterator->next;
        free(iterator);
        taskmgr_count--;
        return;
    }

    while (iterator->next)
    {
        if (iterator->next->thread == thread)
        {
            thread_list_t *tmp = iterator->next;
            iterator->next = tmp->next;
            free(tmp);
        }
        iterator = iterator->next;
    }

    taskmgr_count--;
}

// lock current thread
void tlock() { current_thread->thread->locked = TRUE; }

// unlock current thread
void tunlock() { current_thread->thread->locked = FALSE; }

void taskmgr_calculate_cpu_usage()
{
    uint32_t total_tps;

    thread_list_t* iterator = taskmgr_list;
    while (iterator->next) 
    { 
        if (iterator->thread)
        {
            if (iterator->thread != idle_get_thread()) { total_tps += iterator->thread->time.ticks_per_second; }
        }

        if (iterator->next)
        {
            if (iterator->next->thread != idle_get_thread()) { total_tps += iterator->next->thread->time.ticks_per_second; }
        }
        iterator = iterator->next; 
    }

    if (total_tps == 0) { taskmgr_cpu_usage = 0; return; }
    if (idle_get_thread()->time.ticks_per_second == 0) { taskmgr_cpu_usage = 0; return; }
    double cpu_usage = 100.0 - ((double)(total_tps / 1000) / (double)(idle_get_thread()->time.ticks_per_second / 1000) * 100.0);
    
    if (cpu_usage < 0.0) { cpu_usage = 0.0; }
    if (cpu_usage > 100.0) { cpu_usage = 100.0; }
    taskmgr_cpu_usage = cpu_usage;
}

// update current thread and perform context switch
void taskmgr_schedule()
{
    // validate
    if (!taskmgr_ready) { return; }
    if (!ready_queue) { return; }

    // check if current thread is locked
    if (current_thread->thread->locked) { return; }

    // iterate through linked list
    thread_list_t *iterator = ready_queue;
    while (iterator->next) { iterator = iterator->next; }
    iterator->next = current_thread;
    current_thread->next = 0;

    // switch to new thread
    thread_list_t* new_thread = ready_queue;
    ready_queue = ready_queue->next;
    switch_thread(new_thread);
}

thread_t** taskmgr_get_threads(uint32_t* count)
{
    thread_t** output = tcalloc(sizeof(thread_t*) * taskmgr_count, MEMSTATE_PTRARRAY);
    thread_list_t* iterator = taskmgr_list;
    int i = 0;
    while (iterator->next) 
    { 
        if (iterator->thread) { output[i] = iterator->thread; i++; }
        if (iterator->next)   { output[i] = iterator->next->thread; i++; }
        iterator = iterator->next; 
    }

    *count = taskmgr_count;
    return output;
}

double taskmgr_get_cpu_usage() { return taskmgr_cpu_usage; }

static uint32_t atomic_test_and_set (volatile spinlock_t *lock) {
	register spinlock_t value = SPINLOCK_UNLOCKED;
	
	asm volatile("lock					\
								xchgl	%0, %1"
								: "=q" (value), "=m" (*lock)
								: "0" (value));
	
  return value;
}

void spinlock_lock (spinlock_t *lock)
{
  while (atomic_test_and_set (lock) == SPINLOCK_LOCKED);
  /*{
    sleep();
  }*/
}

void spinlock_unlock (spinlock_t *lock)
{
  *lock = 0;
}

