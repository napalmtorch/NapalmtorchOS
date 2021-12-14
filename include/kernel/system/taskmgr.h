#pragma once
#include <lib/types.h>
#include <kernel/system/thread.h>

typedef struct thread_list
{
    thread_t* thread;
    struct thread_list* next;
} PACKED thread_list_t;

extern thread_list_t* current_thread;

void taskmgr_init(thread_t* init_thread);
void taskmgr_start();
void taskmgr_ready_thread(thread_t* thread);
void taskmgr_unready_thread(thread_t* thread);
void taskmgr_calculate_cpu_usage();
void taskmgr_schedule();

thread_t** taskmgr_get_threads(uint32_t* count);
double   taskmgr_get_cpu_usage();

void tlock();
void tunlock();