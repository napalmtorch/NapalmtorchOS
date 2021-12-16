#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_VMODE(char* input, char** argv, int argc)
{
    if (argc != 3) { term_writeln_fg("Invalid arguments", COL32_RED); return; }

    uint32_t w = atol(argv[1]);
    uint32_t h = atol(argv[2]);
    vesa_setmode(w, h);
    DEBUGMODE old = debug_getmode();
    debug_setmode(DEBUGMODE_TERM);
    term_clear();
    debug_info("Set VESA mode to %dx%dx32", w, h);
    debug_setmode(old);
}