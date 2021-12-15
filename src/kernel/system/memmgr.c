#include <kernel/system/memmgr.h>
#include <kernel/core/kernel.h>

// usage memory region
uint32_t mm_usable_start;
uint32_t mm_usable_end;

// heap table
heap_info_t   mm_info;
heap_entry_t* mm_table;
bool_t        mm_messages;

// local methods
heap_entry_t* mm_allocate_entry(uint32_t size);
heap_entry_t* mm_get_nearest(heap_entry_t* entry);
bool_t        mm_merge_nearest();
heap_entry_t* mm_create_entry(void* ptr, uint32_t len, uint8_t state);
bool_t        mm_delete_entry(heap_entry_t* entry);
bool_t        mm_validate_index(int index);
bool_t        mm_validate_ptr(void* ptr);
bool_t        mm_validate_entry(heap_entry_t* entry, bool_t err);
bool_t        mm_check_index_null(int index);
bool_t        mm_entry_equals(heap_entry_t* a, heap_entry_t* b);
int           mm_free_index();

void mm_init()
{
    // enable messages by default
    mm_messages = FALSE;

    // read memory map
    mm_mmap_read();

    // setup table properties
    mm_info.base      = mm_usable_start;
    mm_info.count_max = 65536;
    mm_info.count     = 0;
    mm_info.length    = mm_align(mm_info.count_max * sizeof(heap_entry_t));
    mm_info.end       = mm_info.base + mm_info.length;
    mm_table          = (heap_entry_t*)mm_info.base;
    memset((void*)mm_info.base, 0, mm_info.length);
    // setup data properties
    mm_info.data_base   = (mm_info.base + mm_info.length) + MEM_ALIGN;
    mm_info.data_end    = mm_usable_end;
    mm_info.data_length = mm_info.data_end - mm_info.data_base;
    mm_info.data_used   = 0;
    //memset((void*)mm_info.data_base, 0, mm_info.data_length);

    // create mass entry
    mm_create_entry(mm_info.data_base, mm_info.data_length, MEMSTATE_FREE);

    // finished
    debug_ok("Initialized memory manager - %d MB free", mm_amount_free() / 1024 / 1024);
}

// read memory map from multiboot header
void mm_mmap_read()
{
    // loop through memory map entries
    for (uint32_t i = 0; i < mboot_get().mmap_len; i += sizeof(mmap_entry_t))
    {
        // get entry from offset
        mmap_entry_t* entry = (mmap_entry_t*)(mboot_get().mmap_addr + i);

        // located kernel-space entry
        if (entry->type == 1 && i > 0)
        {
            // calculate usage memory and return
            mm_usable_start = (kernel_address_end() & 0xFFFFF000) + MEM_ALIGN;
            mm_usable_end = entry->address_low + entry->length_low;
            kernel_size = kernel_address_end() - entry->address_low;
        }
    }
}

void mm_print_heap(DEBUGMODE mode)
{
    DEBUGMODE old = debug_getmode();
    debug_setmode(mode);

    debug_write_col("------ ", COL4_DARKGRAY);
    debug_write_col("HEAP TABLE", COL4_YELLOW);
    debug_writeln_col(" ----------------------------------------", COL4_DARKGRAY);
    debug_writeln_col("INDEX       STATE  PTR         THREAD     SIZE        BYTES", COL4_DARKGRAY);

    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (!mm_validate_ptr(mm_table[i].ptr)) { continue; }
        debug_printf("0x%8x  0x%2x   0x%8x  0x%8x  0x%8x  %d\n", 
            mm_index_from_ptr(mm_table[i].ptr), mm_table[i].state, (uint32_t)mm_table[i].ptr, (uint32_t)mm_table[i].thread, mm_table[i].size, mm_table[i].size_bytes);
    }

    debug_setmode(old);
}

void mm_print_mmap(DEBUGMODE mode)
{

}

