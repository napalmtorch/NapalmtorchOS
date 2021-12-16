#include "core/tokenizer.h"
#include "core/util.h"
#include "utility/debug.h"

tokenizer_t TOKENIZER_NULL = { 0 };

bool_t tokenizer_handle(tokenizer_t* tokenizer);
bool_t tokenizer_handle_word(tokenizer_t* tokenizer);
bool_t tokenizer_handle_comment(tokenizer_t* tokenizer);
bool_t tokenizer_handle_string(tokenizer_t* tokenizer);
bool_t tokenizer_handle_char(tokenizer_t* tokenizer);
bool_t tokenizer_handle_include(tokenizer_t* tokenizer);


token_t token_create(TOKENTYPE type, uint32_t line, uint32_t offset, char* value)
{
    token_t output;
    output.type = type;
    output.line = line;
    output.offset = offset;
    output.value = malloc(strlen(value) + 1);
    strcpy(output.value, value);
    return output;
}

tokenizer_t tokenizer_load(const char* filename)
{
    // attempt to load file
    FILE* fptr = fopen(filename, "rb");
    if (fptr == NULL) { debug_error("tokenizer_load : %s", DEBUG_MSGS[EXCEPTION_FILENOTFOUND]); return TOKENIZER_NULL; }
    fseek(fptr, 0, SEEK_END);
    uint32_t size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    if (size == 0) { debug_error("tokenizer_load : s", DEBUG_MSGS[EXCEPTION_INVALIDSIZE]); return TOKENIZER_NULL; }

    tokenizer_t tokenizer;
    tokenizer.filename = string_create("");
    tokenizer.input    = stream_create(1, NULL);
    tokenizer.word     = string_create("");
    tokenizer.tokens   = vector_create(sizeof(token_t));
    tokenizer.line     = 1;
    tokenizer.offset   = 0;

    // reset filename
    string_clear(&tokenizer.filename);
    tokenizer.filename = string_create(filename);

    // reset input stream
    stream_dispose(&tokenizer.input);
    tokenizer.input = stream_create(size, NULL);
    fread(tokenizer.input.data, size, 1, fptr);

    // reset token list
    vector_clear(&tokenizer.tokens);
    tokenizer.tokens = vector_create(sizeof(token_t));

    debug_info("Loaded file '%s'", tokenizer.filename);
    return tokenizer;
}

bool_t tokenizer_dispose(tokenizer_t* tokenizer)
{
    if (tokenizer == NULL) { return FALSE; }
    string_clear(&tokenizer->filename);
    string_clear(&tokenizer->word);
    stream_dispose(&tokenizer->input);

    for (uint32_t i = 0; i < tokenizer->tokens.count; i++)
    {
        token_t* tok = vector_peek_at(&tokenizer->tokens, i);
        if (tok == NULL) { continue; }
        if (tok->value != NULL) { free(tok->value); }
        free(tok);
    }

    vector_clear(&tokenizer->tokens);
    debug_info("Disposed tokenizer");
    return TRUE;
}

bool_t tokenizer_add_token(tokenizer_t* tokenizer, token_t tok)
{
    if (!vector_push(&tokenizer->tokens, (void*)&tok)) { return FALSE; }
    debug_info("Added token: TYPE = %s, LINE = %d, OFFSET = %d, VALUE = '%s'", token_type_string(tok.type), tok.line, tok.offset, tok.value);
    return TRUE;
}

bool_t tokenizer_run(tokenizer_t* tokenizer)
{
    while (TRUE)
    {
        if (!tokenizer_handle(tokenizer)) { break; }
        if (!stream_has_more(&tokenizer->input)) { debug_ok("Finished tokenizing file '%s'", tokenizer->filename); return TRUE; }
    }

    debug_error("Failed to tokenize file '%s'", tokenizer->filename);
    return FALSE;
}

bool_t tokenizer_handle(tokenizer_t* tokenizer)
{
    char c = stream_next(&tokenizer->input);

    switch (c)
    {
        case '\r':  { break; }
        case '\t':  { string_clear(&tokenizer->word); break; }
        case ';':   { if (!tokenizer_handle_comment(tokenizer)) { return FALSE; } tokenizer->line++; tokenizer->offset = 0; break; }
        case '\n':  { if (!tokenizer_handle_word(tokenizer)) { return FALSE; } tokenizer->line++; tokenizer->offset = 0; break; }
        case '\0':  { if (!tokenizer_handle_word(tokenizer)) { return FALSE; } tokenizer->line++; tokenizer->offset = 0; break; }
        case ',':   { if (!tokenizer_handle_word(tokenizer)) { return FALSE; } break; }
        case ' ':   { if (!tokenizer_handle_word(tokenizer)) { return FALSE; } break; }
        case '\"':  { if (!tokenizer_handle_string(tokenizer)) { return FALSE; } break; }
        case '\'':  { if (!tokenizer_handle_char(tokenizer)) { return FALSE; } break; }
        case '%':   { if (!tokenizer_handle_include(tokenizer)) { return FALSE; } break; }
        default:    { string_append_char(&tokenizer->word, c); tokenizer->offset++; break; }
    }

    return TRUE;
}

