#pragma once
#include <lib/types.h>

typedef struct
{
    uint32_t position;
    uint32_t size;
    uint8_t* data;
} bytestream_t;

typedef struct
{
    bytestream_t input;
    bytestream_t output;
} iostream_t;

bytestream_t bytestream_create(uint32_t size, uint8_t* data);
bool_t       bytestream_dispose(bytestream_t* stream);

bool_t  bytestream_clear(bytestream_t* stream);
bool_t  bytestream_ismore(bytestream_t* stream);
bool_t  bytestream_seek(bytestream_t* stream, uint32_t pos);
uint8_t bytestream_peek(bytestream_t* stream);
uint8_t bytestream_peekat(bytestream_t* stream, uint32_t pos);
uint8_t bytestream_next(bytestream_t* stream);

iostream_t iostream_create(uint32_t input_size, uint32_t output_size, uint8_t* input_data, uint8_t* output_data);
bool_t     iostream_dispose(iostream_t* stream);
