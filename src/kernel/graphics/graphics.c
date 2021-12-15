#include <kernel/graphics/graphics.h>
#include <kernel/core/kernel.h>

void gfx_clear(uint32_t color) { vesa_clear(color); }

void gfx_blit(int x, int y, uint32_t color)
{
    if ((uint32_t)x >= vesa_get_width() || (uint32_t)y >= vesa_get_height()) { return; }
    vesa_setpixel(x, y, color);
}

void gfx_blitx(point_t pos, uint32_t color)
{
    if ((uint32_t)pos.x >= vesa_get_width() || (uint32_t)pos.y >= vesa_get_height()) { return; }
    vesa_setpixel(pos.x, pos.y, color);
}

void gfx_filledrect(int x, int y, int w, int h, uint32_t color)
{
    for (uint32_t yy = 0; yy < h; yy++)
    {
        uint32_t dest_offset = (((y + yy) * vesa_get_width()) + x);
        uint32_t src_offset = (yy * w) * 4;
        memsetl(vesa_get_buffer() + dest_offset, color, w * 4);
    }
}

void gfx_filledrectx(rectangle_t rect, uint32_t color) { gfx_filledrect(rect.x, rect.y, rect.width, rect.height, color); }

void gfx_rect(int x, int y, int w, int h, int thickness, uint32_t color)
{
    gfx_filledrect(x, y, w, thickness, color);
    gfx_filledrect(x, y + h - thickness, w, thickness, color);
    gfx_filledrect(x, y + thickness, thickness, h - (thickness * 2), color);
    gfx_filledrect(x + w - thickness, y + thickness, thickness, h - (thickness * 2), color);
}

void gfx_rectx(rectangle_t rect, int thickness, uint32_t color) { gfx_rect(rect.x, rect.y, rect.width, rect.height, thickness, color); }

void gfx_char(int x, int y, char c, uint32_t fg, font_t font)
{
    if (c == 0) { return; }
    uint32_t p = font.height * c;
    for (size_t cy = 0; cy < font.height; cy++)
    {
        for (size_t cx = 0; cx < font.width; cx++)
        {
            if (bit_address_from_byte(font.data[p + cy], cx + 1))
            { gfx_blit(x + (font.width - cx), y + cy, fg); }
        }
    }
}

void gfx_charx(point_t pos, char c, uint32_t fg, font_t font) { gfx_char(pos.x, pos.y, c, fg, font); }

void gfx_char_bg(int x, int y, char c, uint32_t fg, uint32_t bg, font_t font)
{
    gfx_filledrect(x, y, font_get_width(font), font_get_height(font), bg);
    
    uint32_t p = font.height * c;
    for (size_t cy = 0; cy < font.height; cy++)
    {
        for (size_t cx = 0; cx < font.width; cx++)
        {
            if (bit_address_from_byte(font.data[p + cy], cx + 1))
            { gfx_blit(x + (font.width - cx), y + cy, fg); }
        }
    }
}

void gfx_charx_bg(point_t pos, char c, uint32_t fg, uint32_t bg, font_t font) { gfx_char_bg(pos.x, pos.y, c, fg, bg, font); }

void gfx_string(int x, int y, const char* txt, uint32_t fg, font_t font)
{
    uint32_t len = strlen(txt);
    uint32_t xx = x, yy = y;
    for (uint32_t i = 0; i < len; i++)
    {
        if (txt[i] == 0) { break; }
        if (txt[i] == '\n') { xx = x; yy += font_get_height(font); continue; }
        if (isprint(txt[i])) { gfx_char(xx, yy, txt[i], fg, font); xx += font_get_width(font); }
    }
}

void gfx_stringx(point_t pos, const char* txt, uint32_t fg, font_t font) { gfx_string(pos.x, pos.y, txt, fg, font); }

