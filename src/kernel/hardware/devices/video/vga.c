#include <kernel/hardware/devices/video/vga.h>
#include <kernel/core/kernel.h>

vga_info_t vga_info = { FALSE, FALSE, 0, 0, 0, 0, 0, 0, 0 };

void vga_init()
{   
    if (vga_info.initialized) { return FALSE; }
    vga_info.width       = 80;
    vga_info.height      = 25;
    vga_info.buffer      = (uint8_t*)0xB8000;
    vga_info.initialized = TRUE;

    vga_cursor_enable(0, 15);
    vga_set_cursor(0, 0);
    vga_set_fg(0x0F);
    vga_clear(0x00);
}

void vga_clear(uint8_t color)
{
    if (!vga_info.initialized) { return; }
    tlock();
    vga_info.back_color = color;
    for (uint32_t i = 0; i < vga_info.width * vga_info.height * 2; i += 2)
    {
        vga_info.buffer[i + 0] = 0x20;
        vga_info.buffer[i + 1] = vga_attribute(vga_info.fore_color, vga_info.back_color);
    }
    vga_set_cursor(0, 0);
    tunlock();
}

void vga_newline()
{
    if (!vga_info.initialized) { return; }
    vga_info.cursor_x = 0;
    vga_info.cursor_y++;
    if (vga_info.cursor_y >= vga_info.height) { vga_scroll(); }
    vga_update_cursor();
}

void vga_delete()
{
    if (!vga_info.initialized) { return; }
    if (vga_info.cursor_x > 0)
    {

    }
    else if (vga_info.cursor_y > 0)
    {

    }

    vga_update_cursor();
}

void vga_scroll()
{
    tlock();
    uint32_t line = vga_info.width * 2;
    uint32_t size = vga_info.width * vga_info.height * 2;
    uint8_t* dest = (uint8_t*)vga_info.buffer;
    uint8_t* src = (uint8_t*)((uint32_t)vga_info.buffer + line);
    memcpy(dest, src, size - line);
    for (uint16_t i = 0; i < vga_info.width; i++) { vga_putchar(i, vga_info.height - 1, 0x20, vga_info.fore_color, vga_info.back_color); }
    vga_set_cursor(0, vga_info.height - 1);
    tunlock();
}

void vga_putchar(uint16_t x, uint16_t y, char c, uint8_t fg, uint8_t bg)
{
    if (!vga_info.initialized) { return; }
    if (x >= vga_info.width || y >= vga_info.height) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "vga_putchar"); return; }
    vga_info.buffer[0 + ((y * vga_info.width + x) * 2)] = c;
    vga_info.buffer[1 + ((y * vga_info.width + x) * 2)] = vga_attribute(fg, bg);
}

void vga_putstr(uint16_t x, uint16_t y, const char* str, uint8_t fg, uint8_t bg)
{
    uint16_t x_old = vga_info.cursor_x;
    uint16_t y_old = vga_info.cursor_y;
    vga_set_cursor(x, y);
    vga_write_col(str, fg, bg);
    vga_set_cursor(x_old, y_old);
}

void vga_writechar(char c) { vga_writechar_col(c, vga_info.fore_color, vga_info.back_color); }

void vga_writechar_fg(char c, uint8_t fg) { vga_writechar_col(c, fg, vga_info.back_color); }

void vga_writechar_col(char c, uint8_t fg, uint8_t bg)
{
    if (!vga_info.initialized) { return; }
    if (c == '\n') { vga_newline(); return; }
    if (!isprint(c)) { return; }
    vga_putchar(vga_info.cursor_x, vga_info.cursor_y, c, fg, bg);
    vga_info.cursor_x++;
    if (vga_info.cursor_x >= vga_info.width) { vga_newline(); }
    vga_update_cursor();
}

void vga_write(const char* str) { vga_write_col(str, vga_info.fore_color, vga_info.back_color); }

void vga_write_fg(const char* str, uint8_t fg) { vga_write_col(str, fg, vga_info.back_color); }

void vga_write_col(const char* str, uint8_t fg, uint8_t bg)
{
    if (!vga_info.initialized) { return; }
    if (str == NULL)           { return; }
    if (strlen(str) == 0)      { return; }

    uint32_t i = 0;
    while (str[i] != 0) { vga_writechar_col(str[i], fg, bg); i++; }
}

