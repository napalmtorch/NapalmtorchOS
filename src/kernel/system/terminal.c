#include <kernel/system/terminal.h>
#include <kernel/core/kernel.h>

service_t* term_service;
uint32_t   term_cx, term_cy;
uint32_t   term_width, term_height;
uint32_t   term_fg, term_bg;
font_t     term_font;

void term_start(service_t* service)
{
    term_set_colors(COL32_WHITE, COL32_BLACK);

    term_font = FONT_SYSTEM_8x16;
    term_width = vesa_get_width() / font_get_width(term_font);
    term_height = (vesa_get_height() / font_get_height(term_font)) - 1;

    term_clear();
}

void term_stop(service_t* service)
{
    term_set_cursor(0, 0);
    term_width = 0;
    term_height = 0;
}

void term_init()
{
    term_service = service_create("Terminal", SERVICETYPE_INTERFACE, term_start, term_stop);
    if (!service_register(term_service)) { return; }

    term_service->initialized = TRUE;
    if (!service_start(term_service)) { return; }
}

void term_drawcur()
{
    gfx_filledrect(term_cx * font_get_width(term_font), term_cy * font_get_height(term_font), font_get_width(term_font), font_get_height(term_font), term_fg);
}

void term_clearcur()
{
    gfx_filledrect(term_cx * font_get_width(term_font), term_cy * font_get_height(term_font), font_get_width(term_font), font_get_height(term_font), term_bg);
}

void term_clear() { term_clear_col(term_bg); }

void term_clear_col(uint32_t color)
{
    if (!term_service->started) { return; }
    term_bg = color;
    gfx_clear(color);
    term_set_cursor(0, 0);
    term_drawcur();
}

void term_newline()
{
    if (!term_service->started) { return; }
    term_clearcur();
    term_set_cursor(0, term_cy + 1);
    if (term_cy >= term_height) { term_scroll(); }
    term_drawcur();
}

void term_newlinex(int count)
{
    for (int i = 0; i < count; i++) { term_newline(); }
}

void term_delete()
{
    if (!term_service->started) { return; }

    term_clearcur();

    if (term_cx > 0)
    {
        term_cx--;
        term_putchar(term_cx, term_cy, 0x20, term_fg, term_bg);
    }
    else if (term_cy > 0)
    {
        term_cx = term_width - 1;
        term_cy--;
        term_putchar(term_cx, term_cy, 0x20, term_fg, term_bg);
    }

    term_drawcur();
}

void term_deletex(int count)
{
    for (int i = 0; i < count; i++) { term_delete(); }
}

void term_scroll()
{
    if (!term_service->started) { return; }

    term_clearcur();

    gfx_filledrect(0, vesa_get_height() - font_get_height(term_font), vesa_get_width(), font_get_height(term_font), term_get_bg());
    uint32_t line  = (vesa_get_width() * 4) * font_get_height(term_font);
    uint32_t start = vesa_get_buffer();
    uint32_t size  = vesa_get_width() * vesa_get_height() * 4;
    memcpy(start, start + line, size - line);
    for (int i = 0; i < term_width; i++) { term_putchar(i, term_height - 1, 0x20, term_fg, term_bg); }
    term_set_cursor(0, term_height - 1);
    term_drawcur();
}

void term_scrollx(int count)
{
    for (int i = 0; i < count; i++) { term_scroll(); }
}

void term_putchar(int x, int y, char c, uint32_t fg, uint32_t bg)
{
    if (!term_service->started) { return; }
    if ((uint32_t)x >= term_width || (uint32_t)y >+ term_height) { return; }
    gfx_char_bg(x * font_get_width(term_font), y * font_get_height(term_font), c, fg, bg, term_font);
}

void term_writechar(char c) { term_writechar_col(c, term_fg, term_bg); }

void term_writechar_fg(char c, uint32_t fg) { term_writechar_col(c, fg, term_bg); }

void term_writechar_col(char c, uint32_t fg, uint32_t bg)
{
    if (!term_service->started) { return; }
    if (c == '\n') { term_newline(); return; }
    if (c >= 32 && c <= 126)
    {
        term_putchar(term_cx, term_cy, c, fg, bg);
        term_cx++;
        if (term_cx >= term_width) { term_newline(); }
    }

    term_drawcur();
}

