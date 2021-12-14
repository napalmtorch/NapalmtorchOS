#include <kernel/hardware/devices/video/vga.h>
#include <kernel/core/kernel.h>

// vga info structure
vga_info_t vga_info = { FALSE, FALSE, 0, 0, 0, 0, 0, 0, 0 };

// initialize vga driver with default mode
void vga_init()
{   
    // check if vga has already been initialized
    if (vga_info.initialized) { return FALSE; }

    // set info properties
    vga_info.width       = 80;
    vga_info.height      = 25;
    vga_info.buffer      = (uint8_t*)0xB8000;
    vga_info.initialized = TRUE;

    // setup cursor
    vga_cursor_enable(0, 15);
    vga_set_cursor(0, 0);

    // set colors and clear screen
    vga_set_fg(0x0F);
    vga_clear(0x00);
}

// clear vga screen with specified color
void vga_clear(uint8_t color)
{
    // validate
    if (!vga_info.initialized) { return; }

    // set background color
    vga_info.back_color = color;

    // clear screen data
    for (uint32_t i = 0; i < vga_info.width * vga_info.height * 2; i += 2)
    {
        vga_info.buffer[i + 0] = 0x20;
        vga_info.buffer[i + 1] = vga_attribute(vga_info.fore_color, vga_info.back_color);
    }

    // reset cursor
    vga_set_cursor(0, 0);
}

// generate a newline on vga screen
void vga_newline()
{
    // validate
    if (!vga_info.initialized) { return; }

    // set cursor
    vga_info.cursor_x = 0;
    vga_info.cursor_y++;

    // check if scroll is required
    if (vga_info.cursor_y >= vga_info.height) { vga_scroll(); }

    // update vga screen cursor
    vga_update_cursor();
}

// delete character on vga screen at cursor position
void vga_delete()
{
    // validate
    if (!vga_info.initialized) { return; }

    // not yet implemented - am lazy
    if (vga_info.cursor_x > 0) { }
    else if (vga_info.cursor_y > 0) { }

    // update vga screen cursor
    vga_update_cursor();
}

// scroll the vga screen by one line
void vga_scroll()
{
    // calculate required values to perform scroll
    uint32_t line = vga_info.width * 2;
    uint32_t size = vga_info.width * vga_info.height * 2;
    uint8_t* dest = (uint8_t*)vga_info.buffer;
    uint8_t* src = (uint8_t*)((uint32_t)vga_info.buffer + line);

    // move all data up 1 line
    memcpy(dest, src, size - line);

    // clear bottom line
    for (uint16_t i = 0; i < vga_info.width; i++) { vga_putchar(i, vga_info.height - 1, 0x20, vga_info.fore_color, vga_info.back_color); }

    // update cursor position to bottom of screen
    vga_set_cursor(0, vga_info.height - 1);
}

// put character at specified position on vga screen
void vga_putchar(uint16_t x, uint16_t y, char c, uint8_t fg, uint8_t bg)
{
    // validate
    if (!vga_info.initialized) { return; }
    if (x >= vga_info.width || y >= vga_info.height) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "vga_putchar"); return; }

    // set character in buffer
    vga_info.buffer[0 + ((y * vga_info.width + x) * 2)] = c;
    // set color attribute in buffer
    vga_info.buffer[1 + ((y * vga_info.width + x) * 2)] = vga_attribute(fg, bg);
}

// put screen at specified position on vga screen
void vga_putstr(uint16_t x, uint16_t y, const char* str, uint8_t fg, uint8_t bg)
{
    // save old cursor position
    uint16_t x_old = vga_info.cursor_x;
    uint16_t y_old = vga_info.cursor_y;
    
    // set cursor position and write string at it
    vga_set_cursor(x, y);
    vga_write_col(str, fg, bg);
    
    // restore cursor position
    vga_set_cursor(x_old, y_old);
}

// write character to current position
void vga_writechar(char c) { vga_writechar_col(c, vga_info.fore_color, vga_info.back_color); }

