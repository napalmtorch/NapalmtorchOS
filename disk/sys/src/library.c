#include <sys/lib/library.h>

typedef void*    (*MTYPE_MM_ALLOCATE)(size_t size, bool_t clear, uint8_t state);
typedef void     (*MTYPE_MM_FREE)(void* ptr);
typedef uint32_t (*MTYPE_MM_AMOUNT_INSTALLED)(void);
typedef uint32_t (*MTYPE_MM_AMOUNT_RESERVED)(void);
typedef uint32_t (*MTYPE_MM_AMOUNT_USED)(void);

typedef void (*MTYPE_TERM_CLEAR_COL)(uint32_t bg);
typedef void (*MTYPE_TERM_WRITECHAR_COL)(char c, uint32_t fg, uint32_t bg);
typedef void (*MTYPE_TERM_WRITE_COL)(const char* str, uint32_t fg, uint32_t bg);
typedef void (*MTYPE_TERM_PUTCHAR)(int x, int y, char c, uint32_t fg, uint32_t bg);

typedef void     (*MTYPE_TERM_SET_CURSOR)(int x, int y);
typedef void     (*MTYPE_TERM_SET_COLORS)(uint32_t fg, uint32_t bg);
typedef int      (*MTYPE_TERM_GET_CURSOR_X)(void);
typedef int      (*MTYPE_TERM_GET_CURSOR_Y)(void);
typedef uint32_t (*MTYPE_TERM_GET_FG)(void);
typedef uint32_t (*MTYPE_TERM_GET_BG)(void);

MTYPE_MM_ALLOCATE         MM_ALLOCATE;
MTYPE_MM_FREE             MM_FREE;
MTYPE_MM_AMOUNT_INSTALLED MM_AMOUNT_INSTALLED;
MTYPE_MM_AMOUNT_RESERVED  MM_AMOUNT_RESERVED;
MTYPE_MM_AMOUNT_USED      MM_AMOUNT_USED;

MTYPE_TERM_CLEAR_COL     TERM_CLEAR_COL;
MTYPE_TERM_WRITECHAR_COL TERM_WRITECHAR_COL;
MTYPE_TERM_WRITE_COL     TERM_WRITE_COL;
MTYPE_TERM_PUTCHAR       TERM_PUTCHAR;

MTYPE_TERM_SET_CURSOR    TERM_SET_CURSOR;
MTYPE_TERM_SET_COLORS    TERM_SET_COLORS;
MTYPE_TERM_GET_CURSOR_X  TERM_GET_CURSOR_X;
MTYPE_TERM_GET_CURSOR_Y  TERM_GET_CURSOR_Y;
MTYPE_TERM_GET_FG        TERM_GET_FG;
MTYPE_TERM_GET_BG        TERM_GET_BG;

void lib_init()
{
    void** methods = NULL;
    asm volatile("int $0x80" : : "a"(&methods), "b"(0), "c"(0xFF), "d"(0));

    MM_ALLOCATE         = methods[0];
    MM_FREE             = methods[1];
    MM_AMOUNT_INSTALLED = methods[2];
    MM_AMOUNT_RESERVED  = methods[3];
    MM_AMOUNT_USED      = methods[4];

    TERM_CLEAR_COL      = methods[5];
    TERM_WRITECHAR_COL  = methods[6];
    TERM_WRITE_COL      = methods[7];
    TERM_PUTCHAR        = methods[8];

    TERM_SET_CURSOR     = methods[9];
    TERM_SET_COLORS     = methods[10];
    TERM_GET_CURSOR_X   = methods[11];
    TERM_GET_CURSOR_Y   = methods[12];
    TERM_GET_FG         = methods[13];
    TERM_GET_BG         = methods[14];
}

void term_clear_col(uint32_t bg) { TERM_CLEAR_COL(bg); }

void term_writechar(char c) { TERM_WRITECHAR_COL(c, TERM_GET_FG(), TERM_GET_BG()); }
void term_writechar_fg(char c, uint32_t fg) { TERM_WRITECHAR_COL(c, fg, TERM_GET_BG()); }
void term_writechar_col(char c, uint32_t fg, uint32_t bg) { TERM_WRITECHAR_COL(c, fg, bg); }

void term_write(const char* str) { TERM_WRITE_COL(str, TERM_GET_FG(), TERM_GET_BG()); }
void term_write_fg(const char* str, uint32_t fg) { TERM_WRITE_COL(str, fg, TERM_GET_BG()); }
void term_write_col(const char* str, uint32_t fg, uint32_t bg) { TERM_WRITE_COL(str, fg, bg); }

void term_writeln(const char* str)
{
    term_write(str);
    term_writechar(0x0A);
}

void term_writeln_fg(const char* str, uint32_t fg)
{
    term_write_fg(str, fg);
    term_writechar(0x0A);
}

void term_writeln_col(const char* str, uint32_t fg, uint32_t bg)
{
    TERM_WRITE_COL(str, fg, bg);
    term_writechar(0x0A);
}

void term_putchar(int x, int y, char c, uint32_t fg, uint32_t bg) { TERM_PUTCHAR(x, y, c, fg, bg); }

void term_set_cursor(int x, int y) { TERM_SET_CURSOR(x, y); }

void term_set_colors(uint32_t fg, uint32_t bg) { TERM_SET_COLORS(fg, bg); }

int  term_get_cursor_x() { return TERM_GET_CURSOR_X(); }

int  term_get_cursor_y() { return TERM_GET_CURSOR_Y(); }

uint32_t term_get_fg() { return TERM_GET_FG(); }

uint32_t term_get_bg() { return TERM_GET_BG(); }

void* malloc(size_t size) { return MM_ALLOCATE(size, FALSE, 0x01); }
void* calloc(size_t size) { return MM_ALLOCATE(size, TRUE,  0x01); }
void free(void* ptr) { MM_FREE(ptr); }