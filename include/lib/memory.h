#pragma once
#include <lib/types.h>

void* memchr(const void* str, int c, uint32_t n);
int   memcmp(const void* str1, const void* str2, uint32_t n);
int   memicmp(const void* str1, const void* str2, uint32_t n);
void* memcpy(void* dest, const void* src, uint32_t n);
void* memmove(void* dest, const void* src, uint32_t n);
void* memset(void* dest, int c, uint32_t n);
void* memsetl(void* dest, int c, uint32_t n);

void* malloc(uint32_t size);
void* tmalloc(uint32_t size, uint8_t state);
void* calloc(uint32_t size);
void* tcalloc(uint32_t size, uint8_t state);
void* realloc(void* ptr, uint32_t size);
void  free(void* ptr);
void  freearray(void** ptr, uint32_t count);