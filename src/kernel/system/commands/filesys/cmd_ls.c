#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_LS(char* input, char** argv, int argc)
{
    char* path = (char*)(input + 3);
    term_printf("Listing contents of %s\n", path);

    int dirs_count = 0;
    char** dirs = vfs_get_dirs(path, &dirs_count);
    for (int i = 0; i < dirs_count; i++) { term_printf("- %s\n", dirs[i]); }
    freearray(dirs, dirs_count);

    int files_count = 0;
    char** files = vfs_get_files(path, &files_count);
    for (int i = 0; i < files_count; i++) { term_printf("%s\n", files[i]); }
    freearray(files, files_count);
}