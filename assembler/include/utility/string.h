#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "utility/types.h"

static const char ASCII_LF  = 0x0A;
static const char ASCII_CR  = 0x0D;
static const char ASCII_EOF = 0x00;
static const char ASCII_CRLF[2] = { ASCII_CR, ASCII_LF };

typedef struct
{
    char*    data;
    uint32_t length;
} string_t;

string_t  string_create(const char* str);

bool_t     string_validate(string_t* str);
bool_t     string_clear(string_t* str);

string_t*  string_set(string_t* str, char* value);

string_t* string_append_char(string_t* str, char c);
string_t* string_append(string_t* str, char* value);

string_t* string_insert_char(string_t* str, int index, char c);
string_t* string_insert(string_t* str, int index, char* value);

string_t* string_delete(string_t* str, int count);
string_t* string_delete_at(string_t* str, int index, int count);

uint32_t   string_length(string_t* str);
bool_t     string_equals(string_t* str1, string_t* str2);
bool_t     string_equals_c(string_t* str1, const char* str2);

bool_t streql(const char* str1, const char* str2);

char*  strupper(char* str);
char*  strlower(char* str);