#include <kernel/system/syscall.h>
#include <kernel/core/kernel.h>

void syscall_execute(registers_t* regs)
{
    // get system call identifier from register
    uint32_t code = regs->ecx;

    // loop through list of system calls
    for (uint32_t i = 0; i < SYSCALL_COUNT; i++)
    {
        // located desired system call - now execute
        if (SYSTEM_CALLS[i]->code == code)
        {
            syscall_args_t args = { regs->eax, regs->ebx, regs->ecx, regs->edx };
            if (SYSTEM_CALLS[i]->m_execute == NULL) { debug_error("Pointer to system call method was null"); return; }
            SYSTEM_CALLS[i]->m_execute(args);
            debug_info("Executed system call 0x%8x - ARGS(0x%8x, 0x%8x, 0x%8x)", code, regs->eax, regs->ebx, regs->edx);
            return;
        }
    }

    debug_error("Invalid system call 0x%8x", code);
}

void SYSCALL_METHOD_LOADMETHODS(syscall_args_t args)
{
    uint32_t* output_ptr = (uint32_t*)args.a;
    void* methods = elf_map_methods();
    output_ptr[0] = (uint32_t)methods;
}