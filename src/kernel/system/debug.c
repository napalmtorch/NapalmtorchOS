#include <kernel/system/debug.h>
#include <kernel/core/kernel.h>

// current debugging modes
DEBUGMODE debug_mode;
DEBUGMODE debug_mode_last;

// set current debugging mode
void debug_setmode(DEBUGMODE mode) 
{ 
    // save current mode
    debug_mode_last = debug_mode;

    // set current mode
    debug_mode = mode; 
}

// return the last debugging mode
void debug_lastmode() { debug_setmode(debug_mode_last); }

// get the current debugging mode
DEBUGMODE debug_getmode() { return debug_mode; }

// disable interrupts and halt execution
void debug_halt() { cli(); hlt(); }

// bochs breakpoint
void debug_breakpoint() { asm volatile("xchgw %bx, %bx"); }

// check if debugging mode supports terminal output
bool_t debug_isterm() { return (debug_mode == DEBUGMODE_TERM) || (debug_mode == DEBUGMODE_ALL); }

// check if debugging mode supports serial output
bool_t debug_isserial() { return (debug_mode == DEBUGMODE_SERIAL) || (debug_mode == DEBUGMODE_ALL); }

// generate new line
void debug_newline()
{
    if (debug_isserial()) { serial_newline(); }
    if (debug_isterm()) { term_newline(); }
}

// write character
void debug_writechar(char c)
{
    if (debug_isserial()) { serial_writechar(c); }
    if (debug_isterm()) { term_writechar(c); }
}

// write string
void debug_write(const char* str)
{
    if (debug_isserial()) { serial_write(str); }
    if (debug_isterm()) { term_write(str); }
}

// write string with specified fore color
void debug_write_col(const char* str, uint8_t color)
{
    if (debug_isserial()) { serial_write_col(str, color); }
    if (debug_isterm()) { term_write_fg(str, color_4bpp_to_32bpp(color)); }
}

// write line
void debug_writeln(const char* str)
{
    if (debug_isserial()) { serial_writeln(str); }
    if (debug_isterm()) { term_writeln(str); }
}

// write line with specified fore color
void debug_writeln_col(const char* str, uint8_t color)
{
    if (debug_isserial()) { serial_writeln_col(str, color); }
    if (debug_isterm()) { term_writeln_fg(str, color_4bpp_to_32bpp(color)); }
}

// write formatted text with manually specified arguments
int debug_vprintf(const char* str, va_list args)
{
    DEBUGMODE old_mode = debug_getmode();

    while (*str != 0)
    {
        if (*str == '%')
        {
            str++;
            if (*str == '%') { debug_writechar('%'); }
            if (*str == 'c') { debug_writechar((char)va_arg(args, int)); }
            else if (*str == 'd' || *str == 'u')
            {
                int dec = va_arg(args, int);
                char str[16];
                memset(str, 0, 16);
                debug_write(itoa(dec, str, 10));
            }
            else if (*str == 'u')
            {
                uint32_t dec = va_arg(args, uint32_t);
                char str[16];
                memset(str, 0, 16);
                debug_write(ltoa(dec, str, 10));
            }
            else if (*str == 'x')
            {
                uint32_t dec = va_arg(args, uint32_t);
                char str[16];
                memset(str, 0, 16);
                debug_write(ltoa(dec, str, 16));
            }
            else if (*str == '2')
            {
                str++;
                if (*str != 'x' && *str != 'X') { debug_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    debug_write(strhex(num, str, FALSE, 1));
                }
            }
            else if (*str == '4')
            {
                str++;
                if (*str != 'x' && *str != 'X') { debug_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    debug_write(strhex(num, str, FALSE, 2));
                }
            }
            else if (*str == '8')
            {
                str++;
                if (*str != 'x' && *str != 'X') { debug_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    debug_write(strhex(num, str, FALSE, 4));
                }
            }
            else if (*str == 'f')
            {
                double num = va_arg(args, double);
                char str[32];
                memset(str, 0, 32);
                debug_write(ftoa(num, str, 2));
            }
            else if (*str == 's')
            {
                char* val = va_arg(args, char*);
                debug_write(val);
            }
            else if (*str == 'a')
            {
                DEBUGMODE mode = va_arg(args, int);
                debug_setmode(mode);
            }
            else { debug_writechar(*str); }
        }
        else { debug_writechar(*str); }
        str++;
    }

    debug_setmode(old_mode);
}

