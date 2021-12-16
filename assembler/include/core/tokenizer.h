#pragma once
#include "utility/types.h"
#include "utility/string.h"
#include "utility/vector.h"
#include "utility/stream.h"

typedef enum
{
    TOKENTYPE_UNKNOWN,
    TOKENTYPE_IDENTIFIER,
    TOKENTYPE_COMMENT,
    TOKENTYPE_INCLUDE,
    TOKENTYPE_INSTRUCTION,
    TOKENTYPE_LABEL,
    TOKENTYPE_CONSTANT,
    TOKENTYPE_VARIABLE,

    TOKENTYPE_LITERAL_DEC,
    TOKENTYPE_LITERAL_HEX,
    TOKENTYPE_LITERAL_BIN,
    TOKENTYPE_LITERAL_CHAR,
    TOKENTYPE_LITERAL_STRING,
    TOKENTYPE_LITERAL_REG,    
} TOKENTYPE;

static inline const char* token_type_string(TOKENTYPE type)
{
    switch (type)
    {
        case TOKENTYPE_IDENTIFIER:      { return "IDENTIFIER "; }
        case TOKENTYPE_COMMENT:         { return "COMMENT    "; }
        case TOKENTYPE_INCLUDE:         { return "INCLUDE    "; }
        case TOKENTYPE_INSTRUCTION:     { return "INSTRUCTION"; }
        case TOKENTYPE_LABEL:           { return "LABEL      "; }
        case TOKENTYPE_CONSTANT:        { return "CONSTANT   "; }
        case TOKENTYPE_VARIABLE:        { return "VARIABLE   "; }
        case TOKENTYPE_LITERAL_DEC:     { return "DECIMAL    "; }
        case TOKENTYPE_LITERAL_HEX:     { return "HEXADECIMAL"; }
        case TOKENTYPE_LITERAL_BIN:     { return "BINARY     "; }
        case TOKENTYPE_LITERAL_CHAR:    { return "CHARACTER  "; }
        case TOKENTYPE_LITERAL_STRING:  { return "STRING     "; }
        case TOKENTYPE_LITERAL_REG:     { return "REGISTER   "; }
        default:                        { return "UNKNOWN"; }
    }
}

typedef struct
{
    TOKENTYPE type;
    uint32_t  line;
    uint32_t  offset;
    char*     value;
} token_t;

typedef struct
{
    vector_t tokens;
    stream_t input;
    string_t filename;
    string_t word;
    uint32_t line;
    uint32_t offset;

} tokenizer_t;

static const char* ASM_MNEMONICS[] = 
{
    "ADD", "SUB", "MUL", "DIV",
    "SHL", "SHR", "OR", "XOR", "AND", "NOT"
    "CMP", "JE", "JNE", "JG", "JGE", "JL", "JLE", "JZ", "JNZ", "JC", "JNC", "JA", "JNA",
    "PUSH", "PUSHA", "POP", "POPA", "JMP", "CALL", "RET",
    "MOV", "IN", "OUT",
};
static const uint32_t ASM_MNEMONICS_COUNT = sizeof(ASM_MNEMONICS) / sizeof(const char*);

static const char* ASM_REG_NAMES[] = 
{
    "EAX", "EBX", "ECX", "EDX",
    "ESI", "EDI", "ESP", "EBP",
    "EIP",
};
static const uint32_t ASM_REG_NAMES_COUNT = sizeof(ASM_REG_NAMES) / sizeof(const char*);

static const char* HEX_VALUES = "0123456789ABCDEF";

token_t token_create(TOKENTYPE type, uint32_t line, uint32_t offset, char* value);

tokenizer_t tokenizer_load(const char* filename);
bool_t      tokenizer_dispose(tokenizer_t* tokenizer);
bool_t      tokenizer_add_token(tokenizer_t* tokenizer, token_t tok);
bool_t      tokenizer_run(tokenizer_t* tokenizer);