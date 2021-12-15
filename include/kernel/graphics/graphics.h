#pragma once
#include <lib/types.h>
#include <kernel/graphics/color.h>
#include <kernel/graphics/font.h>
#include <kernel/graphics/bitmap.h>

typedef struct
{
    int x;
    int y;
} PACKED point_t;

static inline bool_t point_equals(point_t a, point_t b) { return a.x == b.x && a.y == b.y; }
static inline bool_t point_equals_xy(point_t a, int x, int y) { return a.x == x && a.y == y; }

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} PACKED rectangle_t;

static inline bool_t rectangle_equals(rectangle_t a, rectangle_t b) { return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height; }
static inline bool_t rectangle_equals_xywh(rectangle_t a, int x, int y, int w, int h) { return a.x == x && a.y == y && a.width == w && a.height == h; }
static inline bool_t rectangle_intersects(rectangle_t a, rectangle_t b) { return b.x >= a.x && b.x < a.x + a.width && b.y >= a.y && b.y < a.y + a.height; }
static inline bool_t rectangle_intersects_xywh(rectangle_t a, int x, int y, int w, int h) { return a.x >= x && a.x < x + w && a.y >= y && a.y < y + h; }

void gfx_clear(uint32_t color);

void gfx_blit(int x, int y, uint32_t color);
void gfx_blitx(point_t pos, uint32_t color);

void gfx_filledrect(int x, int y, int w, int h, uint32_t color);
void gfx_filledrectx(rectangle_t rect, uint32_t color);

void gfx_rect(int x, int y, int w, int h, int thickness, uint32_t color);
void gfx_rectx(rectangle_t rect, int thickness, uint32_t color);

void gfx_char(int x, int y, char c, uint32_t fg, font_t font);
void gfx_charx(point_t pos, char c, uint32_t fg, font_t font);
void gfx_char_bg(int x, int y, char c, uint32_t fg, uint32_t bg, font_t font);
void gfx_charx_bg(point_t pos, char c, uint32_t fg, uint32_t bg, font_t font);

void gfx_string(int x, int y, const char* txt, uint32_t fg, font_t font);
void gfx_stringx(point_t pos, const char* txt, uint32_t fg, font_t font);
void gfx_string_bg(int x, int y, const char* txt, uint32_t fg, uint32_t bg, font_t font);
void gfx_stringx_bg(point_t pos, const char* txt, uint32_t fg, uint32_t bg, font_t font);

void gfx_array(int x, int y, int w, int h, uint32_t* data);
void gfx_arrayx(rectangle_t rect, uint32_t* data);
void gfx_arraytrans(int x, int y, int w, int h, uint32_t trans, uint32_t* data);
void gfx_arraytrans_x(rectangle_t rect, uint32_t trans, uint32_t* data);

void gfx_bmp(int x, int y, int w, int h, uint32_t* data);

void image_clear(uint32_t color, image_t image);

void image_blit(int x, int y, uint32_t color, image_t image);
void image_blitx(point_t pos, uint32_t color, image_t image);

void image_filledrect(int x, int y, int w, int h, uint32_t color, image_t image);
void image_filledrectx(rectangle_t rect, uint32_t color, image_t image);

void image_rect(int x, int y, int w, int h, int thickness, uint32_t color, image_t image);
void image_rectx(rectangle_t rect, int thickness, uint32_t color, image_t image);

void image_char(int x, int y, char c, uint32_t fg, font_t font, image_t image);
void image_charx(point_t pos, char c, uint32_t fg, font_t font, image_t image);
void image_char_bg(int x, int y, char c, uint32_t fg, uint32_t bg, font_t font, image_t image);
void image_charx_bg(point_t pos, char c, uint32_t fg, uint32_t bg, font_t font, image_t image);

void image_string(int x, int y, const char* txt, uint32_t fg, font_t font, image_t image);
void image_stringx(point_t pos, const char* txt, uint32_t fg, font_t font, image_t image);
void image_string_bg(int x, int y, const char* txt, uint32_t fg, uint32_t bg, font_t font, image_t image);
void image_stringx_bg(point_t pos, const char* txt, uint32_t fg, uint32_t bg, font_t font, image_t image);

void image_array(int x, int y, int w, int h, uint32_t* data, image_t image);
void image_arrayx(rectangle_t rect, uint32_t* data, image_t image);
void image_arraytrans(int x, int y, int w, int h, uint32_t trans, uint32_t* data, image_t image);
void image_arraytrans_x(rectangle_t rect, uint32_t trans, uint32_t* data, image_t image);
