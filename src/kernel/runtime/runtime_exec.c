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
    runtime->exit_code = 0xAD;

    ((thread_t*)runtime->thread)->runtime = runtime;
    return runtime;
}

runtime_t* runtime_create_raw(uint8_t* data, uint32_t size)
{

}

void runtime_dispose(runtime_t* runtime)
{
    if (runtime == NULL) { return; }
    free(runtime->name);
    free(runtime);
}

bool_t runtime_start(runtime_t* runtime)
{
    taskmgr_ready_thread(runtime->thread);
}

bool_t runtime_stop(runtime_t* runtime)
{
    
}

int runtime_main(runtime_t* runtime)
{
    runtime->state = VMSTATE_RUNNING;

    while (TRUE)
    {
        spinlock_lock(&((thread_t*)runtime->thread)->lock);
        if (runtime->state == VMSTATE_TERMINATED) { runtime_dispose(runtime); break; }
        spinlock_unlock(&((thread_t*)runtime->thread)->lock);
    }

    return runtime->exit_code;
}