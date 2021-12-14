#pragma once
#include <lib/types.h>

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, old_esp, ss; 

} PACKED registers_t;

typedef void (*isr_t)(registers_t*);

static inline void cli() { asm volatile("cli"); }
static inline void sti() { asm volatile("sti"); }

void irq_register(uint8_t num, isr_t callback);
void irq_unregister(uint8_t num);

void isr_handler(registers_t* regs);
void irq_handler(registers_t* regs);
void pit_handler(registers_t* regs);