// print formatted string
int debug_printf(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    debug_vprintf(str, args);
    va_end(args);
}

// print debug message header
void debug_header(const char* str, uint8_t color)
{
    debug_write("[");
    debug_write_col(str, color);
    debug_write("] ");
}

// print debug message header and formatted string
void debug_headerf(const char* hstr, uint8_t color, const char* str, va_list args)
{
    DEBUGMODE old_mode = debug_getmode();
    debug_header(hstr, color);
    debug_vprintf(str, args);
    debug_newline();
    debug_setmode(old_mode);
}

// print formatted ok message
void debug_ok(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    debug_headerf("  OK  ", 0x0A, str, args);
    va_end(args);
}

// print formatted info message
void debug_info(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    debug_headerf("  >>  ", 0x0B, str, args);
    va_end(args);
}

// print formatted warning message
void debug_warn(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    debug_headerf("  ??  ", 0x0E, str, args);
    va_end(args);
}

// print formatted error message
void debug_error(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    debug_headerf("  !!  ", 0x0C, str, args);
    va_end(args);
}

// dump specified amount of memory at pointer
void debug_dumpmem(uint8_t* ptr, uint32_t size, DEBUGMODE mode)
{
    DEBUGMODE old = debug_getmode();
    debug_setmode(mode);
    int bytes_per_line = 16;
    char temp[16];
    char chars[bytes_per_line];
    memset(temp, 0, 16);
    memset(chars, 0, bytes_per_line);

    debug_write("Dumping memory at: ");
    strhex((uint32_t)ptr, temp, FALSE, 4);
    debug_write_col(temp, 0x0B);
    debug_newline();

    int xx = 0;
    uint32_t pos = 0;
    for (size_t i = 0; i < (size / bytes_per_line); i++)
    {              
        // address range
        pos = i * bytes_per_line;
        chars[0] = '\0';
        strhex((uint32_t)(ptr + pos), temp, TRUE, 4);
        debug_write_col(temp, 0x0B); debug_write(":");
        strhex((uint32_t)(ptr + pos + (bytes_per_line - 1)), temp, FALSE, 4);
        debug_write_col(temp, 0x0B);
        debug_write("    ");

        // bytes
        for (size_t j = 0; j < bytes_per_line; j++)
        {
            memset(temp, 0, 16);
            strhex(ptr[pos + j], temp, FALSE, 1);
            if (ptr[pos + j] > 0) { debug_write(temp); }
            else { debug_write_col(temp, 0x0C); }
            debug_write(" ");

            if (ptr[pos + j] >= 32 && ptr[pos + j] <= 126) { stradd(chars, ptr[pos + j]); }
            else { stradd(chars, '.'); }
        }

        debug_write("    ");
        debug_write_col(chars, 0x0E);
        debug_newline();
    }
    debug_setmode(old);
}

// print registers struct
void debug_dumpregs(registers_t* regs, DEBUGMODE mode)
{
    DEBUGMODE old = debug_getmode();
    debug_setmode(mode);
    debug_writeln("REGISTERS:");
    char temp[64];
    memset(temp, 0, 64);

    debug_write("EAX: "); debug_write(strhex(regs->eax, temp, TRUE, 4)); debug_write(" "); memset(temp, 0, 64);
    debug_write("EBX: "); debug_write(strhex(regs->ebx, temp, TRUE, 4)); debug_write(" "); memset(temp, 0, 64);
    debug_write("ECX: "); debug_write(strhex(regs->ecx, temp, TRUE, 4)); debug_write(" "); memset(temp, 0, 64);
    debug_write("EDX: "); debug_write(strhex(regs->edx, temp, TRUE, 4)); debug_write(" "); memset(temp, 0, 64);
    debug_newline();
    
    debug_write("ESP: "); debug_write(strhex(regs->esp, temp, TRUE, 4)); debug_write(" "); memset(temp, 0, 64);
    debug_write("EBP: "); debug_write(strhex(regs->ebp, temp, TRUE, 4)); debug_write(" "); memset(temp, 0, 64);
    debug_write("ESI: "); debug_write(strhex(regs->esi, temp, TRUE, 4)); debug_write(" "); memset(temp, 0, 64);
    debug_write("EDI: "); debug_write(strhex(regs->edi, temp, TRUE, 4)); debug_write(" "); memset(temp, 0, 64);
    debug_newline();

    debug_write("EIP: "); debug_writeln(strhex(regs->eip, temp, TRUE, 4)); memset(temp, 0, 64);
    debug_write("EFL: "); debug_writeln(strhex(regs->eflags & 0xFFFF, temp, TRUE, 2)); memset(temp, 0, 64);
    debug_write("CS:  "); debug_writeln(strhex(regs->cs & 0xFFFF, temp, TRUE, 2)); memset(temp, 0, 64);
    debug_write("DS:  "); debug_writeln(strhex(regs->ds & 0xFFFF, temp, TRUE, 2)); memset(temp, 0, 64);
    debug_write("SS:  "); debug_writeln(strhex(regs->ss & 0xFFFF, temp, TRUE, 2)); memset(temp, 0, 64);
    debug_write("INT: "); debug_writeln(strhex(regs->int_no & 0xFFFF, temp, TRUE, 2)); memset(temp, 0, 64);
    debug_write("ERR: "); debug_writeln(strhex(regs->err_code & 0xFFFF, temp, TRUE, 2)); memset(temp, 0, 64);

    debug_setmode(old);
}

