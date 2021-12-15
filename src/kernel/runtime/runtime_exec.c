#include <kernel/runtime/runtime_exec.h>
#include <kernel/core/kernel.h>

runtime_t runtime_create(executable_t prog)
{
    runtime_t runtime;
    memset(&runtime, 0, sizeof(runtime_t));
}

runtime_t runtime_create_raw(uint8_t* data, uint32_t size)
{

}

bool_t runtime_start(runtime_t* runtime)
{

}

bool_t runtime_stop(runtime_t* runtime)
{
    
}

int runtime_main(thread_t* thread)
{

}