void gfx_string_bg(int x, int y, const char* txt, uint32_t fg, uint32_t bg, font_t font)
{
    uint32_t len = strlen(txt);
    uint32_t xx = x, yy = y;
    for (uint32_t i = 0; i < len; i++)
    {
        if (txt[i] == 0) { break; }
        if (txt[i] == '\n') { xx = x; yy += font_get_height(font); continue; }
        if (isprint(txt[i])) { gfx_char_bg(xx, yy, txt[i], fg, bg, font); xx += font_get_width(font); }
    }
}

void gfx_stringx_bg(point_t pos, const char* txt, uint32_t fg, uint32_t bg, font_t font) { gfx_string_bg(pos.x, pos.y, txt, fg, bg, font); }

void gfx_array(int x, int y, int w, int h, uint32_t* data)
{
    if (data == NULL) { return; }
    while (x + w > vesa_get_width()) { w--; }
    while (y + h > vesa_get_height()) { h--; }

    for (int yy = 0; yy < h; yy++)
    {
        uint8_t* src = (uint8_t*)(data + (yy * w));
        int xx = x;
        if (xx < 0) { xx = 0; }
        while (xx + w > vesa_get_width()) { xx--; }
        uint32_t real_offset = (xx + ((y + yy) * vesa_get_width())) * 4;
        uint8_t* dest = (uint8_t*)((uint32_t)vesa_get_buffer() + real_offset);
        if (y + yy >= vesa_get_height()) { return; }
        if (y + yy >= 0 && dest >= vesa_get_buffer() && dest < (uint8_t*)((uint32_t)vesa_get_buffer() + (vesa_get_width() * vesa_get_height() * 4)))
        {
            if (x >= 0) { memcpy(dest, src, w * 4); }
            else { memcpy(dest, src - (x * 4), (w + x) * 4); }
        }
    }
}

void gfx_bmp(int x, int y, int w, int h, uint32_t* data)
{
    for (uint32_t yy = 0; yy < h; yy++)
    {
        uint32_t dest_offset = (((y + yy) * vesa_get_width()) + x);
        uint32_t src_offset = (yy * w) * 4;
        memcpy(vesa_get_buffer() + dest_offset, data + (src_offset / 4), w * 4);
    }
}

void gfx_arrayx(rectangle_t rect, uint32_t* data)
{

}

void gfx_arraytrans(int x, int y, int w, int h, uint32_t trans, uint32_t* data)
{

}

void gfx_arraytrans_x(rectangle_t rect, uint32_t trans, uint32_t* data)
{

}

// ----------------------------------------------------------------------------------------------------

void image_clear(uint32_t color, image_t image)
{
    uint32_t  dest       = (uint32_t)image.data;
    uint32_t  num_dwords = image.size / 4;
    uint32_t  num_bytes  = image.size % 4;
    uint32_t* dest32     = (uint32_t*)dest;
    uint8_t*  dest8      = ((uint8_t*)dest) + num_dwords * 4;
    uint8_t   val8       = (uint8_t)color;
    uint32_t  i;

    for (i = 0;i < num_dwords;i++) { dest32[i] = color; }
    for (i = 0;i < num_bytes;i++)  { dest8[i] = val8; }
}

void image_blit(int x, int y, uint32_t color, image_t image)
{
    if ((uint32_t)x >= image.width || (uint32_t)y >= image.height) { return; }
    image.data[y * image.width + x] = color;
}

void image_blitx(point_t pos, uint32_t color, image_t image) { image_blit(pos.x, pos.y, color, image); }

void image_filledrect(int x, int y, int w, int h, uint32_t color, image_t image)
{
    for (int yy = 0; yy < h; yy++)
    {
        for (int xx = 0; xx < w; xx++) { image_blit(x + xx, y + yy, color, image); }
    }
}

void image_filledrectx(rectangle_t rect, uint32_t color, image_t image) { image_filledrect(rect.x, rect.y, rect.width, rect.height, color, image); }

