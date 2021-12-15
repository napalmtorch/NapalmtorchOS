#pragma once
#include <lib/types.h>
#include <kernel/runtime/runtime_exec.h>

#define VM_PAGESTATE_FREE     0
#define VM_PAGESTATE_USED     1
#define VM_PAGESTATE_DISPOSED 2

#define VM_ACCESS_R       0
#define VM_ACCESS_RW      1
#define VM_ACCESS_EXE_R   2
#define VM_ACCESS_EXE_RW  3

#define VM_DEFAULT_MEM    1 * 1024 * 1024
#define VM_PAGE_SIZE      4096

bool_t      runtime_allocate_pages(runtime_t* runtime, uint32_t addr, uint32_t size);
bool_t      runtime_free_page(runtime_t* runtime, runtime_page_t* page);
bool_t      runtime_collect(runtime_t* runtime);

bool_t      runtime_write8(runtime_t* runtime, uint32_t addr, uint8_t data);
bool_t      runtime_write16(runtime_t* runtime, uint32_t addr, uint16_t data);
bool_t      runtime_write32(runtime_t* runtime, uint32_t addr, uint32_t data);
uint8_t     runtime_read8(runtime_t* runtime, uint32_t addr);
uint16_t    runtime_read16(runtime_t* runtime, uint32_t addr);
uint32_t    runtime_read32(runtime_t* runtime, uint32_t addr);