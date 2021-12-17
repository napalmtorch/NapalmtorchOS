#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_KILL(char* input, char** argv, int argc)
{
    if (argc == 2)
    {
        uint32_t id = atol(argv[1]);
        process_t* proc = procmgr_get_byid(id);
        procmgr_terminate(proc);
        //thread_t* thread = taskmgr_get_thread_byid(id);
        //if (thread == NULL) { term_printf("No thread running with id %d\n", id); return; }
        //if (!taskmgr_terminate(thread)) { term_printf("Unable to terminate thread %d\n", id); return; }
        //term_write_fg("Terminated thread ", COL32_GREEN);
        //term_printf("%d\n", id);
        return;
    }

   term_writeln_fg("Invalid arguments", COL32_RED);
}