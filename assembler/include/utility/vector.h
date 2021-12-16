#pragma once
#include <stdint.h>
#include <stddef.h>
#include "utility/types.h"

typedef struct
{
    void*    items;
    uint32_t item_size;
    uint32_t count;
} vector_t;

vector_t vector_create(uint32_t item_size);

bool_t    vector_validate(vector_t* vec);
bool_t    vector_clear(vector_t* vec);

bool_t    vector_push(vector_t* vec, void* item);
void*     vector_pop(vector_t* vec);
void*     vector_peek(vector_t* vec);
void*     vector_peek_at(vector_t* vec, int index);