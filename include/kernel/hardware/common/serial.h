#pragma once
#include <lib/types.h>

typedef enum
{
    SERIALPORT_NONE = 0x000,
    SERIALPORT_COM1 = 0x3F8,
    SERIALPORT_COM2 = 0x2F8,
    SERIALPORT_COM3 = 0x3E8,
    SERIALPORT_COM4 = 0x2E8,
} SERIALPORT;

void        serial_setport(SERIALPORT port);
SERIALPORT  serial_getport();
const char* serial_getportstr(SERIALPORT port);

char serial_read();
void serial_writechar(char c);
void serial_writechar_col(char c, uint8_t color);
void serial_write(const char* str);
void serial_write_col(const char* str, uint8_t color);
void serial_writeln(const char* str);
void serial_writeln_col(const char* str, uint8_t color);
void serial_newline();
int  serial_vprintf(const char* str, va_list args);
int  serial_printf(const char* str, ...);
void serial_setcol(uint8_t color);
