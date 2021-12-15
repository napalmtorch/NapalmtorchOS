#include <lib/memory.h>
#include <kernel/core/kernel.h>

void* memchr(const void* str, int c, uint32_t n)
{
    uint8_t* s = (uint8_t*)str;
    for (uint32_t i = 0; i < n; i++)
    {
        if (s[i] == (uint8_t)c) { return str + i; }
    }
    return NULL;
}

int memcmp(const void* str1, const void* str2, uint32_t n)
{
    uint8_t* p = (uint8_t*)str1;
    uint8_t* q = (uint8_t*)str2;
    int state = 0;

    if (str1 == str2) { return state; }
    while (n > 0)
    {
        if (*p != *q) { state = (*p > *q) ? 1 : -1; break; }
        n--;
        p++;
        q++;
    }
    return state;
}

int memicmp(const void* str1, const void* str2, uint32_t n)
{

}

void* memcpy(void* dest, const void* src, uint32_t n)
{
    asm volatile ("cld; rep movsb" : "+c" (n), "+S" (src), "+D" (dest) :: "memory");
	return dest;
}

void* memmove(void* dest, const void* src, uint32_t n)
{
    return memcpy(dest, src, n);
}

void* memset(void* dest, int c, uint32_t n)
{
    uint32_t num_dwords = n / 4;
    uint32_t num_bytes  = n % 4;
    uint32_t *dest32  = (uint32_t*)dest;
    uint8_t *dest8    = ((uint8_t*)dest) + num_dwords * 4;
    uint8_t val8      = (uint8_t)c;
    uint32_t val32    = c | (c << 8) | (c << 16) | (c << 24);
    uint32_t i;

    for (i = 0; i < num_dwords; i++) { dest32[i] = val32; }
    for (i = 0; i < num_bytes;  i++) { dest8[i]  = val8;  }
    return dest;
}

void* memsetl(void* dest, int c, uint32_t n)
{
    uint32_t len = n / 4;
    asm volatile ("cld; rep stosl" : "+c" (len), "+D" (dest) : "a" (c) : "memory");
	return dest;
}

void* malloc(uint32_t size) { return mm_allocate(size, FALSE, MEMSTATE_USED); }

void* tmalloc(uint32_t size, uint8_t state) { return mm_allocate(size, FALSE, state); }

void* calloc(uint32_t size) { return mm_allocate(size, TRUE, MEMSTATE_USED); }

void* tcalloc(uint32_t size, uint8_t state) { return mm_allocate(size, TRUE, state); }

void* realloc(void* ptr, uint32_t size) { return NULL; }

void free(void* ptr) 
{ 
    if (ptr == NULL) { return; }
    mm_free(ptr); 
}

void freearray(void** ptr, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        if (ptr[i] != NULL) { mm_free(ptr[i]); }
    }
    mm_free(ptr);
}