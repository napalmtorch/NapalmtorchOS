#pragma once
#include <lib/types.h>
#include <kernel/hardware/interrupts/interrupts.h>

typedef enum
{
    DEBUGMODE_NONE,
    DEBUGMODE_SERIAL,
    DEBUGMODE_TERM,
    DEBUGMODE_ALL,
} DEBUGMODE;

typedef enum
{
    EXCEPTION_UNKKNOWN,
    EXCEPTION_ARG,
    EXCEPTION_ARG_NULL,
    EXCEPTION_ARG_OUTOFRANGE,
    EXCEPTION_FORMAT,
    EXCEPTION_INDEX_OUTOFRANGE,
    EXCEPTION_NOTSUPPORTED,
    EXCEPTION_NULLPTR,
    EXCEPTION_OUTOFMEMORY,
    EXCEPTION_TIMEOUT,
    EXCEPTION_FILENOTFOUND,
    EXCEPTION_CORRUPTEDMEMORY,
    EXCEPTION_COUNT,
} EXCEPTION;

typedef enum
{
    IEXCEPTION_ZERO_DIV,
    IEXCEPTION_DEBUG,
    IEXCEPTION_NMI,
    IEXCEPTION_BREAKPOINT,
    IEXCEPTION_OVERFLOW,
    IEXCEPTION_OUTOFBOUNDS,
    IEXCEPTION_INVALIDOP,
    IEXCEPTION_NO_COPROC,
    IEXCEPTION_DOUBLE_FAULT,
    IEXCEPTION_COPROC_SEG_OVERRUN,
    IEXCEPTION_BAD_TTS,
    IEXCEPTION_NULL_SEG,
    IEXCEPTION_STACK_FAULT,
    IEXCEPTION_GP_FAULT,
    IEXCEPTION_PAGE_FAULT,
    IEXCEPTION_UNKNOWN_INT,
    IEXCEPTION_COPROC_FAULT,
    IEXCEPTION_ALIGNMENT_CHECK,
    IEXCEPTION_MACHINE_CHECK,
    IEXCEPTION_RESERVED0,
    IEXCEPTION_RESERVED1,
    IEXCEPTION_RESERVED2,
    IEXCEPTION_RESERVED3,
    IEXCEPTION_RESERVED4,
    IEXCEPTION_RESERVED5,
    IEXCEPTION_RESERVED6,
    IEXCEPTION_RESERVED7,
    IEXCEPTION_RESERVED8,
    IEXCEPTION_RESERVED9,
    IEXCEPTION_RESERVED10,
    IEXCEPTION_RESERVED11,
    IEXCEPTION_RESERVED12,
    IEXCEPTION_COUNT,
} IEXCEPTION;

static const char* EXCEPTION_MSGS[EXCEPTION_COUNT] = 
{
    "Unknown",
    "ArgumentException",
    "NullArgumentException",
    "ArgumentOutOfRangeException",
    "FormatException",
    "IndexOutOfRangeException",
    "NotSupportedException",
    "NullPointerException",
    "OutOfMemoryException",
    "TimeoutException",
    "FileNotFoundException",
    "MemoryCorruptionException",
};

static const char* IEXCEPTION_MSGS[IEXCEPTION_COUNT] = 
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void      debug_setmode(DEBUGMODE mode);
void      debug_lastmode();
DEBUGMODE debug_getmode();

void      debug_halt();
void      debug_breakpoint();

void debug_newline();
void debug_writechar(char c);
void debug_write(const char* str);
void debug_write_col(const char* str, uint8_t color);
void debug_writeln(const char* str);
void debug_writeln_col(const char* str, uint8_t color);
int  debug_vprintf(const char* str, va_list args);
int  debug_printf(const char* str, ...);

void debug_header(const char* str, uint8_t color);
void debug_ok(const char* str, ...);
void debug_info(const char* str, ...);
void debug_warn(const char* str, ...);
void debug_error(const char* str, ...);

void debug_dumpmem(uint8_t* ptr, uint32_t size, DEBUGMODE mode);
void debug_dumpregs(registers_t* regs, DEBUGMODE mode);
void debug_dumpregs_raw(DEBUGMODE mode);

void error(EXCEPTION code);
void errorf(EXCEPTION code, const char* str, ...);
void panic(EXCEPTION code, registers_t* regs);
void panici(IEXCEPTION code, registers_t* regs);
void panicf(EXCEPTION code, registers_t* regs, const char* str, ...);