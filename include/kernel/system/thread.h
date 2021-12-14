#pragma once
#include <lib/types.h>
#include <kernel/hardware/interrupts/interrupts.h>

typedef enum
{
    THREADSTATE_HALTED,
    THREADSTATE_RUNNING,
    THREADSTATE_COMPLETED,
} THREADSTATE;

typedef int (*thread_entry_t)(void*);

typedef struct
{
    uint32_t esp, ebp, eax, ebx, ecx, edx, esi, edi, eflags;
} PACKED thread_regs_t;

typedef struct
{
    thread_regs_t registers;
    uint32_t      id;
    uint32_t*     stack;
    uint32_t      stack_size;
    bool_t        locked;

} thread_t;

struct thread_list;

extern void switch_thread(struct thread_list *next);

thread_t* thread_initial();
thread_t* thread_create(thread_entry_t entry, uint32_t stack_size, void* arg);