void vga_writeln(const char* str) { vga_writeln_col(str, vga_info.fore_color, vga_info.back_color); }

void vga_writeln_fg(const char* str, uint8_t fg) { vga_writeln_col(str, fg, vga_info.back_color); }

void vga_writeln_col(const char* str, uint8_t fg, uint8_t bg)
{
    vga_write_col(str, fg, bg);
    vga_newline();
}

int vga_vprintf(const char* str, va_list args)
{
    while (*str != 0)
    {
        if (*str == '%')
        {
            str++;
            if (*str == '%') { vga_writechar('%'); }
            if (*str == 'c') { vga_writechar((char)va_arg(args, int)); }
            else if (*str == 'd' || *str == 'u')
            {
                int dec = va_arg(args, int);
                char str[16];
                memset(str, 0, 16);
                vga_write(itoa(dec, str, 10));
            }
            else if (*str == 'u')
            {
                uint32_t dec = va_arg(args, uint32_t);
                char str[16];
                memset(str, 0, 16);
                vga_write(ltoa(dec, str, 10));
            }
            else if (*str == 'x')
            {
                uint32_t dec = va_arg(args, uint32_t);
                char str[16];
                memset(str, 0, 16);
                vga_write(ltoa(dec, str, 16));
            }
            else if (*str == '2')
            {
                str++;
                if (*str != 'x' && *str != 'X') { vga_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    vga_write(strhex(num, str, FALSE, 1));
                }
            }
            else if (*str == '4')
            {
                str++;
                if (*str != 'x' && *str != 'X') { vga_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    vga_write(strhex(num, str, FALSE, 2));
                }
            }
            else if (*str == '8')
            {
                str++;
                if (*str != 'x' && *str != 'X') { vga_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    vga_write(strhex(num, str, FALSE, 4));
                }
            }
            else if (*str == 'f')
            {
                /* TODO : implement float to string */
            }
            else if (*str == 's')
            {
                char* val = va_arg(args, char*);
                vga_write(val);
            }
            else { vga_writechar(*str); }
        }
        else { vga_writechar(*str); }
        str++;
    }

    vga_update_cursor();
    return TRUE;
}

int vga_printf(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    int res = vga_vprintf(str, args);
    va_end(args);
    return res;
}

void vga_update_cursor()
{
    uint16_t pos = vga_info.cursor_y * vga_info.width + vga_info.cursor_x;
	port_outb(0x3D4, 0x0F);
	port_outb(0x3D5, (uint8_t) (pos & 0xFF));
	port_outb(0x3D4, 0x0E);
	port_outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void vga_set_cursor(uint16_t x, uint16_t y)
{
    if (x >= vga_info.width || y >= vga_info.height) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "vga_set_cursor"); return; }
    vga_info.cursor_x = x;
    vga_info.cursor_y = y;
    vga_update_cursor();
}

void vga_set_cursor_x(uint16_t x) { vga_set_cursor(x, vga_info.cursor_y); }

void vga_set_cursor_y(uint16_t y) { vga_set_cursor(vga_info.cursor_x, y); }

void vga_cursor_enable(uint8_t top, uint8_t bottom)
{
    vga_info.cursor_enabled = TRUE;

    port_outb(0x3D4, 0x0A);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xC0) | top);
 
	port_outb(0x3D4, 0x0B);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xE0) | bottom);
}

void vga_cursor_disable()
{
    vga_info.cursor_enabled = FALSE;
    port_outb(0x3D4, 0x0A);
	port_outb(0x3D5, 0x20);
}

void vga_set_colors(uint8_t fg, uint8_t bg) { vga_set_fg(fg); vga_set_bg(bg); }

void vga_set_fg(uint8_t fg) { vga_info.fore_color = fg; }
void vga_set_bg(uint8_t bg) { vga_info.back_color = bg; }

uint16_t vga_get_cursor_x() { return vga_info.cursor_x; }
uint16_t vga_get_cursor_y() { return vga_info.cursor_y; }

uint8_t vga_get_fg() { return vga_info.fore_color; }
uint8_t vga_get_bg() { return vga_info.back_color; }
uint8_t vga_attribute(uint8_t fg, uint8_t bg) { return (uint16_t)((bg << 4) | fg); }

uint16_t vga_get_width() { return vga_info.width; }
uint16_t vga_get_height() { return vga_info.height; }
