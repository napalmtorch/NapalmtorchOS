#include "utility/stream.h"
#include "utility/string.h"
#include "utility/debug.h"

stream_t stream_create(uint32_t size, char* data)
{
    stream_t output;
    output.position = 0;
    output.size = size;
    if (data == NULL) { output.data = malloc(size + 1); memset(output.data, 0, size + 1); }
    else { output.data = data; }
    return output;
}

bool_t stream_dispose(stream_t* stream)
{
    if (!stream_validate(stream)) { debug_error("stream_dispose : %s", DEBUG_MSGS[EXCEPTION_NULLPTR]); return FALSE; }
    free(stream->data);
    return TRUE;
}

char stream_next(stream_t* stream)
{
    if (!stream_validate(stream)) { debug_error("stream_next : %s", DEBUG_MSGS[EXCEPTION_NULLPTR]); return 0; }
    if (!stream_has_more(stream)) { return 0; }
    char c = stream->data[stream->position];
    stream->position++;
    return c;
}

char stream_peek(stream_t* stream)
{
    if (!stream_validate(stream)) { debug_error("stream_peek : %s", DEBUG_MSGS[EXCEPTION_NULLPTR]); return 0; }
    if (!stream_has_more(stream)) { return 0; }
    return stream->data[stream->position];
}

char stream_peekat(stream_t* stream, int pos)
{
    if (!stream_validate(stream)) { debug_error("stream_peekat : %s", DEBUG_MSGS[EXCEPTION_NULLPTR]); return 0; }
    if (pos < 0 || pos >= stream->size) { debug_error("stream_peekat : %s", DEBUG_MSGS[EXCEPTION_INDEX_OUTOFRANGE]); return 0; }
    return stream->data[pos];
}

bool_t stream_seek(stream_t* stream, int pos)
{   
    if (!stream_validate(stream)) { debug_error("stream_seek : %s", DEBUG_MSGS[EXCEPTION_NULLPTR]); return FALSE; }
    stream->position = pos;
    return TRUE;
}

bool_t stream_has_more(stream_t* stream)
{
    if (!stream_validate(stream)) { debug_error("stream_has_more : %s", DEBUG_MSGS[EXCEPTION_NULLPTR]); return FALSE; }
    if (stream->data == NULL) { return FALSE; }
    if (stream->data[stream->position] == 0) { return FALSE; }
    if (stream->position >= stream->size) { return FALSE; }
    return TRUE;
}

bool_t stream_validate(stream_t* stream)
{
    if (stream == NULL) { return FALSE; }
    return TRUE;
}