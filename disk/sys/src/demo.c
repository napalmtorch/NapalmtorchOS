#include <sys/lib/library.h>

int _start(void* arg)
{
    lib_init();

    char* data = calloc(10);
    data[0] = 'H';
    data[1] = 'E';
    data[2] = 'L';
    data[3] = 'L';
    data[4] = 'O';

    term_clear_col(0xFF007F7F);
    term_writechar_col('X', 0xFFFFFF00, 0xFF0000FF);
    term_write_col("Hello world\n", 0xFFFF0000, 0xFFFFFFFF);
    term_write_col(data, 0xFF00FF00, 0xFF4F4F4F);

    free(data);

    return 69;
}