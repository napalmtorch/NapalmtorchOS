#include <kernel/runtime/runtime_mem.h>
#include <kernel/core/kernel.h>


bool_t runtime_allocate_page(runtime_t* runtime, int index)
{
    if (index < 0 || index >= runtime->page_count_max) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "runtime_allocate_page"); return FALSE; }
    runtime->pages[index].state = VM_PAGESTATE_USED;
    if (runtime->pages[index].data != NULL) { return FALSE; }
    runtime->pages[index].data = tcalloc(VM_PAGE_SIZE, MEMSTATE_VMPAGE);
    debug_info("Allocated page: ID = 0x%4x, ADDR = 0x%8x, STATE = 0x%2x", runtime->pages[index].index, runtime->pages[index].address, runtime->pages[index].state);
    return TRUE;
}

bool_t runtime_free_page(runtime_t* runtime, runtime_page_t* page)
{
    if (!runtime_validate(runtime)) { return FALSE; }
    page->state = VM_PAGESTATE_DISPOSED;
    return runtime_collect(runtime);
}

bool_t runtime_collect(runtime_t* runtime)
{
    if (!runtime_validate(runtime)) { return FALSE; }

    for (uint32_t i = 0; i < runtime->page_count_max; i++)
    {
        if (runtime->pages[i].index >= runtime->page_count_max) { continue; }
        if (runtime->pages[i].state == VM_PAGESTATE_DISPOSED)
        {
            if (runtime->pages[i].data != NULL) { free(runtime->pages[i].data); runtime->pages[i].data = NULL; }
            runtime->pages[i].state = VM_PAGESTATE_FREE;
        }
    }

    return TRUE;
}

int runtime_index_from_address(runtime_t* runtime, uint32_t addr)
{
    if (!runtime_validate(runtime)) { return -1; }
    //if (addr >= runtime->page_count_max * VM_PAGE_SIZE) { panic(KRESULT_VM_INVALIDADDRESS, NULL); return -1; }
    for (uint32_t i = 0; i < runtime->page_count_max; i++)
    {
        if (addr >= runtime->pages[i].address && addr < runtime->pages[i].address + VM_PAGE_SIZE) { return i; }
    }
    return -1;
}

bool_t runtime_write8(runtime_t* runtime, uint32_t addr, uint8_t data)
{
    if (!runtime_validate(runtime)) { return FALSE; }
    int i = runtime_index_from_address(runtime, addr);
    if (i < 0 || i >= runtime->page_count_max) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "runtime_write8"); return FALSE; }
    if (runtime->pages[i].data == NULL) { if (!runtime_allocate_page(runtime, i)) { return FALSE; } }
    uint32_t offset = addr - runtime->pages[i].address;
    runtime->pages[i].data[offset] = data;
    return TRUE;
}

bool_t runtime_write16(runtime_t* runtime, uint32_t addr, uint16_t data)
{
    if (!runtime_write8(runtime, addr + 1, (uint8_t)((data & 0xFF00) >> 8))) { return FALSE; }
    if (!runtime_write8(runtime, addr + 0, (uint8_t)((data & 0x00FF)))) { return FALSE; }
    return TRUE;
}

bool_t runtime_write32(runtime_t* runtime, uint32_t addr, uint32_t data)
{
    if (!runtime_validate(runtime)) { return FALSE; }
    if (!runtime_write8(runtime, addr + 3, (uint8_t)((data & 0xFF000000) >> 24))) { return FALSE; }
    if (!runtime_write8(runtime, addr + 2, (uint8_t)((data & 0x00FF0000) >> 16))) { return FALSE; }
    if (!runtime_write8(runtime, addr + 1, (uint8_t)((data & 0x0000FF00) >> 8))) { return FALSE; }
    if (!runtime_write8(runtime, addr + 0, (uint8_t)((data & 0xFFFFFFFF)))) { return FALSE; }
    return TRUE;
}

uint8_t runtime_read8(runtime_t* runtime, uint32_t addr)
{
    if (!runtime_validate(runtime)) { return 0; }
    int i = runtime_index_from_address(runtime, addr);
    if (i < 0 || i >= runtime->page_count_max) {  panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "runtime_read8"); return FALSE; }
    if (runtime->pages[i].data == NULL) { return 0; }
    uint32_t offset = addr - runtime->pages[i].address;
    return runtime->pages[i].data[offset];
}

uint16_t runtime_read16(runtime_t* runtime, uint32_t addr)
{
    if (!runtime_validate(runtime)) { return 0; }
    uint16_t val = (uint16_t)((runtime_read8(runtime, addr + 1) << 8) | runtime_read8(runtime, addr));
    return val;
}

uint32_t runtime_read32(runtime_t* runtime, uint32_t addr)
{
    if (!runtime_validate(runtime)) { return 0; }
    uint32_t val = (uint32_t)((runtime_read8(runtime, addr + 3) << 24) | (runtime_read8(runtime, addr + 2) << 16) | (runtime_read8(runtime, addr + 1) << 8) | runtime_read8(runtime, addr));
    return val;
}