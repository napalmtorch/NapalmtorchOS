#pragma once
#include <lib/types.h>

typedef struct
{
    bool_t   initialized;
    bool_t   cursor_enabled;
    uint16_t cursor_x;
    uint16_t cursor_y;
    uint16_t width;
    uint16_t height;
    uint8_t  fore_color;
    uint8_t  back_color;
    uint8_t* buffer;
} PACKED vga_info_t;

void vga_init();

void vga_clear(uint8_t color);
void vga_newline();
void vga_delete();
void vga_scroll();

void vga_putchar(uint16_t x, uint16_t y, char c, uint8_t fg, uint8_t bg);
void vga_putstr(uint16_t x, uint16_t y, const char* str, uint8_t fg, uint8_t bg);
void vga_writechar(char c);
void vga_writechar_fg(char c, uint8_t fg);
void vga_writechar_col(char c, uint8_t fg, uint8_t bg);
void vga_write(const char* str);
void vga_write_fg(const char* str, uint8_t fg);
void vga_write_col(const char* str, uint8_t fg, uint8_t bg);
void vga_writeln(const char* str);
void vga_writeln_fg(const char* str, uint8_t fg);
void vga_writeln_col(const char* str, uint8_t fg, uint8_t bg);

int  vga_vprintf(const char* str, va_list args);
int  vga_printf(const char* str, ...);

void vga_update_cursor();
void vga_set_cursor(uint16_t x, uint16_t y);
void vga_set_cursor_x(uint16_t x);
void vga_set_cursor_y(uint16_t y);
void vga_cursor_enable(uint8_t top, uint8_t bottom);
void vga_cursor_disable();

void vga_set_colors(uint8_t fg, uint8_t bg);
void vga_set_fg(uint8_t fg);
void vga_set_bg(uint8_t bg);

uint16_t vga_get_cursor_x();
uint16_t vga_get_cursor_y();

uint8_t vga_get_fg();
uint8_t vga_get_bg();
uint8_t vga_attribute(uint8_t fg, uint8_t bg);

uint16_t vga_get_width();
uint16_t vga_get_height();