void image_rect(int x, int y, int w, int h, int thickness, uint32_t color, image_t image)
{
    image_filledrect(x, y, w, thickness, color, image);
    image_filledrect(x, y + h - thickness, w, thickness, color, image);
    image_filledrect(x, y + thickness, thickness, h - (thickness * 2), color, image);
    image_filledrect(x + w - thickness, y + thickness, thickness, h - (thickness * 2), color, image);
}

void image_rectx(rectangle_t rect, int thickness, uint32_t color, image_t image) { image_rect(rect.x, rect.y, rect.width, rect.height, thickness, color, image); }

void image_char(int x, int y, char c, uint32_t fg, font_t font, image_t image)
{
    if (c == 0) { return; }
    uint32_t p = font.height * c;
    for (size_t cy = 0; cy < font.height; cy++)
    {
        for (size_t cx = 0; cx < font.width; cx++)
        {
            if (bit_address_from_byte(font.data[p + cy], cx + 1))
            { image_blit(x + (font.width - cx), y + cy, fg, image); }
        }
    }
}

void image_charx(point_t pos, char c, uint32_t fg, font_t font, image_t image) { image_char(pos.x, pos.y, c, fg, font, image); }

void image_char_bg(int x, int y, char c, uint32_t fg, uint32_t bg, font_t font, image_t image)
{
    image_filledrect(x, y, font_get_width(font), font_get_height(font), bg, image);
    
    uint32_t p = font.height * c;
    for (size_t cy = 0; cy < font.height; cy++)
    {
        for (size_t cx = 0; cx < font.width; cx++)
        {
            if (bit_address_from_byte(font.data[p + cy], cx + 1))
            { image_blit(x + (font.width - cx), y + cy, fg, image); }
        }
    }
}

void image_charx_bg(point_t pos, char c, uint32_t fg, uint32_t bg, font_t font, image_t image) { image_char_bg(pos.x, pos.y, c, fg, bg, font, image); }

void image_string(int x, int y, const char* txt, uint32_t fg, font_t font, image_t image)
{
    uint32_t len = strlen(txt);
    uint32_t xx = x, yy = y;
    for (uint32_t i = 0; i < len; i++)
    {
        if (txt[i] == 0) { break; }
        if (txt[i] == '\n') { xx = x; yy += font_get_height(font); continue; }
        if (isprint(txt[i])) { image_char(xx, yy, txt[i], fg, font, image); xx += font_get_width(font); }
    }
}

void image_stringx(point_t pos, const char* txt, uint32_t fg, font_t font, image_t image) { image_string(pos.x, pos.y, txt, fg, font, image); }

void image_string_bg(int x, int y, const char* txt, uint32_t fg, uint32_t bg, font_t font, image_t image)
{
    uint32_t len = strlen(txt);
    uint32_t xx = x, yy = y;
    for (uint32_t i = 0; i < len; i++)
    {
        if (txt[i] == 0) { break; }
        if (txt[i] == '\n') { xx = x; yy += font_get_height(font); continue; }
        if (isprint(txt[i])) { image_char_bg(xx, yy, txt[i], fg, bg, font, image); xx += font_get_width(font); }
    }
}

void image_stringx_bg(point_t pos, const char* txt, uint32_t fg, uint32_t bg, font_t font, image_t image) { image_string_bg(pos.x, pos.y, txt, fg, bg, font, image); }

void image_array(int x, int y, int w, int h, uint32_t* data, image_t image)
{

}

void image_arrayx(rectangle_t rect, uint32_t* data, image_t image) { image_array(rect.x, rect.y, rect.width, rect.height, data, image); }

void image_arraytrans(int x, int y, int w, int h, uint32_t trans, uint32_t* data, image_t image)
{

}

void image_arraytrans_x(rectangle_t rect, uint32_t trans, uint32_t* data, image_t image) { image_arraytrans(rect.x, rect.y, rect.width, rect.height, trans, data, image); }
