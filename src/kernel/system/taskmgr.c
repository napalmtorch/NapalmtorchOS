#include <kernel/system/taskmgr.h>
#include <kernel/core/kernel.h>

thread_list_t* ready_queue;
thread_list_t* current_thread;
bool_t         taskmgr_ready;

void taskmgr_init(thread_t* init_thread)
{
    current_thread = (thread_list_t*)calloc(sizeof(thread_list_t));
    current_thread->thread = init_thread;
    current_thread->next   = NULL;
    ready_queue            = NULL;
    taskmgr_ready          = FALSE;
}

void taskmgr_start() { taskmgr_ready = TRUE; }

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

void tlock()
{
    current_thread->thread->locked = TRUE;
}

void tunlock()
{
    current_thread->thread->locked = FALSE;
}

void taskmgr_schedule()
{
    if (!taskmgr_ready) { return; }
    if (!ready_queue) { return; }
    if (current_thread->thread->locked) { return; }

    thread_list_t *iterator = ready_queue;
    while (iterator->next) { iterator = iterator->next; }

    iterator->next = current_thread;

    current_thread->next = 0;
    thread_list_t* new_thread = ready_queue;

    ready_queue = ready_queue->next;

    switch_thread(new_thread);
}