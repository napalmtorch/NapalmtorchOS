#pragma once
#include <lib/types.h>
#include <kernel/graphics/fontdata.h>

typedef struct
{
    uint8_t  width;
    uint8_t  height;
    uint8_t  spacing_x;
    uint8_t  spacing_y;
    uint8_t* data;
} PACKED font_t;

static inline font_get_width(font_t font) { return font.width + font.spacing_x; }
static inline font_get_height(font_t font) { return font.height + font.spacing_y; }

static const font_t FONT_SYSTEM_8x8  = { 8, 8,  0, 0, FONTDATA_SYSTEM_8x8 };
static const font_t FONT_SYSTEM_8x16 = { 8, 16, 0, 0, FONTDATA_SYSTEM_8x16 };
static const font_t FONT_SLIM_8x8    = { 8, 8,  1, 0, FONTDATA_SLIM_8x8 };
static const font_t FONT_SLIM_8x16   = { 8, 16, 0, 0, FONTDATA_SLIM_8x16 };
