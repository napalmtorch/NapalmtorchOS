#include <kernel/hardware/interrupts/descriptors.h>
#include <kernel/core/kernel.h>

extern void set_gdtr();
extern void reload_segments();

static uint32_t gdt_pointer = 0;
static uint32_t  gdt_size = 0;
static uint32_t gdtr_loc = 0;

static uint32_t highpart = 0;
static uint32_t lowpart = 0;

void gdt_init()
{
	gdt_pointer = 0x806; // start GDT data at 4MB
	debug_printf("location of GDT: 0x%x\n", gdt_pointer);
	gdtr_loc =    0x800;
	debug_printf("location of GDTR: 0x%x\n", gdtr_loc);
	gdt_add_descriptor(0, 0);
	gdt_add_descriptor(1, 0x00CF9A000000FFFF);
	gdt_add_descriptor(2, 0x00CF92000000FFFF);
	gdt_add_descriptor(3, 0x008FFA000000FFFF); // 16bit code pl3
	gdt_set_descriptor(4, 0x008FF2000000FFFF); // 16bit data pl3
	debug_printf("Global Descriptor Table is alive.\n");
}

int gdt_set_descriptor()
{
	/* GDTR
	 * 0-1 = SIZE - 1
	 * 2-5 = OFFSET
	 */
	*(uint16_t*)gdtr_loc = (gdt_size - 1) & 0x0000FFFF;
	gdtr_loc += 2;
	*(uint32_t*)gdtr_loc = gdt_pointer;
	set_gdtr();
	debug_printf("GDTR was set. gdtr.size=%d gdtr.offset=0x%x\n", 
		*(uint16_t*)(gdtr_loc-2) + 1, 
		*(uint32_t*)gdtr_loc);
	reload_segments();
	debug_printf("Segments reloaded.\n");
	return 0;
}

int gdt_add_descriptor(uint8_t id, uint64_t desc)
{
	uint32_t loc = gdt_pointer + sizeof(uint64_t)*id;
	*(uint64_t*)loc = desc;
	debug_printf("Added entry %d = 0x%x << 32 | 0x%x\n", id, (*(uint64_t*)loc) >> 32, *(uint32_t*)loc+4);
	gdt_size += sizeof(desc);
	return 0;
}

uint64_t gdt_create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
	uint64_t desc = 0;
	highpart = 0;
	lowpart = 0;
	desc = limit 		& 0x000F0000;
	desc |= (flag << 8) 	& 0x00F0FF00;
	desc |= (base >> 16) 	& 0x000000FF;
	desc |= base		& 0xFF000000;
	
	highpart = desc;
	desc <<= 32;

	desc |= base << 16;
	desc |= limit		& 0x0000FFFF;
	lowpart = (uint32_t)desc;
	return desc;
}

// interrupt descriptor table flag
#define IDT_FLAG 0x8E

// null descriptor entries
const idt_entry_t NULL_IDT_ENTRY = { 0, 0, 0, 0, 0 };

// entry tables
idt_entry_t  idt_entries[256];

// table pointers
desc_ptr_t   idt_ptr;
desc_ptr_t   gdt_ptr;

// clear interrupt descriptor table
void idt_clear() { for (uint16_t i = 0; i < 256; i++) { idt_entries[i] = NULL_IDT_ENTRY; } }


// initialize interrupt descriptor tables
void idt_init()
{
    // clear idt entries
    idt_clear();

    // set table address and size
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
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