void mm_print_info()
{
    term_write_fg("------ ", COL32_DARKGRAY);
    term_write_fg("MEMORY USAGE", COL32_YELLOW);
    term_writeln_fg(" --------------------------------------", COL32_DARKGRAY);

    term_write("- ");
    term_write_fg("INSTALLED               ", COL32_YELLOW);
    term_printf("%d MB(%d bytes)\n", mm_amount_installed() / 1024 / 1024, mm_amount_installed());

    term_write("- ");
    term_write_fg("RESERVED                ", COL32_YELLOW);
    term_printf("%d MB(%d bytes)\n", mm_amount_reserved() / 1024 / 1024, mm_amount_reserved());

    term_write("- ");
    term_write_fg("USED                    ", COL32_YELLOW);
    term_printf("%d MB(%d bytes)\n", mm_amount_used() / 1024 / 1024, mm_amount_used());

    term_write("- ");
    term_write_fg("FREE                    ", COL32_YELLOW);
    term_printf("%d MB(%d bytes)\n", mm_amount_free() / 1024 / 1024, mm_amount_free());
}

void mm_print_alloc(heap_entry_t* entry)
{
    if (!mm_messages) { return; }
    debug_header("MALLOC", COL4_GREEN);
    debug_printf("ID = 0x%8x, STATE = 0x%2x, PTR = 0x%8x, SIZE = 0x%8x, BYTES = %d\n", mm_index_from_ptr(entry->ptr), entry->state, (uint32_t)entry->ptr, entry->size, entry->size_bytes);
}

void mm_print_free(heap_entry_t* entry)
{
    if (!mm_messages) { return; }
    debug_header(" FREE ", COL4_YELLOW);
    debug_printf("ID = 0x%8x, STATE = 0x%2x, PTR = 0x%8x, SIZE = 0x%8x, BYTES = %d\n", mm_index_from_ptr(entry->ptr), entry->state, (uint32_t)entry->ptr, entry->size, entry->size_bytes);
}

void* mm_allocate(size_t size, bool_t clear, uint8_t state)
{
    if (size == 0) { return NULL; }
    //if (size == 0) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "mm_allocate"); return NULL; }
    heap_entry_t* entry = mm_allocate_entry(size);
    if (!mm_validate_entry(entry, TRUE)) { return NULL; }
    entry->thread = taskmgr_get_current_thread();

    if (state != MEMSTATE_FREE) { entry->state = state; }
    if (clear) { memsetl(entry->ptr, 0, entry->size); }
    mm_info.data_used += entry->size;
    mm_print_alloc(entry);
    return entry->ptr;
}

void mm_free(void* ptr)
{
    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (!mm_validate_entry(&mm_table[i], FALSE)) { continue; }
        if (mm_table[i].size == 0 || mm_table[i].state == MEMSTATE_FREE) { continue; }

        if (mm_table[i].ptr == ptr)
        {
            mm_table[i].state = MEMSTATE_FREE;
            mm_info.data_used -= mm_table[i].size;
            mm_print_free(&mm_table[i]);
            mm_merge_nearest();
            return;
        }
    }
}

heap_entry_t* mm_allocate_entry(uint32_t size)
{
    if (size == 0 || size >= mm_amount_free()) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "mm_allocate_entry"); return NULL; }

    uint32_t aligned = mm_align(size);

    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (!mm_validate_entry(&mm_table[i], FALSE)) { continue; }
        if (mm_table[i].size == aligned && mm_table[i].state == MEMSTATE_FREE)
        {
            mm_table[i].state = MEMSTATE_USED;
            mm_table[i].size_bytes = size;
            return &mm_table[i];
        }
    }

    mm_table[0].ptr        += aligned;
    mm_table[0].size       -= aligned;
    mm_table[0].size_bytes -= size;
    mm_table[0].state       = MEMSTATE_FREE;
    heap_entry_t* entry     = mm_create_entry(mm_table[0].ptr - aligned, aligned, MEMSTATE_USED);
    entry->size_bytes       = size;

    if (entry == NULL) { panicf(EXCEPTION_NULLPTR, NULL, "mm_allocate_entry"); return NULL; }
    return entry;
}

