#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "utility/types.h"

static const char* TERMCOL_RESET   = "\x1B[0m";
static const char* TERMCOL_RED     = "\x1B[31m";
static const char* TERMCOL_GREEN   = "\x1B[32m";
static const char* TERMCOL_YELLOW  = "\x1B[33m";
static const char* TERMCOL_BLUE    = "\x1B[34m";
static const char* TERMCOL_MAGENTA = "\x1B[35m";
static const char* TERMCOL_CYAN    = "\x1B[36m";
static const char* TERMCOL_WHITE   = "\x1B[37m";

typedef enum
{
    EXCEPTION_NULLPTR,
    EXCEPTION_INDEX_OUTOFRANGE,
    EXCEPTION_FILENOTFOUND,
    EXCEPTION_INVALIDSIZE,
} EXCEPTION;

static const char* DEBUG_MSGS[] = 
{
    "NullPointerException",
    "IndexOutOfRangeException",
    "FileNotFoundException",
    "InvalidSizeException",
};

void debug_init();
void debug_toggle(bool_t state);
void debug_panic();

void debug_clear();
void debug_newline();
void debug_newlines(int count);

void debug_write(const char* str);
void debug_writeln(const char* str);
void debug_printf(const char* str, ...);

void debug_info(const char* str, ...);
void debug_ok(const char* str, ...);
void debug_warn(const char* str, ...);
void debug_error(const char* str, ...);

void debug_dumpmem(uint8_t* data, uint32_t size);