#pragma once
#include "utility/types.h"

typedef struct
{
    int      position;
    uint32_t size;
    char*    data;
} stream_t;

stream_t stream_create(uint32_t size, char* data);
bool_t   stream_dispose(stream_t* stream);

char     stream_next(stream_t* stream);
char     stream_peek(stream_t* stream);
char     stream_peekat(stream_t* stream, int pos);

bool_t   stream_seek(stream_t* stream, int pos);
bool_t   stream_has_more(stream_t* stream);
bool_t   stream_validate(stream_t* stream);