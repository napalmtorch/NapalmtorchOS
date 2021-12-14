#pragma once
#include <lib/types.h>

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);

char*    ftoa(float num, char* str, int afterpoint);
char*    itoa(int num, char* str, int base);
char*    ltoa(uint32_t num, char* str, int base);
double   atof(const char* str);
int      atoi(const char* str);
uint32_t atol(const char* str);
void     srand(uint32_t seed);
int      rand(void);
int      system(const char* str);
int      getchar(void);
char*    gets(char* buffer);
int      printf(const char* format, ...);
int      putchar(int c);
int      puts(const char* str);