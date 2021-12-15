#pragma once
#include <lib/types.h>

typedef enum
{
    VMREG_EAX,
    VMREG_EBX,
    VMREG_ECX,
    VMREG_EDX,
    VMREG_ESI,
    VMREG_EDI,
    VMREG_ESP,
    VMREG_EBP,
    VMREG_EIP,
    VMREG_COND,
    VMREG_COUNT,
} VMREG;

typedef enum
{
    VMSTATE_HALTED,
    VMSTATE_RUNNING,
    VMSTATE_TERMINATED,
} VMSTATE;

typedef struct
{
    uint32_t code_address;
    uint32_t data_address;
    uint32_t code_size;
    uint32_t data_size;
    uint32_t total_size;
    char*    filename;
    uint8_t* data;
} executable_t;

typedef struct
{
    char*        name;
    uint32_t     registers[VMREG_COUNT];
    VMSTATE      state;
    executable_t program;
    void*        thread;
    uint32_t     exit_code;
} runtime_t;

runtime_t* runtime_create(const char* name, executable_t prog);
runtime_t* runtime_create_raw(uint8_t* data, uint32_t size);
void       runtime_dispose(runtime_t* runtime);
bool_t     runtime_start(runtime_t* runtime);
bool_t     runtime_stop(runtime_t* runtime);
int        runtime_main(runtime_t* runtime);