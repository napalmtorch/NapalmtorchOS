#include <kernel/runtime/process.h>
#include <kernel/core/kernel.h>

process_t** proc_list;
uint32_t    proc_count;
uint32_t    proc_count_max;
uint32_t    proc_id = 0;

int procmgr_free_index();

void procmgr_init()
{
    proc_id         = 0;
    proc_count      = 0;
    proc_count_max  = 4096;
    proc_list = tcalloc(proc_count_max * sizeof(process_t*), MEMSTATE_PTRARRAY);
    debug_ok("Initialized process manager");
}

void procmgr_monitor()
{
    for (uint32_t i = 0; i < proc_count_max; i++)
    {
        if (proc_list[i] == NULL) { return; }

        if (proc_list[i]->state == PROCSTATE_TERMINATED)
        {
            free(proc_list[i]->data);
            free(proc_list[i]);
            debug_info("Cleaned terminated processes");
            proc_list[i] = NULL;
            return;
        }
    }
}

void procmgr_load(process_t* proc)
{
    if (proc == NULL) { debug_error("Tried to load null process"); return; }
    int i = procmgr_free_index();
    if (i < 0 || i >= proc_count_max) { debug_error("Maximum amount of processes reached"); return; }

    proc->id = proc_id++;
    proc->state = PROCSTATE_HALTED;
    proc_list[i] = proc;
    proc_count++;
    if (!elf_attach(proc)) { return FALSE; }
    debug_info("Loaded process '%s', id = %d", proc->name, proc->id);
}

process_t* procmgr_load_file(const char* filename)
{
    uint32_t* len = 0;
    uint8_t* prog_data = vfs_read_bytes(filename, &len);
    if (prog_data == NULL) { debug_error("Error reading executable file '%s'", filename); return NULL; }
    
    program_header_t* header = (program_header_t*)prog_data;
    if (header->signature != 0xBAD0A515) { debug_error("Invalid signature in executable file '%s'", filename); return NULL; }

    process_t* proc = tcalloc(sizeof(process_t), MEMSTATE_SYSTEM);
    strcpy(proc->name, header->name);

    proc->data = prog_data;
    proc->size = len;

    procmgr_load(proc);
    return proc;
}


bool_t procmgr_start(process_t* proc)
{
    if (!procmgr_is_loaded(proc)) { debug_error("Tried to start un-loaded process '%s'", proc->name); return FALSE; }
    debug_info("Starting process '%s'", proc->name);
    proc->state = PROCSTATE_RUNNING;
    taskmgr_ready_thread(proc->thread);
    return TRUE;
}

bool_t procmgr_terminate(process_t* proc)
{
    if (!procmgr_is_loaded(proc)) { debug_error("Tried to terminate un-loaded process '%s'", proc->name); return; }
    for (uint32_t i = 0; i < proc_count_max; i++)
    {
        if (proc_list[i] == NULL) { continue; }
        if (proc_list[i] == proc)
        {
            uint32_t id = proc_list[i]->id;
            if (!taskmgr_terminate(proc_list[i]->thread)) { break; }
            proc->state = PROCSTATE_TERMINATED;
            debug_info("Terminated process %d", id);
            return TRUE;
        }
    }

    debug_error("Unable to terminate process '%s', id = ", proc->name, proc->id);
    return FALSE;
}

int procmgr_free_index()
{
    for (uint32_t i = 0; i < proc_count_max; i++)
    {
        if (proc_list[i] == NULL) { return i; }
    }
    return -1;
}

bool_t procmgr_is_loaded(process_t* proc)
{
    if (proc == NULL) { return FALSE; }
    for (uint32_t i = 0; i < proc_count_max; i++)
    {
        if (proc_list[i] == NULL) { continue; }
        if (proc_list[i] == proc) { return TRUE; }
    }
    return FALSE;
}

process_t* procmgr_get_byid(uint32_t id)
{
    for (uint32_t i = 0; i < proc_count_max; i++)
    {
        if (proc_list[i] == NULL) { continue; }
        if (proc_list[i]->id == id) { return proc_list[i]; }
    }
    return NULL;
}