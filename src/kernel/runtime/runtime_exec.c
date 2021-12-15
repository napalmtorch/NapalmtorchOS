#include <kernel/runtime/runtime_exec.h>
#include <kernel/core/kernel.h>

runtime_t* runtime_create(const char* name, executable_t prog)
{
    runtime_t* runtime = tcalloc(sizeof(runtime_t), MEMSTATE_VM);

    runtime->name = tcalloc(strlen(name) + 1, MEMSTATE_STRING);
    strcpy(runtime->name, name);

    runtime->thread = thread_create(name, runtime_main, 32768, runtime);
    runtime->state  = VMSTATE_HALTED;
    runtime->program = prog;
    runtime->exit_code = 0;

    runtime->page_count     = 0;
    runtime->page_count_max = VM_DEFAULT_MEM / VM_PAGE_SIZE;
    runtime->pages = tcalloc(sizeof(runtime_page_t) * runtime->page_count_max, MEMSTATE_ARRAY);
    for (uint32_t i = 0; i < runtime->page_count_max; i++)
    {
        runtime->pages[i].index   = i;
        runtime->pages[i].address = i * VM_PAGE_SIZE;
        runtime->pages[i].state   = VM_PAGESTATE_FREE;
        runtime->pages[i].access  = VM_ACCESS_RW;
        runtime->pages[i].data    = NULL;
    }

    ((thread_t*)runtime->thread)->runtime = runtime;
    return runtime;
}

runtime_t* runtime_create_raw(uint8_t* data, uint32_t size)
{

}

void runtime_dispose(runtime_t* runtime)
{
    if (runtime == NULL) { return; }

    for (uint32_t i = 0; i < runtime->page_count_max; i++)
    {
        if (runtime->pages[i].data != NULL) { free(runtime->pages[i].data); }
    }

    free(runtime->pages);
    free(runtime->name);
    free(runtime);
}

bool_t runtime_start(runtime_t* runtime)
{
    taskmgr_ready_thread(runtime->thread);
}

bool_t runtime_exit(runtime_t* runtime)
{
    runtime->exit_code = 0;
    runtime->state = VMSTATE_TERMINATED;
}

bool_t runtime_halt(runtime_t* runtime)
{
    runtime->state = VMSTATE_HALTED;
}

bool_t runtime_continue(runtime_t* runtime)
{
    runtime->state = VMSTATE_RUNNING;
}

int runtime_main(runtime_t* runtime)
{
    runtime_halt(runtime);
    for (uint32_t i = 0; i < runtime->program.total_size; i++) { runtime_write8(runtime, i, runtime->program.data[i]); }
    runtime->registers[VMREG_EIP] = runtime->program.code_address;
    runtime_continue(runtime);

    while (TRUE)
    {
        spinlock_lock(&((thread_t*)runtime->thread)->lock);
        if (runtime->state == VMSTATE_RUNNING)
        {
            runtime_step(runtime);
            if (runtime->exit_code > 0) { runtime_dispose(runtime); break; }
        }
        if (runtime->state == VMSTATE_TERMINATED) { runtime_dispose(runtime); break; }
        spinlock_unlock(&((thread_t*)runtime->thread)->lock);
    }

    return runtime->exit_code;
}

void runtime_step(runtime_t* runtime)
{
    runtime->current_op = runtime_read8(runtime, runtime->registers[VMREG_EIP]);
    if (!runtime_execute(runtime, runtime->current_op)) { runtime->exit_code = 0xFF; }
}

bool_t runtime_execute(runtime_t* runtime, uint8_t op)
{
    for (uint32_t i = 0; i < VM_ISA_COUNT; i++)
    {
        if (VM_ISA[i] == NULL) { continue; }
        if (VM_ISA[i]->opcode == op)
        {
            VM_ISA[i]->m_execute(runtime);
            if (VM_ISA[i]->increment) { runtime->registers[VMREG_EIP] += VM_ISA[i]->cycles; }
            debug_info("Executed instruction 0x%2x", op);
            runtime_print_state(runtime);
            return TRUE;
        }
    }

    debug_error("Invalid instruction 0x%2x", op);
    runtime_print_state(runtime);
    runtime->exit_code = 0x01;
    return FALSE;
}

