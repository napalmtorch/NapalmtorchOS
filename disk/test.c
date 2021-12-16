#include "../include/lib/types.h"

typedef void (*term_vprintf)(const char* str, uint32_t fg, uint32_t bg);

int _start(void* arg)
{
    term_vprintf p = (term_vprintf*)0x001115B1;

    p("Hello world\n", 0xFFFF0000, 0xFFFFFFFF);
    p("This is a test\n", 0xFFFFFFFF, 0xFF007F7F);
    return 69;
}