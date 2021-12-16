#include "utility/string.h"
#include "utility/debug.h"

string_t string_create(const char* str)
{
    string_t output;
    if (str == NULL)      { output.data = NULL; output.length = 0; return output; }
    if (strlen(str) == 0) { output.data = NULL; output.length = 0; return output; }
    output.length = strlen(str);
    output.data   = malloc(output.length + 1);
    memset(output.data, 0, output.length);
    memcpy(output.data, str, output.length);
    output.data[output.length] = 0;
    return output;
}

bool_t string_validate(string_t* str)
{
    if (str == NULL) { return FALSE; }
    return TRUE;
}

bool_t string_clear(string_t* str)
{
    if (!string_validate(str)) { debug_error("string_clear : %s", DEBUG_MSGS[0]); return FALSE; }
    if (str->data != NULL) { free(str->data); str->data = NULL; }
    str->length = 0;
    return TRUE;
}

string_t* string_set(string_t* str, char* value)
{
    if (!string_clear(str)) { return NULL; }
    string_append(str, value);
    return str;
}

string_t* string_append_char(string_t* str, char c)
{
    if (!string_validate(str)) { debug_error("string_append_char : %s", DEBUG_MSGS[0]); return NULL; }
    char* new_data = malloc(str->length + 2);
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
    return str;
}

string_t* string_append(string_t* str, char* value)
{
    if (!string_validate(str)) { debug_error("string_append : %s", DEBUG_MSGS[0]); return NULL; }

    uint32_t len = strlen(value);
    for (uint32_t i = 0; i < len; i++) { (void)string_append_char(str, value[i]); }
    return str;
}

string_t* string_insert_char(string_t* str, int index, char c)
{
    if (!string_validate(str)) { debug_error("string_insert_char : %s", DEBUG_MSGS[0]); return NULL; }
    char* new_data = malloc(str->length + 2);
    memset(new_data, 0, str->length + 2);

    if (str->data != NULL)
    {
        int new_pos = 0;
        int pos = 0;
        for (uint32_t i = 0; i < str->length + 1; i++)
        {
            if (i == index) { new_pos++; continue; }
            new_data[new_pos] = str->data[pos];
            new_pos++;
            pos++;
        }
        free(str->data);
    }

    new_data[index] = c;
    new_data[str->length + 1] = 0;
    str->data = new_data;
    str->length++;
    return str;
}

string_t* string_insert(string_t* str, int index, char* value)
{
    if (!string_validate(str)) { debug_error("string_insert : %s", DEBUG_MSGS[0]); return NULL; }

    uint32_t len = strlen(value);
    for (uint32_t i = 0; i < len; i++) 
    { 
        (void)string_insert_char(str, index + i, value[i]); 
        
    }
    return str; 
}

string_t* string_delete(string_t* str, int count)
{
    if (!string_validate(str)) { debug_error("string_delete : %s", DEBUG_MSGS[0]); return NULL; }
    if (str->length < count) { debug_error("string_delete : %s", DEBUG_MSGS[1]); return NULL; }
    if (count == 0) { return str; }
    char* new_data = malloc((str->length + 1) - count);
    memset(new_data, 0, (str->length + 1) - count);

    if (str->data != NULL)
    {
        memcpy(new_data, str->data, str->length - count);
        free(str->data);
    }

    str->data = new_data;
    str->length -= count;
    return str;
}

string_t* string_delete_at(string_t* str, int index, int count)
{
    if (!string_validate(str)) { debug_error("string_delete_at : %s", DEBUG_MSGS[0]); return NULL; }
    if (count == 0) { return str; }
    if (index < 0 || index + count > str->length) { debug_error("string_delete_at : %s", DEBUG_MSGS[1]); return NULL; }
    char* new_data = malloc((str->length + 1) - count);
    memset(new_data, 0, (str->length + 1) - count);

    if (str->data != NULL)
    {
        int new_pos = 0;
        int pos = 0;
        for (uint32_t i = 0; i < str->length; i++)
        {
            if (i >= index && i < index + count) { pos++; continue; }
            new_data[new_pos] = str->data[pos];
            new_pos++;
            pos++;
        }
        free(str->data);
    }

    str->data = new_data;
    str->length -= count;
    return str;
}

uint32_t string_length(string_t* str)
{
    if (!string_validate(str)) { debug_error("string_length : %s", DEBUG_MSGS[0]); return 0; }
    return str->length;
}

bool_t string_equals(string_t* str1, string_t* str2) { return streql(str1->data, str2->data); }

bool_t string_equals_c(string_t* str1, const char* str2) { return streql(str1->data, str2); }

bool_t streql(const char* str1, const char* str2) { return strcmp(str1, str2) == 0; }

char* strupper(char* str)
{
    uint32_t len = strlen(str);
    for (uint32_t i = 0; i < len; i++) { str[i] = toupper(str[i]); }
    return str;
}

char* strlower(char* str)
{
    uint32_t len = strlen(str);
    for (uint32_t i = 0; i < len; i++) { str[i] = tolower(str[i]); }
    return str;
}