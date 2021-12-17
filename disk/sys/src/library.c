#include <sys/lib/library.h>

typedef void*    (*MTYPE_MM_ALLOCATE)(size_t size, bool_t clear, uint8_t state);
typedef void     (*MTYPE_MM_FREE)(void* ptr);
typedef uint32_t (*MTYPE_MM_AMOUNT_INSTALLED)(void);
typedef uint32_t (*MTYPE_MM_AMOUNT_RESERVED)(void);
typedef uint32_t (*MTYPE_MM_AMOUNT_USED)(void);

typedef void (*MTYPE_TERM_CLEAR_COL)(uint32_t bg);
typedef void (*MTYPE_TERM_WRITECHAR_COL)(char c, uint32_t fg, uint32_t bg);
typedef void (*MTYPE_TERM_WRITE_COL)(const char* str, uint32_t fg, uint32_t bg);

MTYPE_MM_ALLOCATE         MM_ALLOCATE;
MTYPE_MM_FREE             MM_FREE;
MTYPE_MM_AMOUNT_INSTALLED MM_AMOUNT_INSTALLED;
MTYPE_MM_AMOUNT_RESERVED  MM_AMOUNT_RESERVED;
MTYPE_MM_AMOUNT_USED      MM_AMOUNT_USED;

MTYPE_TERM_CLEAR_COL     TERM_CLEAR_COL;
MTYPE_TERM_WRITECHAR_COL TERM_WRITECHAR_COL;
MTYPE_TERM_WRITE_COL     TERM_WRITE_COL;

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
}

void term_clear_col(uint32_t bg) { TERM_CLEAR_COL(bg); }
void term_writechar_col(char c, uint32_t fg, uint32_t bg) { TERM_WRITECHAR_COL(c, fg, bg); }
void term_write_col(const char* str, uint32_t fg, uint32_t bg) { TERM_WRITE_COL(str, fg, bg); }

void* malloc(size_t size) { return MM_ALLOCATE(size, FALSE, 0x01); }
void* calloc(size_t size) { return MM_ALLOCATE(size, TRUE,  0x01); }
void free(void* ptr) { MM_FREE(ptr); }