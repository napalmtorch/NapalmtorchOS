#include <kernel/hardware/interrupts/interrupts.h>
#include <kernel/core/kernel.h>

// interrupt handler function pointers
isr_t isr_handlers[IDT_COUNT];

// clear interrupt handler function pointers
void isr_clear() { for (uint32_t i = 0; i < IDT_COUNT; i++) { isr_handlers[i] = NULL; } }

// initialize isr interface
void isr_init()
{
    isr_clear();
    debug_ok("Initialized interrupt services routines");
}

// register interrupt handler routine to specified function pointer
void irq_register(uint8_t num, isr_t callback)
{
    if (callback == NULL) { return; }
    isr_handlers[num] = callback;
}

// un-register interrupt handler routine
void irq_unregister(uint8_t num)
{
    cli();
    isr_handlers[num] = NULL;
    sti();
}

// on isr interrupt from common stub
void isr_handler(registers_t* regs)
{
    debug_info("THREAD: 0x%8x, ID = %d", (uint32_t)taskmgr_get_current_thread(), taskmgr_get_current_thread()->id);
    debug_dumpregs(&taskmgr_get_current_thread()->registers, DEBUGMODE_SERIAL);
    panici(regs->int_no, regs);
}

// on irq interrupt from common stub
void irq_handler(registers_t* regs)
{  
    // execute handler method
    if (isr_handlers[regs->int_no] != NULL) { isr_handlers[regs->int_no](regs);  }

    // send slave eoi if necessary
    if (regs->int_no >= 40) { port_outb(PIC_SLAVE_CMD, PIC_EOI); }

    // send master eio
    port_outb(PIC_MASTER_CMD, PIC_EOI);
}

// seperate interrupt routine for pit - used for task switching
void pit_handler(registers_t* regs)
{
    // send eio
    port_outb(PIC_MASTER_CMD, PIC_EOI);

    // calculate pit timing information
    pit_calculate();

    // perform thread scheduling
    taskmgr_schedule();
}
