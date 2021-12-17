#pragma once

#define PACKED __attribute__((packed))
#define UNUSED(x) (void)(x)
#define NULL 0

#define FALSE 0
#define TRUE  1

typedef unsigned char        bool_t;

typedef signed char          int8_t;
typedef signed short         int16_t;
typedef signed int           int32_t;
typedef signed long long     int64_t;

typedef unsigned char        uint8_t;
typedef unsigned short       uint16_t;
typedef unsigned int         uint32_t;
typedef unsigned long long   uint64_t;

typedef unsigned int size_t;

#define INT8_MIN  -0x80
#define INT16_MIN -0x8000U
#define INT32_MIN -0x80000000U
#define INT64_MIN -0x8000000000000000L

#define INT8_MAX  0x7FU
#define INT16_MAX 0x7FFFU
#define INT32_MAX 0x7FFFFFFFU
#define INT64_MAX 0x7FFFFFFFFFFFFFFFL

#define UINT8_MAX  0xFFU
#define UINT16_MAX 0xFFFFU
#define UINT32_MAX 0xFFFFFFFFU
#define UINT64_MAX 0xFFFFFFFFFFFFFFFFU

typedef char* va_list;
#define __va_argsiz(t) (((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, pN) ((ap) = ((va_list) (&pN) + __va_argsiz(pN)))
#define va_end(ap)	((void)0)
#define va_arg(ap, t) (((ap) = (ap) + __va_argsiz(t)), *((t*) (void*) ((ap) - __va_argsiz(t))))

#define bits_low16(address) (uint16_t)((address) & 0xFFFF)
#define bits_high16(address) (uint16_t)(((address) & 0xFFFF0000) >> 16)

static inline bool_t bit_address_from_byte(uint32_t to_convert, int to_return)
{
    int mask = 1 << (to_return - 1);
    return ((to_convert & mask) != 0);
}

void lib_init();

void term_clear_col(uint32_t bg);
void term_writechar(char c);
void term_writechar_fg(char c, uint32_t fg);
void term_writechar_col(char c, uint32_t fg, uint32_t bg);
void term_write(const char* txt);
void term_write_fg(const char* txt, uint32_t fg);
void term_write_col(const char* txt, uint32_t fg, uint32_t bg);
void term_writeln(const char* txt);
void term_writeln_fg(const char* txt, uint32_t fg);
void term_writeln_col(const char* txt, uint32_t fg, uint32_t bg);
void term_putchar(int x, int y, char c, uint32_t fg, uint32_t bg);
void term_set_cursor(int x, int y);
void term_set_colors(uint32_t fg, uint32_t bg);
int  term_get_cursor_x();
int  term_get_cursor_y();
uint32_t term_get_fg();
uint32_t term_get_bg();

void* malloc(size_t size);
void* calloc(size_t size);
void free(void* ptr);