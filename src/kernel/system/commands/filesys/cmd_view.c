#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_VIEW(char* input, char** argv, int argc)
{
    const char* strr = "view -t /sys/testdoc.txt";

    if (argc != 3) { term_writeln_fg("Invalid arguments", COL32_RED); return; }

    char* path = (char*)(input + 8);
    if (!vfs_file_exists(path)) { term_writeln_fg("Unable to locate file", COL32_RED); return; }

    if (streql(argv[1], "-t"))
    {
        char* file_data = vfs_read_text(path);
        if (file_data == NULL) { term_writeln_fg("Unable to read file", COL32_RED); return; }
        term_writeln(file_data);
        free(file_data);
    }
    else if (streql(argv[1], "-b"))
    {
        uint32_t file_size = 0;
        char* file_data = vfs_read_bytes(path, &file_size);
        if (file_data == NULL) { term_writeln_fg("Unable to read file", COL32_RED); return; }
        debug_dumpmem(file_data, file_size, DEBUGMODE_TERM);
        free(file_data);
    }
}