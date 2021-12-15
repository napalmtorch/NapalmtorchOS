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
    return runtime;
}

runtime_t* runtime_create_raw(uint8_t* data, uint32_t size)
{

}

bool_t runtime_start(runtime_t* runtime)
{

}

bool_t runtime_stop(runtime_t* runtime)
{
    
}

int runtime_main(runtime_t* runtime)
{
    uint32_t code = 0;
    while (TRUE)
    {
        
    }
    return code;
}