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
    VMFLAG_POSITIVE     = 1 << 0,
    VMFLAG_ZERO         = 1 << 1,
    VMFLAG_NEGATIVE     = 1 << 2,
} VMFLAG;

typedef enum
{
    VMOP_ADD            = 0x01,
    VMOP_SUBTRACT       = 0x02,
    VMOP_MULTIPLY       = 0x03,
    VMOP_DIVIDE         = 0x04,
    VMOP_LSHIFT         = 0x05,
    VMOP_RSHIFT         = 0x06,
    VMOP_OR             = 0x07,
    VMOP_XOR            = 0x08,
    VMOP_AND            = 0x09,
    VMOP_NOT            = 0x0A,
    VMOP_COPY           = 0x0B,
    VMOP_COUNT,
} VMOP;

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
    uint32_t index;
    uint32_t address;
    uint8_t* data;
    uint8_t  state;
    uint8_t  access;
    uint8_t  reserved[18];
} PACKED runtime_page_t;

typedef struct
{
    char*           name;
    uint32_t        registers[VMREG_COUNT];
    VMSTATE         state;
    executable_t    program;
    void*           thread;
    runtime_page_t* pages;
    uint32_t        page_count;
    uint32_t        page_count_max;
    uint32_t        exit_code;
    uint32_t        current_op;
} runtime_t;

runtime_t* runtime_create(const char* name, executable_t prog);
runtime_t* runtime_create_raw(uint8_t* data, uint32_t size);
void       runtime_dispose(runtime_t* runtime);
bool_t     runtime_start(runtime_t* runtime);
bool_t     runtime_exit(runtime_t* runtime);
bool_t     runtime_halt(runtime_t* runtime);
bool_t     runtime_continue(runtime_t* runtime);
int        runtime_main(runtime_t* runtime);

void       runtime_step(runtime_t* runtime);
bool_t     runtime_execute(runtime_t* runtime, uint8_t op);

bool_t     runtime_validate(runtime_t* runtime);

bool_t      runtime_perform_op_imm(runtime_t* vm, VMREG r, uint32_t val, VMOP op);
bool_t      runtime_perform_op_reg(runtime_t* vm, VMREG rx, VMREG ry, VMOP op);
void        runtime_set_flags(runtime_t* vm, VMREG r);

bool_t      runtime_stackpush(runtime_t* vm, uint32_t value);
bool_t      runtime_stackpush_regs(runtime_t* vm);
uint32_t    runtime_stackpop(runtime_t* vm);
uint32_t    runtime_stackpeek(runtime_t* vm);
uint32_t    runtime_stackpeek_at(runtime_t* vm, uint32_t offset, bool_t neg);
uint32_t*   runtime_stackpop_regs(runtime_t* vm);

void runtime_print_state(runtime_t* runtime);