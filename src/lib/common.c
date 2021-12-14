#include <lib/common.h>
#include <kernel/core/kernel.h>

const char     PUNCT_CHARS[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
const char*    ITOA_STR = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
const uint32_t LTOA_DIGITS = 32;

int isalnum(int c)
{ 
    if (c >= 'A' && c <= 'Z') { return TRUE; }
    if (c >= 'a' && c <= 'z') { return TRUE; }
    if (c >= '0' && c <= '9') { return TRUE; }
    return FALSE;
}

int isalpha(int c)
{
    if (c >= 'A' && c <= 'Z') { return TRUE; }
    if (c >= 'a' && c <= 'z') { return TRUE; }
    return FALSE;
}

int iscntrl(int c)
{ 
    if (c >= 0 && c <= 31) { return TRUE; }
    if (c == 127) { return TRUE; }
    return FALSE;
}

int isdigit(int c)
{
    if (c >= '0' && c <= '9') { return TRUE; }
    return FALSE;
}

int isgraph(int c)
{ 
    if (c == ' ') { return FALSE; }
    if (isprint(c)) { return TRUE; }
    return FALSE;
}

int islower(int c)
{ 
    if (c >= 'a' && c <= 'z') { return TRUE; }
    return FALSE;
}

int isprint(int c)
{ 
    if (!iscntrl(c)) { return TRUE; } 
    return FALSE;
}

int ispunct(int c)
{ 
    for (uint32_t i = 0; i < 32; i++) { if (c == PUNCT_CHARS[i]) { return TRUE; } }
    return FALSE;
}

int isspace(int c)
{ 
    if (c == 0x09 || (c >= 0x0A && c <= 0x0D)) { return TRUE; }
    if (c == 0x20) { return TRUE; }
    return FALSE;
}

int isupper(int c)
{ 
    if (c >= 'A' && c <= 'Z') { return TRUE; }
    return FALSE;
}

int isxdigit(int c)
{ 
    if (c >= 'A' && c <= 'F') { return TRUE; }
    if (c >= 'a' && c <= 'f') { return TRUE; }
    if (c >= '0' && c <= '9') { return TRUE; }
    return FALSE;
}

int tolower(int c)
{
    if (c >= 'A' && c <= 'Z') { return c + 32; }
    return c;
}

int toupper(int c)
{ 
    if (c >= 'a' && c <= 'z') { return c - 32; }
    return c;
}

void strrevl(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) 
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int int2str(int x, char str[], int d)
{
    int i = 0;
    while (x) { str[i++] = (x % 10) + '0'; x = x / 10; }
    while (i < d) { str[i++] = '0'; }
    strrevl(str, i);
    str[i] = '\0';
    return i;
}


char* ftoa(float num, char* str, int afterpoint)
{
    int ipart = (int)num;
    float fpart = num - (float)ipart;
    int i = int2str(ipart, str, 0);
    if (afterpoint != 0)
    {
        str[i] = '.';
        fpart = fpart * powd(10.0f, afterpoint);
        int2str((int)fpart, str + i + 1, afterpoint);
    }
    return str;
}

char* itoa(int num, char* str, int base)
{
    if (base < 2 || base > 36) { *str = '\0'; return str; }
    char* ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    do {
        tmp_value = num;
        num /= base;
        *ptr++ = ITOA_STR[35 + (tmp_value - num * base)];
    } while (num);

    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return str;
}

void ultoa(unsigned long value, char* result, int base)
{
    unsigned char index;
    char buffer[LTOA_DIGITS];
    index = LTOA_DIGITS;
    do 
    {
        buffer[--index] = '0' + (value % base);
        if ( buffer[index] > '9') { buffer[index] += 'A' - ':'; }
        value /= base;
    } while (value != 0);

    do { *result++ = buffer[index++]; } while (index < LTOA_DIGITS);
    *result = 0;
}

char* ltoa(uint32_t num, char* str, int base)
{
    if (num < 0 && base == 10) 
    {
        *str++ = '-';
        ultoa(-num, str, base);
        return str;
    }
    ultoa(num, str, base);
    return str;
}

double atof(const char* str) { return 0; }
int atoi(const char* str) { return 0; }

uint32_t atol(const char* str)
{
    for (size_t i = 0; i < strlen(str); i++)
    { if (str[i] < 48 || str[i] > 57) { return 0; } }
    int res = 0;
    for (int i = 0; str[i] != 0; i++) { res = res * 10 + str[i] - '0'; }
    return res;
}

void srand(uint32_t seed) { }
int rand(void) { return 0; }
int system(const char* str) { return 0; }
int getchar(void) { return 0; }
char* gets(char* buffer) { return NULL; }
int printf(const char* format, ...) { return 0; }
int putchar(int c) { return 0; }
int puts(const char* str) { return 0; }