#pragma once
#include <lib/types.h>

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10

#define GDT_ACCESSED	0x01
#define GDT_READWRITE	0x02
#define GDT_DIRECTION	0x04
#define GDT_EXEC	0x08
#define GDT_STATIC	0x10
#define GDT_PRESENT	0x20

typedef struct
{
    uint16_t base_low;
    uint16_t selector;
    uint8_t  reserved;
    uint8_t  flags;
    uint16_t base_high;
} PACKED idt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} PACKED desc_ptr_t;

extern void gdt_init();
extern int gdt_set_descriptor();
extern int gdt_add_descriptor(uint8_t id, uint64_t desc);
extern uint64_t gdt_create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);

extern void idt_flush(uint32_t val);

void idt_init();
void idt_set_isrs();
void idt_set_irqs();
void idt_set_descriptor(uint32_t num, uint32_t base, uint16_t sel, uint8_t flags);


extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void irq_syscall();
