#pragma once
#include <lib/types.h>
#include <kernel/hardware/interrupts/interrupts.h>

typedef struct
{
    uint32_t a, b, c, d;
} PACKED syscall_args_t;

typedef void (*syscall_method_t)(syscall_args_t args);

typedef struct
{
    uint32_t         code;
    const char*      name;
    syscall_method_t m_execute;
} PACKED syscall_t;

void SYSCALL_METHOD_LOADMETHODS(syscall_args_t args);

static const syscall_t SYSCALL_LOADMETHODS = { 0xFF, "LOAD METHODS", SYSCALL_METHOD_LOADMETHODS };

static const syscall_t* SYSTEM_CALLS[] = 
{
    &SYSCALL_LOADMETHODS,
};
static const uint32_t SYSCALL_COUNT = sizeof(SYSTEM_CALLS) / sizeof(syscall_t*);

void syscall_execute(registers_t* regs);