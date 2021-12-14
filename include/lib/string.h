#pragma once
#include <lib/types.h>

typedef struct
{
    uint8_t* data;
    uint32_t length;
} string_t;

char*    strcat(char* dest, const char* src);
char*    strncat(char* dest, const char* src, uint32_t n);
char*    strchr(const char* str, int c);
char*    strrchr(const char* str, int c);
int      strcmp(const char* str1, const char* str2);
int      strcmpi(const char* str1, const char* str2);
int      strncmp(const char* str1, const char* str2, uint32_t n);
int      strcoll(const char* str1, const char* str2);
char*    strcpy(char* dest, const char* src);
char*    strncpy(char* dest, const char* src, uint32_t n);
uint32_t strcspn(const char* str1, const char* str2);
uint32_t strlen(const char* str);
char*    strpbrk(const char* str1, const char* str2);
uint32_t strspn(const char* str1, const char* str2);
char*    strstr(const char* haystack, const char* needle);
char*    strtok(char* str, const char* delim);
uint32_t strxfrm(char* dest, const char* src, uint32_t n);
char*    strdup(const char* str);
char*    strlwr(char* str);
char*    strupr(char* str);
char*    strrev(char* str);
char*    strset(char* str, int c);
char*    strnset(char* str, int c, uint32_t n);
char*    strhex(uint32_t value, char* result, bool_t prefix, uint8_t bytes);
bool_t   streql(const char* str1, const char* str2);
char*    stradd(char* str, int c);
char**   strsplit(char* str, char delim, int* count);

string_t string_create(const char* str);
bool_t   string_validate(string_t* str);
bool_t   string_clear(string_t* str);
char*    string_append_char(string_t* str, char c);
char*    string_append_int(string_t* str, uint32_t num, uint8_t base);
char*    string_append(string_t* str, const char* append);