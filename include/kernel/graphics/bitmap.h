#pragma once
#include <lib/types.h>

typedef struct
{
    uint32_t  width;
    uint32_t  height;
    uint32_t* data;
    uint32_t  size;
} PACKED image_t;

image_t image_create(uint32_t w, uint32_t h);
