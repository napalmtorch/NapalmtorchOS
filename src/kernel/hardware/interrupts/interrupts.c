#include <kernel/hardware/interrupts/interrupts.h>
#include <kernel/core/kernel.h>

isr_t isr_handlers[IDT_COUNT];

void isr_clear() { for (uint32_t i = 0; i < IDT_COUNT; i++) { isr_handlers[i] = NULL; } }

void isr_init()
{
    isr_clear();
    debug_ok("Initialized interrupt services routines");
}

void irq_register(uint8_t num, isr_t callback)
{
    if (callback == NULL) { return; }
    isr_handlers[num] = callback;
}

void irq_unregister(uint8_t num)
{
    cli();
    isr_handlers[num] = NULL;
    sti();
}

void isr_handler(registers_t* regs)
{
    panici(regs->int_no, regs);
}

void irq_handler(registers_t* regs)
{  
    if (isr_handlers[regs->int_no] != NULL) { isr_handlers[regs->int_no](regs);  }

    if (regs->int_no >= 40) { port_outb(PIC_SLAVE_CMD, PIC_EOI); }
    port_outb(PIC_MASTER_CMD, PIC_EOI);
}

void pit_handler(registers_t* regs)
{
    port_outb(PIC_MASTER_CMD, PIC_EOI);
    pit_calculate();
    taskmgr_schedule();
}
