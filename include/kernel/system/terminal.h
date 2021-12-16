#pragma once
#include <lib/types.h>
#include <kernel/graphics/graphics.h>
#include <kernel/system/service.h>

void term_start(service_t* service);
void term_init();

void term_clear();
void term_clear_col(uint32_t color);

void term_newline();
void term_newlinex(int count);
void term_delete();
void term_deletex(int count);
void term_scroll();
void term_scrollx(int count);

void term_putchar(int x, int y, char c, uint32_t fg, uint32_t bg);
void term_writechar(char c);
void term_writechar_fg(char c, uint32_t fg);
void term_writechar_col(char c, uint32_t fg, uint32_t bg);
void term_write(const char* txt);
void term_write_fg(const char* txt, uint32_t fg);
void term_write_col(const char* txt, uint32_t fg, uint32_t bg);
void term_writeln(const char* txt);
void term_writeln_fg(const char* txt, uint32_t fg);
void term_writeln_col(const char* txt, uint32_t fg, uint32_t bg);
int  term_vprintf(const char* str, va_list args);
int  term_printf(const char* str, ...);

void    term_set_cursor(int x, int y);
void    term_set_cursor_x(int x);
void    term_set_cursor_y(int y);
point_t term_get_curpos();
int     term_get_cursor_x();
int     term_get_cursor_y();

void     term_set_colors(uint32_t fg, uint32_t bg);
void     term_set_fg(uint32_t fg);
void     term_set_bg(uint32_t bg);
uint32_t term_get_fg();
uint32_t term_get_bg();

uint32_t term_get_width();
uint32_t term_get_height();

void   term_set_font(font_t font);
font_t term_get_font();