#pragma once
#include <lib/types.h>
#include <kernel/hardware/interrupts/interrupts.h>
#include <kernel/runtime/runtime_exec.h>

#define SPINLOCK_LOCKED 0
#define SPINLOCK_UNLOCKED 1

typedef volatile uint32_t spinlock_t;

typedef enum
{
    THREADSTATE_HALTED,
    THREADSTATE_RUNNING,
    THREADSTATE_TERMINATED,
} THREADSTATE;

typedef int (*thread_entry_t)(void*);

typedef struct
{
    uint32_t esp, ebp, eax, ebx, ecx, edx, esi, edi, eflags;
} PACKED thread_regs_t;

typedef struct
{
    uint32_t ticks, ticks_per_second, tps_old;
    uint64_t ticks_total;
    uint32_t seconds_total;
    uint32_t time, last_time;
    double   cpu_usage;
} PACKED thread_time_t;

typedef struct
{
    thread_regs_t registers;
    uint32_t      id;
    uint32_t*     stack;
    uint32_t      stack_size;
    THREADSTATE   state;
    thread_time_t time;
    spinlock_t    lock;
    runtime_t*    runtime;
    char          name[64];
    uint32_t      exit_code;
} PACKED thread_t;

extern void switch_thread();

thread_t* thread_initial();
thread_t* thread_create(const char* name, thread_entry_t entry, uint32_t stack_size, void* arg);
void      thread_monitor(thread_t* thread);