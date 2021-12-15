#include <kernel/system/taskmgr.h>
#include <kernel/core/kernel.h>

#define TASKMGR_MAX 4096

thread_t** taskmgr_list;
thread_t*  taskmgr_current;
thread_t*  taskmgr_next;
bool_t     taskmgr_ready;
uint32_t   taskmgr_count;
uint32_t   taskmgr_index;
double     taskmgr_cpu_usage;

// initialize task management interface with initial thread
void taskmgr_init(thread_t* init_thread)
{
    taskmgr_list           = tcalloc(sizeof(thread_t*) * TASKMGR_MAX, MEMSTATE_PTRARRAY);
    taskmgr_list[0]        = init_thread;
    taskmgr_count++;
    taskmgr_current        = init_thread;
    taskmgr_ready          = FALSE;
    taskmgr_next           = NULL;
    taskmgr_count          = 1;
    taskmgr_index          = 0;
}

// allow task switching to happen
void taskmgr_start() { taskmgr_ready = TRUE; }

int taskmgr_free_index()
{
    for (uint32_t i = 0; i < TASKMGR_MAX; i++)
    {
        if (taskmgr_list[i] == NULL) { return i; }
    }
    return -1;
}

// mark thread as ready and add to linked list
void taskmgr_ready_thread(thread_t* thread)
{
    cli();
    int index = taskmgr_free_index();
    if (index < 0 || index >= TASKMGR_MAX) { panicf(EXCEPTION_INDEX_OUTOFRANGE, NULL, "taskmgr_ready_thread"); return; }
    taskmgr_list[index] = thread;
    taskmgr_count++;
    thread->state = THREADSTATE_RUNNING;
    debug_info("Loaded thread %d", thread->id);
    sti();
}

// mark thread as not ready and remove from linked list
void taskmgr_unready_thread(thread_t* thread)
{
    for (uint32_t i = 0; i < TASKMGR_MAX; i++)
    {
        if (taskmgr_list[i] == NULL) { continue; }
        if (taskmgr_list[i] == thread)
        {
            
            debug_info("Unloaded thread");
            taskmgr_count--;
            taskmgr_current = NULL;
            taskmgr_next = NULL;
            taskmgr_index = 0;
            
            return;
        }
    }
}

bool_t taskmgr_terminate(thread_t* thread)
{
    cli();
    for (uint32_t i = 0; i < TASKMGR_MAX; i++)
    {
        if (taskmgr_list[i] == NULL) { continue; }
        if (taskmgr_list[i] == thread)
        {
            taskmgr_list[i]->state = THREADSTATE_TERMINATED;
            sti();
            return TRUE;
        }
    }
    sti();
    return FALSE;
}

uint32_t total_tps = 0;
uint32_t total_tps_old = 0;

void taskmgr_calculate_cpu_usage()
{
    total_tps = 0;
    total_tps_old = 0;

    for (uint32_t i = 0; i < TASKMGR_MAX; i++)
    {
        if (taskmgr_list[i] == NULL) { continue; }
        if (taskmgr_list[i] == idle_get_thread()) { continue; }
        total_tps_old += taskmgr_list[i]->time.tps_old;
        total_tps += taskmgr_list[i]->time.ticks_per_second;
    }

    if (total_tps == 0) { return; }
    if (idle_get_thread()->time.ticks_per_second == 0) { return; }
    
    //long double idle_time = (long double)idle_get_thread()->time.ticks_per_second - (long double)idle_get_thread()->time.tps_old;
    //idle_time = idle_time / (long double)((long double)total_tps - (long double)total_tps_old);
    long double idle_time = (long double)total_tps / (long double)idle_get_thread()->time.ticks_per_second;
    double cpu_usage = 100.0 - (idle_time * 100.0);

    if (cpu_usage <= 0.0) { cpu_usage = 0.0; }
    if (cpu_usage >= 100.0) { cpu_usage = 100.0; }
    taskmgr_cpu_usage = cpu_usage;
}

// update current thread and perform context switch
void taskmgr_schedule()
{
    // validate
    if (!taskmgr_ready) { return; }
    
    taskmgr_current = taskmgr_list[taskmgr_index];
    if (taskmgr_current == NULL) { taskmgr_index = 0; return; }

    taskmgr_index++;
    while (taskmgr_list[taskmgr_index] == NULL) { taskmgr_index++; }
    if (taskmgr_index >= TASKMGR_MAX) { taskmgr_index = 0; }
    taskmgr_next = taskmgr_list[taskmgr_index];

    if (taskmgr_next->state == THREADSTATE_HALTED) { taskmgr_schedule(); return; }
    else if (taskmgr_next->state == THREADSTATE_TERMINATED)
    {
        free(taskmgr_next->stack);
        free(taskmgr_next);
        taskmgr_list[taskmgr_index] = NULL;
        taskmgr_next = taskmgr_list[0];
        taskmgr_index = 0;
    }

    switch_thread();
}

thread_t** taskmgr_get_threads(uint32_t* count)
{
    thread_t** thread_list = tcalloc(taskmgr_count * sizeof(thread_t*), MEMSTATE_PTRARRAY);
    memcpy(thread_list, taskmgr_list, taskmgr_count * sizeof(thread_t*));
    *count = taskmgr_count;
    return thread_list;
}

thread_t* taskmgr_get_thread_byid(uint32_t id)
{
    for (uint32_t i = 0; i < TASKMGR_MAX; i++)
    {
        if (taskmgr_list[i] == NULL) { continue; }
        if (taskmgr_list[i]->id == id)
        {
            return taskmgr_list[i];
        }
    }
    return NULL;
}

thread_t*  taskmgr_get_current_thread() { return taskmgr_current; }

double taskmgr_get_cpu_usage() { return taskmgr_cpu_usage; }

static uint32_t atomic_test_and_set (volatile spinlock_t *lock) 
{
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
}

void spinlock_unlock (spinlock_t *lock)
{
    *lock = 0;
}