#include <lib/stream.h>
#include <kernel/core/kernel.h>

bytestream_t bytestream_create(uint32_t size, uint8_t* data)
{
    bytestream_t stream;
    stream.position  = 0;
    stream.size      = size;

    if (data == NULL) { stream.data = tcalloc(size, MEMSTATE_ARRAY); }
    else { stream.data = data; }

    return stream;
}

bool_t bytestream_validate(bytestream_t* stream)
{
    if (stream == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "bytestream_validate"); return FALSE; }
    if (stream->data == NULL) { panicf(EXCEPTION_NULLPTR, NULL, "bytestream_validate"); return FALSE; }
    return TRUE;
}

bool_t bytestream_dispose(bytestream_t* stream)
{
    if (!bytestream_validate(stream)) { return FALSE; }
    free(stream->data);
    return TRUE;
}

bool_t bytestream_clear(bytestream_t* stream)
{
    if (!bytestream_validate(stream)) { return FALSE; }
    memset(stream->data, 0, stream->size);
    stream->position = 0;
    return TRUE;
}

bool_t bytestream_ismore(bytestream_t* stream)
{
    if (!bytestream_validate(stream)) { return FALSE; }
    if (stream->position >= stream->size) { return FALSE; }
    if (stream->data[stream->position] == 0) { return FALSE; }
    return TRUE;
}

bool_t bytestream_seek(bytestream_t* stream, uint32_t pos)
{
    if (!bytestream_validate(stream)) { return FALSE; }
    if (pos >= stream->size) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "bytestream_seek"); return FALSE; }
    stream->position = pos;
    return TRUE;
}

uint8_t bytestream_peek(bytestream_t* stream)
{
    if (!bytestream_validate(stream)) { return 0; }
    return stream->data[stream->position];
}

uint8_t bytestream_peekat(bytestream_t* stream, uint32_t pos)
{
    if (!bytestream_validate(stream)) { return 0; }
    if (pos >= stream->size) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "bytestream_peekat"); return FALSE; }
    return stream->data[pos];
}

uint8_t bytestream_next(bytestream_t* stream)
{
    if (!bytestream_ismore(stream)) { return 0; }
    stream->position++;
    return stream->data[stream->position - 1];
}

iostream_t iostream_create(uint32_t input_size, uint32_t output_size, uint8_t* input_data, uint8_t* output_data)
{
    iostream_t stream;
    stream.input = bytestream_create(input_size, input_data);
    stream.output = bytestream_create(output_size, output_data);
    return stream;
}

bool_t iostream_dispose(iostream_t* stream)
{
    if (stream == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "iostream_dispose"); return FALSE; }
    if (!bytestream_dispose(&stream->input)) { return FALSE; }
    if (!bytestream_dispose(&stream->output)) { return FALSE; }
    return TRUE;
}