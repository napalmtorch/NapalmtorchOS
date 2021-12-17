#include <sys/lib/library.h>

int _start(void* argv)
{
    lib_init();
    
    return 69;
}