// print registers read directly
void debug_dumpregs_raw(DEBUGMODE mode)
{
    register uint32_t ebp asm("ebp");
    register uint32_t esp asm("esp");
    register uint32_t edi asm("edi");
    register uint32_t esi asm("esi");
    register uint32_t ebx asm("ebx");

    register uint32_t eax asm("eax");
    register uint32_t ecx asm("ecx");
    register uint32_t edx asm("edx");

    DEBUGMODE old = debug_getmode();
    debug_setmode(mode);
    debug_write_col("------ ", 0x08);
    debug_write_col("REGISTERS", 0x0E);
    debug_write_col(" ----------------------------------------\n", 0x08);

    char temp[64];
    memset(temp, 0, 64);

    debug_write("EBP: "); debug_write(strhex(ebp, temp, TRUE, 4)); debug_write(" ");
    debug_write("ESP: "); debug_write(strhex(esp, temp, TRUE, 4)); debug_write(" ");
    debug_write("ESI: "); debug_write(strhex(esi, temp, TRUE, 4)); debug_write(" ");
    debug_write("EDI: "); debug_write(strhex(edi, temp, TRUE, 4)); debug_write(" ");
    debug_write("EBX: "); debug_write(strhex(ebx, temp, TRUE, 4)); debug_write(" ");
    debug_newline();

    debug_write("EAX: "); debug_write(strhex(eax, temp, TRUE, 4)); debug_write(" ");
    debug_write("ECX: "); debug_write(strhex(ecx, temp, TRUE, 4)); debug_write(" ");
    debug_write("EDX: "); debug_write(strhex(edx, temp, TRUE, 4)); debug_write(" ");
    debug_newline();
    debug_setmode(old);
}

// print error with exception code
void error(EXCEPTION code)
{
    debug_header("  !!  ", 0x0C);
    debug_writeln(EXCEPTION_MSGS[code]);
}

// print error with exception code and formatted string
void errorf(EXCEPTION code, const char* str, ...)
{
    va_list args;
    va_start(args, str);
    debug_headerf("  !!  ", 0x0C, str, args);
    va_end(args);
}

// kernel panic with exception code and specified registers
void panic(EXCEPTION code, registers_t* regs)
{
    cli();
    error(code);
    if (regs != NULL) { debug_dumpregs(regs, debug_mode); } else { debug_dumpregs_raw(debug_mode); }
    debug_halt();
}

// kernel panic with interrupt exception code and specified registers
void panici(IEXCEPTION code, registers_t* regs)
{
    cli();
    debug_header("  !!  ", 0x0C);
    debug_writeln(IEXCEPTION_MSGS[code]);
    if (regs != NULL) { debug_dumpregs(regs, debug_mode); } else { debug_dumpregs_raw(debug_mode); }
    debug_halt();
}

// kernel panic with exception code, specified registers, and formatted string
void panicf(EXCEPTION code, registers_t* regs, const char* str, ...)
{
    cli();
    va_list args;
    va_start(args, str);
    error(code);
    debug_header("  EX  ", 0x0C);
    debug_vprintf(str, args);
    debug_newline();
    va_end(args);
    if (regs != NULL) { debug_dumpregs(regs, debug_mode); } else { debug_dumpregs_raw(debug_mode); }
    debug_halt();
}