bool_t tokenizer_handle_word(tokenizer_t* tokenizer)
{
    if (tokenizer->word.length == 0) { return TRUE; }
    if (string_equals_c(&tokenizer->word, " ")) { string_clear(&tokenizer->word); return TRUE; }
    char* word_upper = malloc(tokenizer->word.length + 1);
    strcpy(word_upper, tokenizer->word.data);
    strupper(word_upper);

    stream_seek(&tokenizer->input, tokenizer->input.position - 1);
    char c = stream_peek(&tokenizer->input);
    if (c == ',') { (void)stream_next(&tokenizer->input); }
    (void)stream_next(&tokenizer->input);

    for (uint32_t i = 0; i < ASM_MNEMONICS_COUNT; i++)
    {
        if (streql(word_upper, ASM_MNEMONICS[i]))
        {
            token_t tok = token_create(TOKENTYPE_INSTRUCTION, tokenizer->line, tokenizer->offset, word_upper);
            if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
            string_clear(&tokenizer->word);
            free(word_upper);
            return TRUE;
        }
    }

    if (string_is_register(word_upper))
    {
        token_t tok = token_create(TOKENTYPE_LITERAL_REG, tokenizer->line, tokenizer->offset, word_upper);
        if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
        string_clear(&tokenizer->word);
        free(word_upper);
        return TRUE;
    }

    if (streql(word_upper, "EQU"))
    {
        token_t tok = token_create(TOKENTYPE_CONSTANT, tokenizer->line, tokenizer->offset, word_upper);
        if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
        string_clear(&tokenizer->word);
        free(word_upper);
        return TRUE;
    }

    if (streql(word_upper, "DD"))
    {
        token_t tok = token_create(TOKENTYPE_VARIABLE, tokenizer->line, tokenizer->offset, word_upper);
        if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
        string_clear(&tokenizer->word);
        free(word_upper);
        return TRUE;
    }

    if (word_upper[strlen(word_upper) - 1] == ':')
    {
        tokenizer->word.data[tokenizer->word.length - 1] = 0;
        token_t tok = token_create(TOKENTYPE_LABEL, tokenizer->line, tokenizer->offset, tokenizer->word.data);
        if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
        string_clear(&tokenizer->word);
        free(word_upper);
        return TRUE;
    }

    bool_t dec = TRUE;
    for (uint32_t i = 0; i < strlen(word_upper); i++) { if (!isdigit(word_upper[i])) { dec = FALSE; break; } }
    if (dec)
    {
        token_t tok = token_create(TOKENTYPE_LITERAL_DEC, tokenizer->line, tokenizer->offset, tokenizer->word.data);
        if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
        string_clear(&tokenizer->word);
        free(word_upper);
        return TRUE;
    }

    bool_t hex = TRUE;
    if (strlen(word_upper) > 2)
    {
        if (word_upper[0] == '0' && (word_upper[1] == 'X'))
        {
            string_t str = string_create(word_upper + 2);
            free(word_upper);
            word_upper = str.data;
        }

        for (uint32_t i = 0; i < strlen(word_upper); i++)
        {
            bool_t found = FALSE;
            for (uint32_t j = 0; j < 16; j++) { if (word_upper[i] == HEX_VALUES[j]) { found = TRUE; break; } }
            if (!found) { hex = FALSE; break; }
        }
    }
    if (hex)
    {
        token_t tok = token_create(TOKENTYPE_LITERAL_HEX, tokenizer->line, tokenizer->offset, word_upper);
        if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
        string_clear(&tokenizer->word);
        free(word_upper);
        return TRUE;
    }

    token_t tok = token_create(TOKENTYPE_IDENTIFIER, tokenizer->line, tokenizer->offset, tokenizer->word.data);
    if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
    string_clear(&tokenizer->word);
    free(word_upper);
    return TRUE;
}

bool_t tokenizer_handle_comment(tokenizer_t* tokenizer)
{
    //string_t comment = string_create("");

    while (TRUE)
    { 
        char c = stream_next(&tokenizer->input);
        if (!stream_has_more(&tokenizer->input)) { break; }
        if (c == '\n') { break; }
        //string_append_char(&comment, c);
    }

    //token_t tok = token_create(TOKENTYPE_COMMENT, tokenizer->line, tokenizer->offset, comment.data);
    //if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
    //string_clear(&tokenizer->word);
    //string_clear(&comment);
    return TRUE;
}

bool_t tokenizer_handle_string(tokenizer_t* tokenizer)
{
    string_t value = string_create("");

    while (TRUE)
    {
        char c = stream_next(&tokenizer->input);
        if (c == '\"') { break; }
        string_append_char(&value, c);
    }

    token_t tok = token_create(TOKENTYPE_LITERAL_STRING, tokenizer->line, tokenizer->offset, value.data);
    if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
    string_clear(&tokenizer->word);
    string_clear(&value);
    return TRUE;
}

bool_t tokenizer_handle_char(tokenizer_t* tokenizer)
{
    string_t value = string_create("");

    while (TRUE)
    {
        char c = stream_next(&tokenizer->input);
        if (c == '\'') { break; }
        string_append_char(&value, c);
    }

    token_t tok = token_create(TOKENTYPE_LITERAL_CHAR, tokenizer->line, tokenizer->offset, value.data);
    if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
    string_clear(&tokenizer->word);
    string_clear(&value);
    return TRUE;
}   

bool_t tokenizer_handle_include(tokenizer_t* tokenizer)
{
    stream_seek(&tokenizer->input, tokenizer->input.position + 8);
	token_t tok = token_create(TOKENTYPE_INCLUDE, tokenizer->line, tokenizer->offset, "%INCLUDE");
    if (!tokenizer_add_token(tokenizer, tok)) { free(tok.value); return FALSE; }
    string_clear(&tokenizer->word);
    return TRUE;
}
