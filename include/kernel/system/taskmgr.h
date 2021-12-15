#pragma once
#include <lib/types.h>
#include <kernel/system/thread.h>

void   taskmgr_init(thread_t* init_thread);
void   taskmgr_start();
void   taskmgr_ready_thread(thread_t* thread);
void   taskmgr_unready_thread(thread_t* thread);
bool_t taskmgr_terminate(thread_t* thread);
void   taskmgr_calculate_cpu_usage();
void   taskmgr_schedule();

thread_t** taskmgr_get_threads(uint32_t* count);
thread_t*  taskmgr_get_thread_byid(uint32_t id);
thread_t*  taskmgr_get_current_thread();
double   taskmgr_get_cpu_usage();

void spinlock_lock (spinlock_t *lock);
void spinlock_unlock (spinlock_t *lock);

void pause();
