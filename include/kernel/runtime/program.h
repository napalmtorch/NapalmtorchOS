#pragma once
#include <lib/types.h>
#include <kernel/system/thread.h>

typedef struct
{
    const char* name;
    thread_t*   thread;
    uint8_t*    program_data;
} PACKED program_t;

program_t program_create(const char* filename);