#include "utility/vector.h"
#include "utility/string.h"
#include "utility/debug.h"

vector_t NULL_VECTOR = { 0, 0, 0 };

vector_t vector_create(uint32_t item_size)
{
    if (item_size == 0) { debug_error("vector_create : %s", DEBUG_MSGS[0]); return NULL_VECTOR; }
    vector_t output;
    output.item_size = item_size;
    output.items     = NULL;
    output.count     = 0;
    return output;
}

bool_t vector_validate(vector_t* vec)
{
    if (vec == NULL) { return FALSE; }
    if (vec->item_size == 0) { return FALSE; }
    return TRUE;
}

bool_t vector_clear(vector_t* vec)
{
    if (vec == NULL) { return FALSE; }
    if (vec->item_size == 0) { return FALSE; }
    if (vec->items != NULL) { free(vec->items); vec->items = 0; }
    vec->count = 0;
    return TRUE;
}

bool_t vector_push(vector_t* vec, void* item)
{
    if (!vector_validate(vec)) { debug_error("vector_push : %s", DEBUG_MSGS[0]); return FALSE; }
    uint32_t size = (vec->count + 1) * vec->item_size;
    void* new_items = malloc(size);

    if (vec->items != NULL)
    {
        memcpy(new_items, vec->items, vec->count * vec->item_size);
        free(vec->items);
    }

    void* new_item = (void*)((uint32_t)new_items + (vec->count * vec->item_size));
    memcpy(new_item, item, vec->item_size);

    vec->items = new_items;
    vec->count++;
    return TRUE;
}

void* vector_pop(vector_t* vec)
{
    if (!vector_validate(vec)) { debug_error("vector_pop : %s", DEBUG_MSGS[0]); return NULL; }
    if (vec->count == 0) { debug_error("vector_pop : %s", DEBUG_MSGS[1]); return NULL; }
    uint32_t size = (vec->count - 1) * vec->item_size;

    void* output = vector_peek_at(vec, vec->count - 1);
    if (vec->count > 1)
    {
        void* new_items = malloc(size);

        if (vec->items != NULL)
        {
            memcpy(new_items, vec->items, (vec->count - 1) * vec->item_size);
            free(vec->items);
        }

        vec->items = new_items;
    }
    else { free(vec->items); vec->items = NULL; }

    vec->count--;
    return output;
}

void* vector_peek(vector_t* vec)
{
    if (!vector_validate(vec)) { debug_error("vector_peek_at : %s", DEBUG_MSGS[0]); return NULL; }
    return vector_peek_at(vec, vec->count - 1);
}

void* vector_peek_at(vector_t* vec, int index)
{
    if (!vector_validate(vec)) { debug_error("vector_peek_at : %s", DEBUG_MSGS[0]); return NULL; }
    if (index < 0 || index >= vec->count) { debug_error("vector_peek_at : %s", DEBUG_MSGS[1]); return NULL; }

    uint32_t offset = index * vec->item_size;
    void* output = malloc(vec->item_size);
    memcpy(output, (void*)((uint32_t)vec->items + offset), vec->item_size);
    return output;
}