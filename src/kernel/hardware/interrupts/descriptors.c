#include <kernel/hardware/interrupts/descriptors.h>

// interrupt descriptor table flag
#define IDT_FLAG 0x8E

// null descriptor entries
const idt_entry_t NULL_IDT_ENTRY = { 0, 0, 0, 0, 0 };
const gdt_entry_t NULL_GDT_ENTRY = { 0, 0, 0, 0, 0, 0 };

// entry tables
idt_entry_t  idt_entries[IDT_COUNT];
gdt_entry_t  gdt_entries[GDT_COUNT];

// table pointers
desc_ptr_t   idt_ptr;
desc_ptr_t   gdt_ptr;

// clear global descriptor table
void gdt_clear() { for (uint8_t  i = 0; i < GDT_COUNT; i++) { gdt_entries[i] = NULL_GDT_ENTRY; } }

// clear interrupt descriptor table
void idt_clear() { for (uint16_t i = 0; i < IDT_COUNT; i++) { idt_entries[i] = NULL_IDT_ENTRY; } }

// initialize global descriptor tables
void gdt_init()
{
    // clear gdt entries
    gdt_clear();

    // set table address and size
    gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_COUNT) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // set gdt entries
    gdt_set();

    // finished
    gdt_flush((uint32_t)&gdt_ptr);
    debug_ok("Initialized GDT entries");
}

// initialize interrupt descriptor tables
void idt_init()
{
    // clear idt entries
    idt_clear();

    // set table address and size
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_COUNT) - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // set isr descriptors
    idt_set_isrs();
    
    // initialize pic controller
    pic_init();

    // set idt descriptors
    idt_set_irqs();
    
    // finished
    idt_flush((uint32_t)&idt_ptr);
    debug_ok("Initialized IDT entries");
}

// set global desciptor table entries
void gdt_set()
{
    gdt_set_descriptor(0, 0, 0, 0, 0);
    gdt_set_descriptor(1, 0, 0xFFFFF, 0x9A, 0xCF);
    gdt_set_descriptor(2, 0, 0xFFFFF, 0x92, 0xCF);
    gdt_set_descriptor(3, 0, 0xFFFFF, 0xFA, 0xCF);
    gdt_set_descriptor(4, 0, 0xFFFFF, 0xF2, 0xCF);
}

// set interrupt descriptor table service routines
void idt_set_isrs()
{
    idt_set_descriptor(0,  (uint32_t)isr0,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(1,  (uint32_t)isr1,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(2,  (uint32_t)isr2,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(3,  (uint32_t)isr3,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(4,  (uint32_t)isr4,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(5,  (uint32_t)isr5,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(6,  (uint32_t)isr6,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(7,  (uint32_t)isr7,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(8,  (uint32_t)isr8,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(9,  (uint32_t)isr9,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(10, (uint32_t)isr10, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(11, (uint32_t)isr11, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(12, (uint32_t)isr12, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(13, (uint32_t)isr13, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(14, (uint32_t)isr14, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(15, (uint32_t)isr15, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(16, (uint32_t)isr16, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(17, (uint32_t)isr17, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(18, (uint32_t)isr18, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(19, (uint32_t)isr19, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(20, (uint32_t)isr20, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(21, (uint32_t)isr21, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(22, (uint32_t)isr22, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(23, (uint32_t)isr23, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(24, (uint32_t)isr24, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(25, (uint32_t)isr25, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(26, (uint32_t)isr26, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(27, (uint32_t)isr27, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(28, (uint32_t)isr28, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(29, (uint32_t)isr29, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(30, (uint32_t)isr30, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(31, (uint32_t)isr31, KERNEL_CS, IDT_FLAG);
}

// set interrupt descriptor table request routines
void idt_set_irqs()
{
    idt_set_descriptor(32, (uint32_t)irq0,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(33, (uint32_t)irq1,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(34, (uint32_t)irq2,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(35, (uint32_t)irq3,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(36, (uint32_t)irq4,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(37, (uint32_t)irq5,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(38, (uint32_t)irq6,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(39, (uint32_t)irq7,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(40, (uint32_t)irq8,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(41, (uint32_t)irq9,  KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(42, (uint32_t)irq10, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(43, (uint32_t)irq11, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(44, (uint32_t)irq12, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(45, (uint32_t)irq13, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(46, (uint32_t)irq14, KERNEL_CS, IDT_FLAG);
    idt_set_descriptor(47, (uint32_t)irq15, KERNEL_CS, IDT_FLAG);

    idt_set_descriptor(128, (uint32_t)irq_syscall, KERNEL_CS, IDT_FLAG);
}

// set interrupt descriptor table entry properties
void idt_set_descriptor(uint32_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_low  = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector  = sel;
    idt_entries[num].reserved  = 0;
    idt_entries[num].flags     = flags;
}

// set global descriptor table entry properties
void gdt_set_descriptor(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;
    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}