// write character with fore color to current position
void vga_writechar_fg(char c, uint8_t fg) { vga_writechar_col(c, fg, vga_info.back_color); }

// write character with fore color and back color to current position
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

// write string to current position
void vga_write(const char* str) { vga_write_col(str, vga_info.fore_color, vga_info.back_color); }

// write string with fore color to current position
void vga_write_fg(const char* str, uint8_t fg) { vga_write_col(str, fg, vga_info.back_color); }

// write string with fore color and back color to current position
void vga_write_col(const char* str, uint8_t fg, uint8_t bg)
{
    if (!vga_info.initialized) { return; }
    if (str == NULL)           { return; }
    if (strlen(str) == 0)      { return; }

    uint32_t i = 0;
    while (str[i] != 0) { vga_writechar_col(str[i], fg, bg); i++; }
}

// write line to current position
void vga_writeln(const char* str) { vga_writeln_col(str, vga_info.fore_color, vga_info.back_color); }

// write line with fore color to current position
void vga_writeln_fg(const char* str, uint8_t fg) { vga_writeln_col(str, fg, vga_info.back_color); }

// write line with fore color and back color to current position
void vga_writeln_col(const char* str, uint8_t fg, uint8_t bg)
{
    vga_write_col(str, fg, bg);
    vga_newline();
}

// write formatted string with manually specified arguments to vga screen
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

// write formatted string with to vga screen
int vga_printf(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    int res = vga_vprintf(str, args);
    va_end(args);
    return res;
}

// send current vga cursor position to vga io ports
void vga_update_cursor()
{
    // calculate offset
    uint16_t pos = vga_info.cursor_y * vga_info.width + vga_info.cursor_x;

    // set offset to io ports
	port_outb(0x3D4, 0x0F);
	port_outb(0x3D5, (uint8_t) (pos & 0xFF));
	port_outb(0x3D4, 0x0E);
	port_outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

// set current vga cursor position
void vga_set_cursor(uint16_t x, uint16_t y)
{
    // validate position
    if (x >= vga_info.width || y >= vga_info.height) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "vga_set_cursor"); return; }

    // set position and update cursor
    vga_info.cursor_x = x;
    vga_info.cursor_y = y;
    vga_update_cursor();
}

// set current vga cursor x position
void vga_set_cursor_x(uint16_t x) { vga_set_cursor(x, vga_info.cursor_y); }

// set current vga cursor y position
void vga_set_cursor_y(uint16_t y) { vga_set_cursor(vga_info.cursor_x, y); }

// enable vga cursor with specified size
void vga_cursor_enable(uint8_t top, uint8_t bottom)
{
    vga_info.cursor_enabled = TRUE;

    port_outb(0x3D4, 0x0A);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xC0) | top);
 
	port_outb(0x3D4, 0x0B);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xE0) | bottom);
}

// disable vga cursor
void vga_cursor_disable()
{
    vga_info.cursor_enabled = FALSE;
    port_outb(0x3D4, 0x0A);
	port_outb(0x3D5, 0x20);
}

// set default vga colors
void vga_set_colors(uint8_t fg, uint8_t bg) { vga_set_fg(fg); vga_set_bg(bg); }

// set default vga fore color
void vga_set_fg(uint8_t fg) { vga_info.fore_color = fg; }

// set default vga back color
void vga_set_bg(uint8_t bg) { vga_info.back_color = bg; }

// get current vga cursor x position
uint16_t vga_get_cursor_x() { return vga_info.cursor_x; }

// get current vga cursor y position
uint16_t vga_get_cursor_y() { return vga_info.cursor_y; }

// get default vga fore color
uint8_t vga_get_fg() { return vga_info.fore_color; }

// get default vga back color
uint8_t vga_get_bg() { return vga_info.back_color; }

// pack fore color and back color into vga attribute
uint8_t vga_attribute(uint8_t fg, uint8_t bg) { return (uint16_t)((bg << 4) | fg); }

// get current vga screen width
uint16_t vga_get_width() { return vga_info.width; }

// get current vga screen height
uint16_t vga_get_height() { return vga_info.height; }
