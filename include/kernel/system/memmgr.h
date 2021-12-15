#pragma once
#include <lib/types.h>
#include <kernel/system/debug.h>
#include <kernel/system/thread.h>

#define MEMSTATE_FREE      0
#define MEMSTATE_USED      1
#define MEMSTATE_SYSTEM    2
#define MEMSTATE_SERVICE   3
#define MEMSTATE_PTRARRAY  4
#define MEMSTATE_ARRAY     5
#define MEMSTATE_THREAD    6
#define MEMSTATE_THREADSTK 7
#define MEMSTATE_STRING    8
#define MEMSTATE_BITMAP    9
#define MEMSTATE_VM        10
#define MEMSTATE_VMPAGE    11

#define MEM_ALIGN 0x1000

typedef struct
{
    uint32_t size;
    uint32_t address_low;
    uint32_t address_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
} PACKED mmap_entry_t;

typedef struct
{
    uint32_t  size_bytes;
    uint32_t  size;
    thread_t* thread;
    uint16_t  state;
    void*     ptr;
} PACKED heap_entry_t;

typedef struct
{
    uint32_t base;
    uint32_t end;
    uint32_t length;
    uint32_t count;
    uint32_t count_max;
    uint32_t data_base;
    uint32_t data_end;
    uint32_t data_length;
    uint32_t data_used;
} PACKED heap_info_t;

void mm_init();
void mm_mmap_read();

void mm_print_heap(DEBUGMODE mode);
void mm_print_mmap(DEBUGMODE mode);
void mm_print_info();
void mm_print_alloc(heap_entry_t* entry);
void mm_print_free(heap_entry_t* entry);

void* mm_allocate(size_t size, bool_t clear, uint8_t state);
void  mm_free(void* ptr);

heap_entry_t* mm_entry_from_ptr(void* ptr);
uint32_t      mm_size_from_ptr(void* ptr);
int           mm_index_from_ptr(void* ptr);

uint32_t mm_align(uint32_t value);
uint32_t mm_amount_installed();
uint32_t mm_amount_reserved();
uint32_t mm_amount_used();
uint32_t mm_amount_free();
uint32_t mm_amount_entries();
uint32_t mm_amount_entries_max();