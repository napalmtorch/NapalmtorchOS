#pragma once
#include <lib/types.h>
#include <kernel/system/thread.h>

typedef enum
{
    PROCSTATE_HALTED,
    PROCSTATE_RUNNING,
    PROCSTATE_TERMINATED,
} PROCSTATE;

typedef struct
{
    char     name[64];
    uint8_t  reserved[60];
    uint32_t signature;
} PACKED program_header_t;

typedef struct
{
    char      name[64];
    PROCSTATE state;
    thread_t* thread;
    uint8_t*  data;
    uint32_t  size;
    uint32_t  id;
} PACKED process_t;

void procmgr_init();
void procmgr_monitor();
void procmgr_load(process_t* proc);
process_t* procmgr_load_file(const char* filename);
bool_t procmgr_start(process_t* proc);
bool_t procmgr_terminate(process_t* proc);
bool_t procmgr_is_loaded(process_t* proc);
process_t* procmgr_get_byid(uint32_t id);