bool_t runtime_validate(runtime_t* runtime)
{
    if (runtime == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "runtime_validate"); return FALSE; }
    return TRUE;
}

bool_t runtime_perform_op_imm(runtime_t* runtime, VMREG r, uint32_t val, VMOP op)
{
    bool_t error = FALSE;
    bool_t flags = FALSE;
    switch (op)
    {
        case VMOP_ADD:      { runtime->registers[r] += val; flags = TRUE; break; }
        case VMOP_SUBTRACT: { runtime->registers[r] -= val; flags = TRUE; break; }
        case VMOP_MULTIPLY: { runtime->registers[r] *= val; flags = TRUE; break; }
        case VMOP_DIVIDE:   { runtime->registers[r] /= val; flags = TRUE; break; }
        case VMOP_LSHIFT:   { runtime->registers[r] << val; flags = TRUE; break; }
        case VMOP_RSHIFT:   { runtime->registers[r] >> val; flags = TRUE; break; }
        case VMOP_OR:       { runtime->registers[r] |  val; flags = TRUE; break; }
        case VMOP_XOR:      { runtime->registers[r] ^  val; flags = TRUE; break; }
        case VMOP_AND:      { runtime->registers[r] &  val; flags = TRUE; break; }
        case VMOP_NOT:      { runtime->registers[r] = ~runtime->registers[r]; flags = TRUE; break; }
        case VMOP_COPY:     { runtime->registers[r] =  val; flags = FALSE; break; }
        default:            { error = TRUE; flags = FALSE; break; }
    }

    if (!error)
    {
        if (flags) { runtime_set_flags(runtime, r); }
        return TRUE;
    }

    char err_msg[64];
    char temp[16];
    memset(err_msg, 0, 64);
    memset(temp, 0, 16);
    strcpy(err_msg, "Unrecognized immediate operation ");
    strcpy(err_msg + strlen(err_msg), strhex((uint32_t)op, temp, TRUE, 1));
    term_writeln(err_msg);
    runtime->exit_code = 0xAB;
    return FALSE;
}

