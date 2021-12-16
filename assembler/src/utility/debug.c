#include "utility/debug.h"

bool_t debug_enabled;

void debug_init()
{
    debug_toggle(TRUE);
}

void debug_toggle(bool_t state)
{
    debug_enabled = state;
    if (state) { debug_ok("Enabled debugging interface"); }
}

void debug_panic() 
{ 
    if (!debug_enabled) { return; }
    exit(0); 
}

void debug_clear() 
{ 
    if (!debug_enabled) { return; }
    system("clear"); 
}

void debug_newline() 
{ 
    if (!debug_enabled) { return; }
    printf("\n"); 
}

void debug_newlines(int count) 
{ 
    if (!debug_enabled) { return; }
    while (count > 0) { debug_newline(); count--; } 
}

void debug_write(const char* str) { debug_printf("%s", str); }

void debug_writeln(const char* str) { debug_printf("%s\n", str); }

void debug_printf(const char* str, ...)
{
    if (!debug_enabled) { return; }
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

void debug_header(const char* str, const char* col)
{
    debug_write("[");
    debug_printf("%s%s%s", col, str, TERMCOL_RESET);
    debug_write("] ");
}

void debug_info(const char* str, ...)
{
    if (!debug_enabled) { return; }
    va_list args;
    va_start(args, str);
    debug_header("  >>  ", TERMCOL_CYAN);
    vprintf(str, args);
    debug_newline();
    va_end(args);
}

void debug_ok(const char* str, ...)
{
    if (!debug_enabled) { return; }
    va_list args;
    va_start(args, str);
    debug_header("  OK  ", TERMCOL_GREEN);
    vprintf(str, args);
    debug_newline();
    va_end(args);
}

void debug_warn(const char* str, ...)
{
    if (!debug_enabled) { return; }
    va_list args;
    va_start(args, str);
    debug_header("  ??  ", TERMCOL_YELLOW);
    vprintf(str, args);
    debug_newline();
    va_end(args);
}

void debug_error(const char* str, ...)
{
    if (!debug_enabled) { return; }
    va_list args;
    va_start(args, str);
    debug_header("  !!  ", TERMCOL_RED);
    vprintf(str, args);
    debug_newline();
    va_end(args);
    debug_panic();
}

void debug_dumpmem(uint8_t* data, uint32_t size)
{
    if (!debug_enabled) { return; }
    debug_printf("Dumping %s%d%s bytes of memory at %s0x%08X%s\n", TERMCOL_CYAN, size, TERMCOL_RESET, TERMCOL_CYAN, (uint32_t)data, TERMCOL_RESET);
    const uint32_t bpl = 16;
    char chars[bpl + 1];

    for (uint32_t i = 0; i < size; i += bpl)
    {
        memset(chars, 0, bpl + 1);

        debug_printf("%s0x%08X%s:%s0x%08X%s  ", TERMCOL_YELLOW, (uint32_t)(data + i), TERMCOL_RESET, TERMCOL_YELLOW, (uint32_t)(data + i + (bpl - 1)), TERMCOL_RESET);

        for (uint32_t j = 0; j < bpl; j++)
        {
            uint8_t v = data[i + j];
            if (v > 0) { debug_printf("%02X ", v); }
            else { debug_printf("%s%02X%s ", TERMCOL_RED, v, TERMCOL_RESET); }

            if (v >= 32 && v <= 126) { chars[j] = v; } else { chars[j] = '.'; }
        }

        debug_printf("  %s%s%s\n", TERMCOL_CYAN, chars, TERMCOL_RESET);
    }
}