heap_entry_t* mm_get_nearest(heap_entry_t* entry)
{
    if (entry == NULL) { return NULL; }

    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (mm_table[i].ptr > 0 && mm_table[i].size > 0)
        {
            if (&mm_table[i] == entry) { continue; }
            if ((uint32_t)mm_table[i].ptr + mm_table[i].size == (uint32_t)entry->ptr && mm_table[i].state == MEMSTATE_FREE) 
            { 
                return &mm_table[i]; 
            }
            if ((uint32_t)entry->ptr - entry->size == (uint32_t)mm_table[i].ptr && mm_table[i].state == MEMSTATE_FREE) 
            { 
                return &mm_table[i]; 
            }
        }
    }
    
    return NULL;
}

bool_t mm_merge_nearest()
{
    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (mm_validate_ptr(mm_table[i].ptr) && mm_table[i].state == MEMSTATE_FREE)
        {
            heap_entry_t* nearest = mm_get_nearest(&mm_table[i]);

            if (nearest != NULL && nearest != &mm_table[i] && nearest != &mm_table[0] && i > 0)
            {
                if ((uint32_t)mm_table[i].ptr > (uint32_t)nearest->ptr) { mm_table[i].ptr = nearest->ptr; }
                mm_table[i].size += nearest->size;
                mm_table[i].size_bytes = mm_table[i].size;
                mm_table[i].state = MEMSTATE_FREE;
                mm_delete_entry(nearest);
            }
        }
    }

    heap_entry_t* nearest = mm_get_nearest(&mm_table[0]);

    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (!mm_validate_ptr(mm_table[i].ptr)) { continue; }

        if ((uint32_t)mm_table[i].ptr + mm_table[i].size == (uint32_t)mm_table[0].ptr && mm_table[i].state == MEMSTATE_FREE)
        {
            mm_table[0].ptr = mm_table[i].ptr;
            mm_table[0].size += mm_table[i].size;
            mm_table[0].size_bytes = 0;
            mm_table[0].state = MEMSTATE_FREE;
            mm_delete_entry(&mm_table[i]);
            break;
        }
    }

    mm_table[0].size_bytes = mm_table[0].size;

    return TRUE;
}

heap_entry_t* mm_create_entry(void* ptr, uint32_t len, uint8_t state)
{
    int i = mm_free_index();
    if (!mm_validate_index(i)) { panicf(EXCEPTION_INDEX_OUTOFRANGE, NULL, "mm_create_entry"); }
    mm_table[i].ptr        = ptr;
    mm_table[i].size       = len;
    mm_table[i].size_bytes = mm_align(len);
    mm_table[i].state      = state;
    return &mm_table[i];
}

bool_t mm_delete_entry(heap_entry_t* entry)
{
    if (!mm_validate_entry(entry, TRUE)) { return FALSE; }

    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (!mm_validate_entry(&mm_table[i], FALSE)) { continue; }
        if (mm_entry_equals(&mm_table[i], entry))
        {
            memset(&mm_table[i], 0, sizeof(heap_entry_t));
            return TRUE;
        }
    }

    panicf(EXCEPTION_CORRUPTEDMEMORY, NULL, "mm_delete_entry");
    return FALSE;
}

bool_t mm_validate_index(int index)
{
    if (index < 0 || index >= mm_info.count_max) { return FALSE; }
    return TRUE;
}

bool_t mm_validate_ptr(void* ptr)
{
    if (ptr < mm_info.data_base || ptr >= mm_info.data_end) { return FALSE; }
    return TRUE;
}