void term_write(const char* txt) { term_write_col(txt, term_fg, term_bg); }

void term_write_fg(const char* txt, uint32_t fg) { term_write_col(txt, fg, term_bg); }

void term_write_col(const char* txt, uint32_t fg, uint32_t bg)
{
    if (!term_service->started) { return; }
    uint32_t len = strlen(txt);
    for (uint32_t i = 0; i < len; i++) { term_writechar_col(txt[i], fg, bg); }
}

void term_writeln(const char* txt) { term_writeln_col(txt, term_fg, term_bg); }

void term_writeln_fg(const char* txt, uint32_t fg) { term_writeln_col(txt, fg, term_bg); }

void term_writeln_col(const char* txt, uint32_t fg, uint32_t bg)
{
    if (!term_service->started) { return; }
    term_write_col(txt, fg, bg);
    term_newline();
}

int term_vprintf(const char* str, va_list args)
{
    while (*str != 0)
    {
        if (*str == '%')
        {
            str++;
            if (*str == '%') { term_writechar('%'); }
            if (*str == 'c') { term_writechar((char)va_arg(args, int)); }
            else if (*str == 'd' || *str == 'u')
            {
                int dec = va_arg(args, int);
                char str[16];
                memset(str, 0, 16);
                term_write(itoa(dec, str, 10));
            }
            else if (*str == 'u')
            {
                uint32_t dec = va_arg(args, uint32_t);
                char str[16];
                memset(str, 0, 16);
                term_write(ltoa(dec, str, 10));
            }
            else if (*str == 'x')
            {
                uint32_t dec = va_arg(args, uint32_t);
                char str[16];
                memset(str, 0, 16);
                term_write(ltoa(dec, str, 16));
            }
            else if (*str == '2')
            {
                str++;
                if (*str != 'x' && *str != 'X') { term_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    term_write(strhex(num, str, FALSE, 1));
                }
            }
            else if (*str == '4')
            {
                str++;
                if (*str != 'x' && *str != 'X') { term_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    term_write(strhex(num, str, FALSE, 2));
                }
            }
            else if (*str == '8')
            {
                str++;
                if (*str != 'x' && *str != 'X') { term_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    term_write(strhex(num, str, FALSE, 4));
                }
            }
            else if (*str == 'f')
            {
                double num = va_arg(args, double);
                char str[32];
                memset(str, 0, 32);
                term_write(ftoa(num, str, 2));
            }
            else if (*str == 's')
            {
                char* val = va_arg(args, char*);
                term_write(val);
            }
            else { term_writechar(*str); }
        }
        else { term_writechar(*str); }
        str++;
    }
}

int term_printf(const char* str, ...)
{
    if (!term_service->started) { return; }
    va_list args;
    va_start(args, str);
    term_vprintf(str, args);
    va_end(args);
}

void term_set_cursor(int x, int y)
{
     if (!term_service->started) { return; }
    term_clearcur();
    term_cx = x;
    term_cy = y;
    term_drawcur();
}

void term_set_cursor_x(int x)
{
    if (!term_service->started) { return; }
    term_clearcur();
    term_cx = x;
    term_drawcur();
}

void term_set_cursor_y(int y)
{
    if (!term_service->started) { return; }
    term_clearcur();
    term_cy = y;
    term_drawcur();
}

point_t term_get_curpos()
{
    point_t pos = { term_cx, term_cy };
    return pos;
}

int term_get_cursor_x() { return term_cx; }
int term_get_cursor_y() { return term_cy; }

void term_set_colors(uint32_t fg, uint32_t bg)
{
    if (!term_service->started) { return; }
    term_fg = fg;
    term_bg = bg;
}

void term_set_fg(uint32_t fg)
{
    if (!term_service->started) { return; }
    term_fg = fg;
}

void term_set_bg(uint32_t bg)
{
    if (!term_service->started) { return; }
    term_bg = bg;
}

uint32_t term_get_fg() { return term_fg; }

uint32_t term_get_bg() { return term_bg; }

uint32_t term_get_width() { return term_width; }

uint32_t term_get_height() { return term_height; }

void term_set_font(font_t font) { term_font = font; }

font_t term_get_font() { return term_font; }