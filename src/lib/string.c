#include <lib/string.h>
#include <kernel/core/kernel.h>

const char HEX_VALUES[] = "0123456789ABCDEF";

char* strcat(char* dest, const char* src)
{
    if (dest == NULL) { return NULL; }
    if (src == NULL) { return dest; }
    uint32_t src_len = strlen(src);
    uint32_t dest_len = strlen(dest);
    for (uint32_t i = 0; i < src_len; i++) { dest[dest_len + i] = src[i]; }
    dest[strlen(dest)] = 0;
    return dest;
}

char* strncat(char* dest, const char* src, uint32_t n)
{
    if (dest == NULL) { return NULL; }
    if (src == NULL) { return dest; }
    uint32_t dest_len = strlen(dest);
    for (uint32_t i = 0; i < n; i++) { dest[dest_len + i] = src[i]; }
    dest[strlen(dest)] = 0;
    return dest;
}

char* strchr(const char* str, int c)
{
    
}

char* strrchr(const char* str, int c)
{
    
}

int strcmp(const char* str1, const char* str2)
{
    while (*str1)
    {
        if (*str1 != *str2) { break; }
        str1++; str2++;
    }
    return *(char*)str1 - *(char*)str2;
}

int strcmpi(const char* str1, const char* str2)
{
    
}

int strncmp(const char* str1, const char* str2, uint32_t n)
{
    while (n && *str1 && (*str1 == *str2))
    {
        ++str1;
        ++str2;
        --n;
    }
    if (n == 0) { return 0; }
    else { return (*(char*)str1 = *(char*)str2); }
    return 0;
}

int strcoll(const char* str1, const char* str2)
{
    
}

char* strcpy(char* dest, const char* src)
{
    memcpy(dest, src, strlen(src));
    return dest;
}

char* strncpy(char* dest, const char* src, uint32_t n)
{
    for (uint32_t i = 0; i < n; i++) { dest[i] = src[i]; }
    return dest;
}

uint32_t strcspn(const char* str1, const char* str2)
{
    
}

uint32_t strlen(const char* str)
{
    uint32_t len = 0;
    while (str[len] != 0) { len++; }
    return len;
}

char* strpbrk(const char* str1, const char* str2)
{
    
}

uint32_t strspn(const char* str1, const char* str2)
{
    
}

char* strstr(const char* haystack, const char* needle)
{
    
}

char* strtok(char* str, const char* delim)
{
    
}

uint32_t strxfrm(char* dest, const char* src, uint32_t n)
{
    
}

char* strdup(const char* str)
{
    
}

char* strlwr(char* str)
{
    for (uint32_t i = 0; i < strlen(str); i++) { str[i] = tolower(str[i]); }
}

char* strupr(char* str)
{
    for (uint32_t i = 0; i < strlen(str); i++) { str[i] = toupper(str[i]); }
}

char* strrev(char* str)
{
	unsigned char a;
	unsigned len = strlen((const char *)str);
	for (int i = 0, j = len - 1; i < j; i++, j--)
	{
		a = str[i];
		str[i] = str[j];
		str[j] = a;
	}
    return str;
}

char* strset(char* str, int c)
{
    for (uint32_t i = 0; i < strlen(str); i++) { str[i] = c; }
    return str;
}

char* strnset(char* str, int c, uint32_t n)
{
    for (uint32_t i = 0; i < n; i++) { str[i] = c; }
    return str;
}

char* strhex(uint32_t value, char* result, bool_t prefix, uint8_t bytes)
{
    char* orig = result;
    orig[0] = 0;
    if (prefix) { result[0] = '0'; result[1] = 'x'; result += 2; }
    if (bytes == 1)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F))];
    }
    else if (bytes == 2)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF000) >> 12)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F00) >> 8)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00F0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000F))]; 
    }
    else if (bytes == 4)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF0000000) >> 28)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F000000) >> 24)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00F00000) >> 20)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000F0000) >> 16)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0000F000) >> 12)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00000F00) >> 8)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000000F0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0000000F))];
    }
    return orig;
}

bool_t streql(const char* str1, const char* str2)
{
    if (strcmp(str1, str2) == 0) { return TRUE; }
    return FALSE;
}

char* stradd(char* str, int c)
{
    if (str == NULL) { return NULL; }
    uint32_t len = strlen(str);
    str[len] = (char)c;
    str[len + 1] = 0;
    return str;
}

char** strsplit(char* str, char delim, int* count)
{
    if (str == NULL) { return NULL; }
    if (strlen(str) == 0) { return NULL; }

    int len = strlen(str);
    uint32_t num_delimeters = 0;

    for(int i = 0; i < len - 1; i++)
    {
        if(str[i] == delim) { num_delimeters++; }
    }

    uint32_t arr_size = sizeof(char*) * (num_delimeters + 1);
    char** str_array = tcalloc(arr_size, MEMSTATE_STRING);
    int str_offset = 0;

    int start = 0;
    int end = 0;
    while(end < len)
    {
        while(str[end] != delim && end < len) { end++; }

        char* substr = tcalloc(end - start + 1, MEMSTATE_STRING);
        memcpy(substr, str + start, end - start);
        start = end + 1;
        end++;
        str_array[str_offset] = substr;
        str_offset++;
    }

    //return necessary data now
    *count = str_offset;
    return str_array;
}

// -----------------------------------------------------------------------------------------------------------------------

string_t string_create(const char* str)
{
    
    string_t output = { NULL, 0 };
    if (str != NULL)
    {
        output.length = strlen(str);
        if (output.length > 0)
        {
            output.data = tcalloc(output.length + 1, MEMSTATE_STRING);
            memcpy(output.data, str, output.length);
        }
    }
    
    return output;
}

bool_t string_validate(string_t* str)
{
    if (str == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "string_validate"); return FALSE; }
    return TRUE;
}

bool_t string_clear(string_t* str)
{
    if (!string_validate(str)) { return FALSE; }
    if (str->data == NULL) { str->length = 0; return TRUE; }
    free(str->data);
    str->data = NULL;
    str->length = 0;
    return TRUE;
}

char* string_append_char(string_t* str, char c)
{
    if (!string_validate(str)) { return NULL; }
    
    char* new_data = tcalloc(str->length + 2, MEMSTATE_STRING);
    memset(new_data, 0, str->length + 2);

    if (str->data != NULL)
    {
        memcpy(new_data, str->data, str->length);
        uint32_t len = str->length;
        free(str->data);
        str->length = len;
    }

    new_data[str->length] = c;
    new_data[str->length + 1] = 0;
    str->data = new_data;
    str->length++;
    
    return str->data;
}

char* string_append_int(string_t* str, uint32_t num, uint8_t base)
{
    
    if (!string_validate(str)) { return NULL; }
    char append[64];
    ltoa(num, append, base);
    (void)string_append(str, append);
    
    return str->data;
}

char* string_append(string_t* str, const char* append)
{
    
    if (!string_validate(str)) { return NULL; }
    uint32_t new_len = str->length + strlen(append) + 1;
    char* new_data = tcalloc(new_len, MEMSTATE_STRING);
    memset(new_data, 0, str->length + 2);

    if (str->data != NULL)
    {
        memcpy(new_data, str->data, str->length);
        uint32_t len = str->length;
        free(str->data);
        str->length = len;
    }

    memcpy((char*)(new_data + str->length), append, strlen(append));
    str->data = new_data;
    str->length += strlen(append);
    new_data[str->length] = 0;
    
    return str->data;
}