bool_t mm_validate_entry(heap_entry_t* entry, bool_t err)
{
    if (entry == NULL) { if (err) { panicf(EXCEPTION_NULLPTR, NULL, "mm_validate_entry"); return FALSE; } }
    if (!mm_validate_ptr(entry->ptr)) { if (err) { panicf(EXCEPTION_ARG, NULL, "mm_validate_entry"); return FALSE; } }
    if (entry->size == 0 || entry->size == 0) { if (err) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "mm_validate_entry"); return FALSE; } }
    return TRUE;
}

bool_t mm_check_index_null(int index)
{
    if (index < 0 || index >= mm_info.count_max) { return FALSE; }
    if (mm_table[index].ptr != NULL)             { return FALSE; }
    if (mm_table[index].size != 0)               { return FALSE; }
    if (mm_table[index].size_bytes != 0)         { return FALSE; }
    if (mm_table[index].state != MEMSTATE_FREE)  { return FALSE; }
    return TRUE;
}

bool_t mm_entry_equals(heap_entry_t* a, heap_entry_t* b)
{
    if (a == NULL && b == NULL) { return FALSE; }
    if (a == NULL && b != NULL) { return FALSE; }
    if (a != NULL && b == NULL) { return FALSE; }
    if (a->ptr == b->ptr && a->size == b->size && a->size_bytes == b->size_bytes && a->state == b->state) { return TRUE; }
    return FALSE;
}

int mm_free_index()
{
    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (mm_check_index_null(i)) { return i; }
    }
    return -1;
}

heap_entry_t* mm_entry_from_ptr(void* ptr)
{
    if (ptr == NULL) { return NULL; }
    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (mm_table[i].ptr == NULL) { continue; }
        if (mm_table[i].ptr == ptr) { return &mm_table[i]; }
    }
    return NULL;
}

uint32_t mm_size_from_ptr(void* ptr)
{
    if (ptr == NULL) { return 0; }
    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (mm_table[i].ptr == NULL) { continue; }
        if (mm_table[i].ptr == ptr) { return mm_table[i].size; }
    }
    return 0;
}

int mm_index_from_ptr(void* ptr)
{
    if (ptr == NULL) { return -1; }
    for (uint32_t i = 0; i < mm_info.count_max; i++)
    {
        if (!mm_validate_entry(&mm_table[i], FALSE)) { continue; }
        if (mm_table[i].ptr == ptr) { return i; }
    }
    return -1;
}

uint32_t mm_align(uint32_t value)
{
    uint32_t out = value;
    out &= 0xFFFFF000;
    if (out < value) { out += MEM_ALIGN; }
    return out;
}

uint32_t mm_amount_installed()
{
    register uint64_t* mem;
    uint64_t mem_count, a;
    uint32_t memkb;
    uint8_t	irq1, irq2;

    irq1 = port_inb(0x21);
    irq2 = port_inb(0xA1);

    port_outb(0x21, 0xFF);
    port_outb(0xA1, 0xFF);

    mem_count = 0;
    memkb = 0;
    asm volatile("wbinvd");

    do 
    {
        memkb++;
        mem_count += 1024 * 1024;
        mem = (uint64_t*)mem_count;

        a = *mem;
        *mem = 0x55AA55AA;

        asm("":::"memory");
        if (*mem != 0x55AA55AA) { mem_count = 0; }
        else 
        {
            *mem = 0xAA55AA55;
            asm("":::"memory");
            if (*mem != 0xAA55AA55)
                mem_count = 0;
        }

        asm("":::"memory");
        *mem = a;

    } while (memkb < 4096 && mem_count != 0);

    mem = (uint64_t*)0x413;

    port_outb(0x21, irq1);
    port_outb(0xA1, irq2);

    return memkb * 1024 * 1024;
}

uint32_t mm_amount_reserved() { return mm_info.data_length; }

uint32_t mm_amount_used() { return mm_info.data_used; }

uint32_t mm_amount_free() { return mm_info.data_length - mm_info.data_used; }

uint32_t mm_amount_entries() { return mm_info.count; }

uint32_t mm_amount_entries_max() { return mm_info.count_max; }