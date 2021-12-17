#include <kernel/hardware/common/paging.h>
#include <kernel/core/kernel.h>

static uint32_t* page_directory = 0;
static uint32_t  page_dir_loc   = 0;
static uint32_t* last_page      = 0;

void paging_enable()
{
    asm volatile("mov %%eax, %%cr3": :"a"(page_dir_loc));	
	asm volatile("mov %cr0, %eax");
	asm volatile("orl $0x80000000, %eax");
	asm volatile("mov %eax, %cr0");
}

void paging_init()
{
	debug_info("Setting up page tables");
	page_directory = (uint32_t*)0x400000;
	page_dir_loc   = (uint32_t)page_directory;
	last_page      = (uint32_t *)0x404000;
	for(int i = 0; i < 1024; i++) { page_directory[i] = 0 | 2; }

	paging_map(0, 0);
	paging_map(page_directory, page_directory);
	paging_enable();
	debug_info("Initialized paging");
}

void paging_map(uint32_t v_addr, uint32_t p_addr)
{
	uint16_t id = v_addr >> 22;
	for(int i = 0; i < 1024; i++) { last_page[i] = p_addr | 3; p_addr += 4096; }
	page_directory[id] = ((uint32_t)last_page) | 3;
	last_page = (uint32_t *)(((uint32_t)last_page) + 4096);
	debug_info("Mapped virtual address 0x%8x(ID = %d) to physical address 0x%8x", v_addr, id, p_addr);
}

void paging_unmap(uint32_t v_addr)
{
	debug_info("Un-mapped virtual address 0x%8x(ID = %d)", v_addr);
}