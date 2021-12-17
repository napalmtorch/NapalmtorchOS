#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>
#include <kernel/runtime/elf.h>

void CMD_METHOD_RUN(char* input, char** argv, int argc)
{
    process_t* proc = procmgr_load_file(argv[1]);
    if (proc == NULL) { return; }
    procmgr_start(proc);
}