bool_t runtime_perform_op_reg(runtime_t* runtime, VMREG rx, VMREG ry, VMOP op)
{
    bool_t error = FALSE;
    bool_t flags = FALSE;
    switch (op)
    {
        case VMOP_ADD:      { runtime->registers[rx] += runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_SUBTRACT: { runtime->registers[rx] -= runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_MULTIPLY: { runtime->registers[rx] *= runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_DIVIDE:   { runtime->registers[rx] /= runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_LSHIFT:   { runtime->registers[rx] << runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_RSHIFT:   { runtime->registers[rx] >> runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_OR:       { runtime->registers[rx] |  runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_XOR:      { runtime->registers[rx] ^  runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_AND:      { runtime->registers[rx] &  runtime->registers[ry]; flags = TRUE; break; }
        case VMOP_NOT:      { runtime->registers[rx] = ~runtime->registers[rx]; flags = TRUE; break; }
        case VMOP_COPY:     { runtime->registers[rx] =  runtime->registers[ry]; flags = FALSE; break; }
        default:            { error = TRUE; flags = FALSE; break; }
    }

    if (!error)
    {
        if (flags) { runtime_set_flags(runtime, rx); }
        return TRUE;
    }

    char err_msg[64];
    char temp[16];
    memset(err_msg, 0, 64);
    memset(temp, 0, 16);
    strcpy(err_msg, "Unrecognized register operation ");
    strcpy(err_msg + strlen(err_msg), strhex((uint32_t)op, temp, TRUE, 1));
    term_writeln(err_msg);
    runtime->exit_code = 0xAB;
    return FALSE;
}

void runtime_set_flags(runtime_t* runtime, VMREG r)
{
    uint32_t cond = runtime->registers[VMREG_COND];
    if (runtime->registers[r] == 0) { runtime->registers[VMREG_COND] = VMFLAG_ZERO; }
    else if (runtime->registers[r] >> 31 != 0) { runtime->registers[VMREG_COND] = VMFLAG_NEGATIVE; }
    else { runtime->registers[VMREG_COND] = VMFLAG_POSITIVE; }
}

bool_t runtime_stackvalidate(runtime_t* runtime)
{
    if (runtime->registers[VMREG_EBP] < runtime->registers[VMREG_ESP]) { runtime->exit_code = 0x0F; return FALSE; }
    if (runtime->registers[VMREG_ESP] < runtime->program.code_address + runtime->program.code_size)  { runtime->exit_code = 0x0E; return FALSE; }
    return TRUE;
}

bool_t runtime_stackpush(runtime_t* runtime, uint32_t value)
{
    if (!runtime_stackvalidate(runtime)) { return FALSE; }
    runtime->registers[VMREG_ESP] -= 4;
    return runtime_write32(runtime, runtime->registers[VMREG_ESP], value);
}

bool_t runtime_stackpush_regs(runtime_t* runtime)
{
    uint32_t start_sp = runtime->registers[VMREG_ESP];
    for (uint32_t i = 0; i < 8; i++) { if (!runtime_stackpush(runtime, runtime->registers[i])) { return FALSE; } }
    if (!runtime_stackpush(runtime, start_sp)) { return FALSE; }
    if (!runtime_stackpush(runtime, runtime->registers[VMREG_EBP])) { return FALSE; }
    if (!runtime_stackpush(runtime, runtime->registers[VMREG_EIP])) { return FALSE; }
    return runtime_stackpush(runtime, runtime->registers[VMREG_COND]);
}

uint32_t runtime_stackpop(runtime_t* runtime)
{
    if (!runtime_stackvalidate(runtime)) { return FALSE; }
    uint32_t val = runtime_read32(runtime, runtime->registers[VMREG_ESP]);
    runtime->registers[VMREG_ESP] += 4;
    return val;
}

uint32_t runtime_stackpeek(runtime_t* runtime)
{
    if (!runtime_stackvalidate(runtime)) { return FALSE; }
    return runtime_read32(runtime, runtime->registers[VMREG_ESP]);
}

uint32_t runtime_stackpeek_at(runtime_t* runtime, uint32_t offset, bool_t neg)
{
    if (!runtime_stackvalidate(runtime)) { return FALSE; }
    uint32_t addr = runtime->registers[VMREG_ESP];
    if (neg) { addr += offset; } else { addr -= offset; }
    return runtime_read32(runtime, addr);
}

uint32_t* runtime_stackpop_regs(runtime_t* runtime)
{
    uint32_t regs[VMREG_COUNT];

    regs[VMREG_COND] = runtime_stackpop(runtime);
    regs[VMREG_EIP]   = runtime_stackpop(runtime);
    regs[VMREG_EBP]   = runtime_stackpop(runtime);
    regs[VMREG_ESP]   = runtime_stackpop(runtime);
    for (int i = 7; i >= 0; i--) { regs[i] = runtime_stackpop(runtime); }
    uint32_t* output = tcalloc(VMREG_COUNT * 4, MEMSTATE_ARRAY);
    memcpy(output, regs, VMREG_COUNT * 4);
    return output;
}

void runtime_print_state(runtime_t* runtime)
{
    term_writeln_fg("RUNTIME STATE", COL32_YELLOW);
    term_printf("EAX: 0x%8x EBX: 0x%8x ECX: 0x%8x EDX: 0x%8x\n", runtime->registers[VMREG_EAX], runtime->registers[VMREG_EBX], runtime->registers[VMREG_ECX], runtime->registers[VMREG_EDX]);
    term_printf("ESI: 0x%8x EDI: 0x%8x ESP: 0x%8x EBP: 0x%8x\n", runtime->registers[VMREG_ESI], runtime->registers[VMREG_EDI], runtime->registers[VMREG_ESP], runtime->registers[VMREG_EBP]);
    term_printf("EIP: 0x%8x CND: 0x%8x OP:  0x%2x\n", runtime->registers[VMREG_EIP], runtime->registers[VMREG_COND], (uint32_t)runtime->current_op);
}