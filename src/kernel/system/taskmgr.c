#include <kernel/system/taskmgr.h>
#include <kernel/core/kernel.h>

thread_list_t* ready_queue;
thread_list_t* current_thread;
bool_t         taskmgr_ready;

// initialize task management interface with initial thread
void taskmgr_init(thread_t* init_thread)
{
    current_thread = (thread_list_t*)calloc(sizeof(thread_list_t));
    current_thread->thread = init_thread;
    current_thread->next   = NULL;
    ready_queue            = NULL;
    taskmgr_ready          = FALSE;
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
}

// mark thread as not ready and remove from linked list
void taskmgr_unready_thread(thread_t* thread)
{
    thread_list_t* iterator = ready_queue;

    if (iterator->thread == thread)
    {
        ready_queue = iterator->next;
        free(iterator);
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
}

// lock current thread
void tlock() { current_thread->thread->locked = TRUE; }

// unlock current thread
void tunlock() { current_thread->thread->locked = FALSE; }

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